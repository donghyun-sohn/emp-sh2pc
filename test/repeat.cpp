#include "emp-sh2pc/emp-sh2pc.h"
#include <chrono>
using namespace emp;
using namespace std;
using namespace std::chrono;

int party;
int port = 12345;
NetIO* netio;

void setup(const char* ip_addr) {
    usleep(100);
    netio = new NetIO(party == ALICE ? ip_addr : nullptr, port, true);
    setup_semi_honest(netio, party, 1024);

    if (party == ALICE) {
        cout << "[Alice] Connected to Bob at " << ip_addr << ":" << port << endl;
    } else {
        cout << "[Bob] Listening on port " << port << "..." << endl;
    }
}

void done() {
    delete netio;
    finalize_semi_honest();
}

int main(int argc, char** argv) {
    parse_party_and_port(argv, &party, &port);

    const char* ip_addr = (party == ALICE) ? argv[3] : nullptr;
    setup(ip_addr);

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

    done();
}

