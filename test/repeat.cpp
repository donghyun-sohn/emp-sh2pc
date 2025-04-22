#include "emp-sh2pc/emp-sh2pc.h"
#include <chrono>
using namespace emp;
using namespace std;
using namespace std::chrono;

int party;
int port = 12345;
NetIO * netio;
void setup() {
	usleep(100);
	netio =  new emp::NetIO(party == emp::ALICE ? nullptr : "127.0.0.1", port, true);
	emp::setup_semi_honest(netio, party,  1024);
}
void done() {
	delete netio;
	finalize_semi_honest();
}

void test_int_reveal(int number) {
	setup();
	Integer a(32, number, ALICE);
	Integer b;
	for(int i = 0; i < 1000; ++i)
		b = Integer(32, number+1, BOB);
	int32_t aa = a.reveal<int32_t>(PUBLIC);
	int32_t bb = b.reveal<int32_t>(PUBLIC);

	if(aa != number)error("int a!\n");
	if(bb != number+1) error("int b!\n");
	done();
}

int main(int argc, char** argv) {
    int party, port;
    parse_party_and_port(argv, &party, &port);
    NetIO * io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);
    setup_semi_honest(io, party);

    int and_before = CircuitExecution::circ_exec->num_and();
    auto time_start = high_resolution_clock::now();

    for(int i = 0; i < 2500000; ++i) {
        Integer a(32, 123, ALICE);
        Integer b(32, 124, BOB);
        Bit res = a > b;
        Integer c = a + b;
    }

    auto time_end = high_resolution_clock::now();
    int and_after = CircuitExecution::circ_exec->num_and();
    auto elapsed_ms = duration_cast<milliseconds>(time_end - time_start).count();

    cout << "AND gates used: " << (and_after - and_before) << endl;
    cout << "Runtime (ms): " << elapsed_ms << endl;

    delete io;
}
