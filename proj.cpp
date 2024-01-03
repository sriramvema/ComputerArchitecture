#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bitset>
using namespace std;


vector<string> MIPSbits;
string MEMORY[32];
string Registers[32];
int beqCHECK = 0;
int jmp = 0;

void load() {
    ifstream file("IMemory.txt");
    string line;
    int count = 0;
    while(getline(file, line)) {
        MEMORY[count] = line;
        count++;
    }   
}

void lw(string rs, string rt, int offset) {
    int rsIndex = stoi(rs, nullptr, 2); // Convert binary string to int for rs register index
    int rtIndex = stoi(rt, nullptr, 2); // Convert binary string to int for rt register index

    // Calculate the memory address
    
    int address =  stoi(Registers[rsIndex], nullptr, 2) + offset;

    if (address < 0 || address >= 32) {
        cout << "Memory access error: Address out of range" << endl;
        return;
    }

    string word = MEMORY[address]; // Read the word from memory at the calculated address

    
    Registers[rtIndex] = word; // Store the word in the target register

   
}

void sw(string rs, string rt, int offset) {
    int rsIndex = stoi(rs, nullptr, 2); // Convert binary string to int for rs register index
    int rtIndex = stoi(rt, nullptr, 2); // Convert binary string to int for rt register index
    // Calculate the memory address
    int address = stoi(Registers[rsIndex], nullptr, 2) + offset;
    
    string value = Registers[rtIndex]; // Get the value from the rt register
    // Store the value into memory
    MEMORY[address] = value; // Write the word to the memory at the calculated address
}

void ADDU(string rs, string rt, string rd) {
    int rsIndex = stoi(rs, nullptr, 2); // Convert binary string to int for rs register index
    int rtIndex = stoi(rt, nullptr, 2); // Convert binary string to int for rt register index
    int rdIndex = stoi(rd, nullptr, 2); // Convert binary string to int for rd register index
    
    string rsValue = Registers[rsIndex]; // Get the value from the rs register
    string rtValue = Registers[rtIndex]; // Get the value from the rt register

    int result = stoi(rsValue, nullptr, 2) + stoi(rtValue, nullptr, 2); // Perform the addition
    string binaryResult = bitset<32>(result).to_string();
    // Store the result in the rd register
    Registers[rdIndex] = binaryResult; 
}

void ADDIU(string rs, string rt, int immediate) {
    int rsIndex = stoi(rs, nullptr, 2); // Convert binary string to int for rs register index
    int rtIndex = stoi(rt, nullptr, 2); // Convert binary string to int for rt register index

   
    string rsValue = Registers[rsIndex]; // Get the value from the rs register

    int result = stoi(rsValue, nullptr, 2) + immediate; // Perform the addition with the immediate value
    string binaryResult = bitset<32>(result).to_string();

    
    Registers[rtIndex] = binaryResult; // Store the result in the rt register
}

void SUBU(string rs, string rt, string rd) {
    int rsIndex = stoi(rs, nullptr, 2); // Convert binary string to int for rs register index
    int rtIndex = stoi(rt, nullptr, 2); // Convert binary string to int for rt register index
    int rdIndex = stoi(rd, nullptr, 2); // Convert binary string to int for rd register index
    cout << rsIndex << endl;
    cout << rtIndex << endl;
    string rsValue = Registers[rsIndex]; // Get the value from the rs register
    string rtValue = Registers[rtIndex]; // Get the value from the rt register

    int result = stoi(rsValue, nullptr, 2) - stoi(rtValue, nullptr, 2); // Perform the subtraction
    string binaryResult = bitset<32>(result).to_string();

    Registers[rdIndex] = binaryResult; // Store the result in the rd register
}

void BEQ(string rs, string rt, string offset) {
    int rsIndex = stoi(rs, nullptr, 2);
    int rtIndex = stoi(rt, nullptr, 2);
    cout << Registers[rsIndex] << endl;
    cout << Registers[rtIndex] << endl;
    if (Registers[rsIndex] == Registers[rtIndex]) {
        int skip = stoi(offset, nullptr, 2);
        beqCHECK += skip;
    }
}

void NOR(string rs, string rt, string rd) {
    int rsIndex = stoi(rs, nullptr, 2);
    int rtIndex = stoi(rt, nullptr, 2);
    int rdIndex = stoi(rs, nullptr, 2);
    string result = "";
    string final = "";
    if (Registers[rsIndex].length() == Registers[rtIndex].length()) {
        for (size_t i = 0; i < Registers[rtIndex].length(); i++) {
            if (Registers[rsIndex][i] == '0' && Registers[rtIndex][i] == '0') {
                result += '0';
            } else {
                result += '1';
            }
        }
    }
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] == '0') {
            final += '1';
        } else {
            final += '0';
        }
    }
    Registers[rdIndex] = final;
}

void AND(string rs, string rt, string rd) {
    int rsIndex = stoi(rs, nullptr, 2);
    int rtIndex = stoi(rt, nullptr, 2);
    int rdIndex = stoi(rs, nullptr, 2);
    string result = "";
    if (Registers[rsIndex].length() == Registers[rtIndex].length()) {
        for (size_t i = 0; i < Registers[rtIndex].length(); i++) {
            if (Registers[rsIndex][i] == Registers[rtIndex][i]) {
                result += '0';
            } else {
                result += '1';
            }
        }
    }
    cout << result << endl;
    Registers[rdIndex] = result;
}

void OR(string rs, string rt, string rd) {
    int rsIndex = stoi(rs, nullptr, 2);
    int rtIndex = stoi(rt, nullptr, 2);
    int rdIndex = stoi(rs, nullptr, 2);
    string result = "";
    if (Registers[rsIndex].length() == Registers[rtIndex].length()) {
        for (size_t i = 0; i < Registers[rtIndex].length(); i++) {
            if (Registers[rsIndex][i] == '0' && Registers[rtIndex][i] == '0') {
                result += '0';
            } else {
                result += '1';
            }
        }
    }
    cout << result << endl;
    Registers[rdIndex] = result;
}

void format() {
    load();
    string MIPSI[7];
    string MIPSR[6];
    string MIPSJ[2];
    string opcode;
    for(const auto& str : MIPSbits) {
        if (str.length() >= 32) {
            opcode = str.substr(0, 6);
            if (opcode == "000000") {
                MIPSR[0] = str.substr(0, 6);
                MIPSR[1] = str.substr(6, 5);
                MIPSR[2] = str.substr(11, 5);
                MIPSR[3] = str.substr(16, 5);
                MIPSR[4] = str.substr(21, 5);
                MIPSR[5] = str.substr(26, 6);
                
                if (MIPSR[5] == "100001") {//ADDU
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            ADDU(MIPSR[1], MIPSR[2], MIPSR[3]);
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                    
                } else if (MIPSR[5] == "100011") {//SUBU
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            SUBU(MIPSR[1], MIPSR[2], MIPSR[3]);
                        } else {
                            jmp --;
                        }
                    } else {
                        beqCHECK--;
                    }
                } else if (MIPSR[5] == "100100") {//AND
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            AND(MIPSR[1], MIPSR[2], MIPSR[3]);
                        } else {
                            jmp --;
                        }                        
                    } else {
                        beqCHECK--;
                    }
                } else if (MIPSR[5] == "100101") {//OR
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            OR(MIPSR[1], MIPSR[2], MIPSR[3]);
                        } else {
                            jmp --;
                        }
                    } else {
                        beqCHECK--;
                    }
                } else {//NOR
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            NOR(MIPSR[1], MIPSR[2], MIPSR[3]);
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                }
            } else if (opcode == "000010" || opcode == "000011") {
                MIPSJ[0] = str.substr(0, 6);
                MIPSJ[1] = str.substr(6, 26);      
                if (jmp == 0) {
                    int jump = stoi(MIPSJ[1], nullptr, 2);
                    jump = jump/2;
                    jmp += jump;
                } else {
                    jmp --;
                }  

            } else if (opcode == "111111") {
                cout << "HALT" << endl;
            } else {
                MIPSI[0] = str.substr(0, 6);
                MIPSI[1] = str.substr(6, 5);
                MIPSI[2] = str.substr(11, 5);
                MIPSI[3] = str.substr(16, 16);
                if (opcode == "100011") {
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            lw(MIPSI[1], MIPSI[2], stoi(MIPSI[3], nullptr, 2));
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                    
                } else if (opcode == "101011") {
                    cout << "SW" << endl;
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                           sw(MIPSI[1], MIPSI[2], stoi(MIPSI[3], nullptr, 2));
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                } else if (opcode == "000100") { //BEQ
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            BEQ(MIPSI[1], MIPSI[2], MIPSI[3]);
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                } else { //ADDIU
                    if (beqCHECK == 0) {
                        if (jmp == 0) {
                            ADDIU(MIPSI[1], MIPSI[2], stoi(MIPSI[3], nullptr, 2));
                        } else {
                            jmp --;
                        }
                        
                    } else {
                        beqCHECK--;
                    }
                    
                }
                    
            }

        } 
        
    }
}

void write() {
    ofstream outFile("Registers.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < 32; ++i) {
            outFile << Registers[i] << endl;
        }
        outFile.close();
    } else {
        cout << "Error opening file." << endl;
    }
    ofstream MemFile("Memory.txt");
    if (MemFile.is_open()) {
        for (int i = 0; i < 32; ++i) {
            MemFile << MEMORY[i] << endl;
        }
        MemFile.close();
    } else {
        cout << "Error opening file." << endl;
    }

}

int main() {
    ifstream file("Instructions3.txt");
    string line, combined;
    string instructionBinary;
    for (int i = 0; i < 32; ++i) {
        Registers[i] = bitset<32>(0).to_string(); // Initialize all registers to '0'
    }
    int count = 0;
    while(getline(file, line)) {
        combined += line;
        count++;
        if (count == 4) {
            MIPSbits.push_back(combined);
            count = 0;
            combined.clear();
        } 
    }

    format();
    write();


    return 0;
}