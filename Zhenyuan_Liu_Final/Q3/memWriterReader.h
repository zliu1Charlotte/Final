#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

class memWriterReader : public sc_module {
public:
	tlm_utils::simple_initiator_socket<memWriterReader, 32> memWRSocket;  

	SC_CTOR(memWriterReader) : memWRSocket("mem_WR_socket"), nBlockWriteRead(0),
							   memWR_peq(this, &memWriterReader::memWR_cb_peq){
		nBlockWriteRead = new tlm::tlm_generic_payload();
		SC_THREAD(nbMemWR);
		memWRSocket.register_nb_transport_bw(this, &memWriterReader::nb_transport_bw);	
	}
	tlm::tlm_generic_payload* nBlockWriteRead;
	void nbMemWR();
	void doSomethingGood(tlm::tlm_generic_payload& );
	tlm_utils::peq_with_cb_and_phase<memWriterReader> memWR_peq;
	void memWR_cb_peq(tlm::tlm_generic_payload&, const tlm::tlm_phase& );	
	virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_time&);

	int data[3];
};

