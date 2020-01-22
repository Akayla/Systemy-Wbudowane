#include <systemc.h>

using namespace std;

SC_MODULE(cpu2)
{
	sc_fifo_in<int> input;
	sc_inout<sc_uint<8>> hexes[6];
	sc_inout<sc_uint<8>> diodes;

	void task1() {
		
		while (1) {
			sc_uint<8> progNo = input.read();

			if (progNo == 66) {

				cout << "02 Green" << endl;
				diodes.write(66);
				hexes[5].write(2);
				hexes[4].write('G');
				hexes[3].write('R');
				hexes[2].write('E');
				hexes[1].write('E');
				hexes[0].write('N');
			}

			else if (progNo == 34) {

				cout << "02 Yellow" << endl;
				diodes.write(34);
				hexes[5].write(2);
				hexes[4].write('Y');
				hexes[3].write('E');
				hexes[2].write('L');
				hexes[1].write('L');
				hexes[0].write('O');
			}

			else if (progNo == 18) {

				cout << "02 Red" << endl;
				diodes.write(18);
				hexes[5].write(2);
				hexes[4].write('R');
				hexes[3].write('E');
				hexes[2].write('D');
				hexes[1].write(0);
				hexes[0].write(0);
			}

			wait(10, SC_MS);
		}
	}

	SC_CTOR(cpu2) {
		SC_THREAD(task1);
	}
};