#include <systemc.h>
#include <conio.h>

using namespace std;

SC_MODULE(cpu1) {
	sc_inout<sc_uint<8>> switches;
	sc_inout<sc_uint<8>> hexes[6];
	sc_inout<sc_uint<8>> diodes;
	sc_fifo_out<int> output;


	bool er = false;
	bool er_read = false;
	int last = 0;
	int count;
	bool constructed = false;

	bool sw_states[5] = { false, false, false, false, false};

	/* Obsluga menu + error */
	void task1() {

		int f = 0;
		while (1) {
			count = 0;
			wait(500, SC_NS);

			if (_kbhit()) {

				displayHEX();
				displayLED();
				displaySW();
				wait(25, SC_NS);

				cout << "Switch: ";
				cin >> f;

				if (f >= 0 && f <= 8)
				{
					wait(500, SC_NS);
					sc_uint<8> sw_state = switches.read() ^ toBitFlag(f);
					switches.write(sw_state);
					wait(500, SC_NS);
				}
				else
				{
					cout << "switch poza zakresem! <0-8>" << endl;
				}
			}

			wait(500, SC_NS);
			if (f == 2 && diodes.read() != 2) {
				diodes.write(2);
				output.write(f);
			}

			wait(500, SC_NS);
			sc_uint<8> sw_state = switches.read();
			sc_uint<8> oldest_bit = 0x80;
			wait(500, SC_NS);

			for (int i = 7; i >= 0; i--) {
				if (oldest_bit & sw_state) count++;
				oldest_bit = oldest_bit >> 1;
			}

			wait(500, SC_NS);

			/* Error */
			if ( count > 1 ) {
				diodes.write(128);
				hexes[5].write(' ');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');
			} else if (count == 0) {
				diodes.write(sw_state);
				for (int i = 0; i < 6; i++)
					hexes[i].write(' ');
			}
			wait(500, SC_NS);
			switches.write(sw_state);
			wait(100, SC_MS);
		}
	}

	void task2() {
		while (1) {
			sc_uint<8> progNo = switches.read();

			if (progNo == 65 ) {

				cout << "01 Green" << endl;
				diodes.write(65);
				hexes[5].write(1);
				hexes[4].write('G');
				hexes[3].write('R');
				hexes[2].write('E');
				hexes[1].write('E');
				hexes[0].write('N');
			}

			else if (progNo == 33 ) {

				cout << "01 Yellow" << endl;
				diodes.write(33);
				hexes[5].write(1);
				hexes[4].write('Y');
				hexes[3].write('E');
				hexes[2].write('L');
				hexes[1].write('L');
				hexes[0].write('O');
			}

			else if (progNo == 17 ) {

				cout << "01 Red" << endl;
				diodes.write(17);
				hexes[5].write(1);
				hexes[4].write('R');
				hexes[3].write('E');
				hexes[2].write('D');
				hexes[1].write(0);
				hexes[0].write(0);
			}

			wait(10, SC_MS);
		}
	}

	void task3() {
		while (1) {
			sc_uint<8> progNo = switches.read();

			if (progNo == 68) {

				cout << "03 Green" << endl;
				diodes.write(68);
				hexes[5].write(3);
				hexes[4].write('G');
				hexes[3].write('R');
				hexes[2].write('E');
				hexes[1].write('E');
				hexes[0].write('N');
			}

			else if (progNo == 36) {

				cout << "03 Yellow" << endl;
				diodes.write(36);
				hexes[5].write(3);
				hexes[4].write('Y');
				hexes[3].write('E');
				hexes[2].write('L');
				hexes[1].write('L');
				hexes[0].write('O');
			}

			else if (progNo == 20) {

				cout << "03 Red" << endl;
				diodes.write(20);
				hexes[5].write(3);
				hexes[4].write('R');
				hexes[3].write('E');
				hexes[2].write('D');
				hexes[1].write(0);
				hexes[0].write(0);
			}

			wait(10, SC_MS);
		}
	}

	void displayLED() {
		sc_uint<8> diodes_state = diodes.read(); 
		sc_uint<8> old_bit = 0x80;
		cout << "LED:[ ";

		for (int i = 7; i >= 0; i--) {
			if (diodes_state & old_bit) {
				cout << "1";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			else {
				cout << "0";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			old_bit = old_bit >> 1;
		}

		cout << endl;
		cout << "Nr.:[ 8 ][ 7 ][ 6 ][ 5 ][ 4 ][ 3 ][ 2 ][ 1 ]" << endl;

	}

	void displaySW() {
		sc_uint<8> switches_state = switches.read();
		sc_uint<8> old_bit = 0x80;
		cout << "SWI:[ ";

		for (int i = 7; i >= 0; i--) {
			if (switches_state & old_bit) {
				cout << "1";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			else {
				cout << "0";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			old_bit = old_bit >> 1;
		}
		cout << endl;
		cout << "Nr.:[ 8 ][ 7 ][ 6 ][ 5 ][ 4 ][ 3 ][ 2 ][ 1 ]" << endl;
	}


	void displayHEX() {
		if (!constructed) postConstruct();

		sc_uint<8> hex[6];
		for (int i = 0; i < 6; i++)
			hex[i] = hexes[i].read();


		cout << "HEX:          [ ";
		for (int i = 5; i >= 0; i--) {
			if (hex[i] >= 0 && hex[i] <= 6) {
				cout << hex[i];
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			else if (hex[i] == (sc_uint<8>) 'E') {
				cout << "E";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
			else if (hex[i] == (sc_uint<8>) 'R') {
				cout << "R";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 
			else if (hex[i] == (sc_uint<8>) 'O') {
				cout << "O";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 
			else if (hex[i] == (sc_uint<8>) 'G') {
				cout << "G";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 
			else if (hex[i] == (sc_uint<8>) 'D') {
				cout << "D";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 
			else if (hex[i] == (sc_uint<8>) 'N') {
				cout << "N";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 
			else if (hex[i] == (sc_uint<8>) 'Y') {
				cout << "Y";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			} 

			else if (hex[i] == (sc_uint<8>) 'L') {
				cout << "L";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}

			else if (hex[i] == (sc_uint<8>) ' ') {
				cout << " ";
				if (i == 0) cout << " ] "; else cout << " ][ ";
			}
		}
		cout << endl;
		cout << "Nr.:          [ 6 ][ 5 ][ 4 ][ 3 ][ 2 ][ 1 ]" << endl;
	}

	sc_uint<8> toBitFlag(int value) {

		switch (value) {
		case 0:
		case 1:
		case 2:
			return value;
		case 3:
			return 0x4;
		case 4:
			return 0x8;
		case 5:
			return 0x10;
		case 6:
			return 0x20;
		case 7:
			return 0x40;
		case 8: 
			return 0x80;
		default:
			return 0;
		}

	}

	void postConstruct() {
		for (int i = 0; i < 6; i++)
		hexes[i].write((sc_uint<8>) ' ');
	}

	SC_CTOR(cpu1) {
		SC_THREAD(task1); // MENU + ERROR
		SC_THREAD(task2); // Skrzy¿owanie #1
		//SC_THREAD(task3); // Skrzy¿owanie #2
		SC_THREAD(task3); // Skrzy¿owanie #3
	}
};