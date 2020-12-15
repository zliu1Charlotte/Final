#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

class memoryUnit : public sc_module {
public:
	tlm_utils::simple_target_socket<memoryUnit, 32> memSocket;
	static const int SIZE=32;

	SC_CTOR(memoryUnit) : memSocket("memory_side_socket"), 
		mem_peq(this, &memoryUnit::mem_cb_peq) {
		
		memSocket.register_nb_transport_fw(this, &memoryUnit::nb_transport_fw);
		for (int i = 0; i < SIZE; i++)
			memArray[i] = (i + 82);
	}
	virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_time& );
	tlm_utils::peq_with_cb_and_phase<memoryUnit> mem_peq;
	void mem_cb_peq(tlm::tlm_generic_payload&, const tlm::tlm_phase&);
	void send_ENDREQ(tlm::tlm_generic_payload&);
	void send_BEGINRESP(tlm::tlm_generic_payload& );
	int memArray[SIZE];
};

