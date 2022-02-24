#include<stdio.h>
#include<iostream>
#include<exception>

using namespace std;

//page replacement algorithms
void fifo(int nframes, char* type);
void lru(int nframes, char* type);
void segmentedfifo(int nframes, int percent, char* type);

int main(int argc, char* argv[]) {
    try {
        if(argc == 5 || argc == 6) {
            if(argv[3] == "fifo") {
                if(argv[4] == "debug" || argv[4] == "quiet") {
                    fifo((int)argv[2],argv[4]);
                }
                else
                    throw;
            }
            else if(argv[3] == "lru") {
                if(argv[4] == "debug" || argv[4] == "quiet") {
                    lru((int)argv[2],argv[4]);
                }
                else
                    throw;
            }
            else if(argv[3] == "vms") {
                if( (int)argv[4] <= 100 && (int)argv[4] >= 1) {
                    if(argv[4] == "debug" || argv[4] == "quiet") {
                        segmentedfifo((int)argv[2],(int)argv[4],argv[5]);
                    }
                    else
                        throw;
                }
                else
                    throw;
            }
            else
                throw;
        }
        else {
            throw;
        }
    } catch (exception& e) {
        cout << "Not enough/correct arguments. Please format as: memsim <tracefile> <nframes> <fifo|lru|vms> <p(only if vms)> <debug|quiet> or " << endl << endl;
    }

    return 1;
}

void fifo(int nframes, char* type) {

}

void lru(int nframes, char* type) {

}

void segmentedfifo(int nframes, int percent, char* type){

}