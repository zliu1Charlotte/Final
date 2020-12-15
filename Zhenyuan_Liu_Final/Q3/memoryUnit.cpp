#include "memoryUnit.h"

tlm::tlm_sync_enum memoryUnit::nb_transport_fw
       (tlm::tlm_generic_payload& receivedTrans,
        tlm::tlm_phase& phase, sc_time& delay ){

	tlm::tlm_command cmd = receivedTrans.get_command();
	uint64           adr = receivedTrans.get_address();
	unsigned char*   ptr = receivedTrans.get_data_ptr();
	unsigned int     len = receivedTrans.get_data_length();
	unsigned char*   byt = receivedTrans.get_byte_enable_ptr();
	unsigned int     wid = receivedTrans.get_streaming_width();
	if (byt != 0) {
		receivedTrans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
		return tlm::TLM_COMPLETED;}
	if (len > 3 || wid < len) {
		receivedTrans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
		return tlm::TLM_COMPLETED;}
	
	mem_peq.notify(receivedTrans, phase, delay);
	return tlm::TLM_ACCEPTED;
}


void memoryUnit::mem_cb_peq(tlm::tlm_generic_payload& receivedTrans, const tlm::tlm_phase& phase){
	switch (phase) {
	case tlm::BEGIN_REQ:
		send_ENDREQ(receivedTrans);
		break;
	case tlm::BEGIN_RESP:
		send_BEGINRESP(receivedTrans);
		break;
	case tlm::END_REQ:
		SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by target");
		break;
	case tlm::END_RESP:
		cout << "The current transaction was completed at " << sc_time_stamp() << endl;
		break;
	default:
		SC_REPORT_FATAL("TLM-2", "Illegal phase!");
		break;
	}
}


void memoryUnit::send_ENDREQ(tlm::tlm_generic_payload& receivedTrans){
	tlm::tlm_sync_enum status;
	tlm::tlm_phase backwardPhase;
	sc_time delay;
	
	backwardPhase = tlm::END_REQ;
	delay = sc_time(300, SC_NS); 
	cout << "<== The target sends END_REQ at " << sc_time_stamp() << " delay=" << delay << endl;
	status = memSocket->nb_transport_bw(receivedTrans, backwardPhase, delay);
	cout << "--> Return path for END_REQ is " << status << " at " << sc_time_stamp() << endl;

	tlm::tlm_phase int_phase = tlm::BEGIN_RESP;
	delay = delay + sc_time(50, SC_NS); 
	mem_peq.notify(receivedTrans, int_phase, delay);
}


void memoryUnit::send_BEGINRESP(tlm::tlm_generic_payload& receivedTrans){
	tlm::tlm_sync_enum status;
	tlm::tlm_phase backwardPhase;
	sc_time delay;

	tlm::tlm_command cmd = receivedTrans.get_command();
	uint64           adr = receivedTrans.get_address();
	unsigned char*   ptr = receivedTrans.get_data_ptr();
	unsigned int     len = receivedTrans.get_data_length();

	unsigned int i;
	if (cmd == tlm::TLM_READ_COMMAND)
		for (i = 0; i<len; i = i + 1)
			*(ptr + 4*i) = *((unsigned char*)(memArray + adr + i));
	else if (cmd == tlm::TLM_WRITE_COMMAND)
		for (i = 0; i < len; i = i + 1) 
			*((unsigned char*)(memArray + adr + i)) = *(ptr + 4*i);
	
	receivedTrans.set_response_status(tlm::TLM_OK_RESPONSE);
	backwardPhase = tlm::BEGIN_RESP;
	delay = SC_ZERO_TIME;
	cout << "<== The target sends BEGIN_RESP at " << sc_time_stamp() << " delay=" << delay << endl;
	status = memSocket->nb_transport_bw(receivedTrans, backwardPhase, delay);
	cout << "--> Return path for BEGIN_RESP is " << status << " at " << sc_time_stamp() << endl;
	if (cmd == tlm::TLM_READ_COMMAND) cout << "The data is read from memArray: ";
	else cout << "The data is written in memArray: ";
	for (int j = 0; j<3; j++) cout << static_cast<int>(*((unsigned char*)(memArray + adr + j))) << " ";
	cout << "at " << sc_time_stamp() << endl;
}

