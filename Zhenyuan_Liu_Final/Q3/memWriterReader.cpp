#include "memWriterReader.h"

void memWriterReader::nbMemWR(){
	tlm::tlm_phase forwardPhase;
	sc_time delay;
	for (int i = 0; i < 9; i = i + 3){
		tlm::tlm_command cmd = (tlm::tlm_command)(rand() % 2);
		if (cmd == tlm::TLM_WRITE_COMMAND) {
			data[0] = (i+1); data[1] = (i+2); data[2] = (i+3);
			//data[3] = (i+4); data[4] = (i+5);
		}
		nBlockWriteRead->set_command( cmd );
		nBlockWriteRead->set_address( i );
		nBlockWriteRead->set_data_ptr( (unsigned char*) data );
		nBlockWriteRead->set_data_length( 3 );
		nBlockWriteRead->set_streaming_width( 3 );
		nBlockWriteRead->set_byte_enable_ptr( 0 );
		nBlockWriteRead->set_dmi_allowed( false );
		nBlockWriteRead->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

		forwardPhase = tlm::BEGIN_REQ;
		delay = sc_time(0, SC_NS);
		cout << "************************************************************************** " << endl;
		cout << "==> The initiator sends BEGIN_REQ for " << (cmd ? 'W' : 'R');
		if (cmd == 1) {cout << " data: ";
		for (int j = 0; j<3; j++) cout << data[j] << " "; }
		cout << " at " << sc_time_stamp() <<" delay=" << delay << '\n';

		tlm::tlm_sync_enum returnStatus;
		returnStatus = memWRSocket->
			nb_transport_fw(*nBlockWriteRead, forwardPhase, delay);
		cout << "<-- Return path for BEGIN_REQ is " << returnStatus << " at " << sc_time_stamp() << endl;
		wait(130 , SC_NS); 
	}
}

void memWriterReader::doSomethingGood( tlm::tlm_generic_payload& completeTrans)
{
	if ( completeTrans.is_response_error() )
		SC_REPORT_ERROR("TLM-2", "error...\n");
	tlm::tlm_command cmd = completeTrans.get_command();
	uint64           adr = completeTrans.get_address();
	int*             ptr = reinterpret_cast<int*>( completeTrans.get_data_ptr() );
	if (cmd == tlm::TLM_READ_COMMAND){
		cout << "The incoming data from memArray is ready to process: ";
		for (int j = 0; j < 3; j++) cout << *(ptr + j) << " ";
		cout << "at " << sc_time_stamp() << '\n';
	}
}


void memWriterReader::memWR_cb_peq(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase){

	if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP)
		SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
	else if (phase == tlm::BEGIN_RESP){
		tlm::tlm_phase forwardPhase = tlm::END_RESP;
		sc_time delay = sc_time(30, SC_NS);
		cout << "==> The initiator sends END_RESP at " << sc_time_stamp() << " delay=" << delay << endl;
		tlm::tlm_sync_enum returnStatus = 
			memWRSocket->nb_transport_fw(trans, forwardPhase, delay);
		cout << "<-- Return path for END_RESP is " << returnStatus << " at " << sc_time_stamp() << endl;
		doSomethingGood(*nBlockWriteRead);
	}
}


tlm::tlm_sync_enum memWriterReader::nb_transport_bw(tlm::tlm_generic_payload& trans,
	tlm::tlm_phase& phase, sc_time& delay){
	memWR_peq.notify(trans, phase, delay);
	return tlm::TLM_ACCEPTED;
}