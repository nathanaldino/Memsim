#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<exception>
#include<fstream>
#include<cstring>
#include<deque>

using namespace std;


//struct for single memory access in  page table
struct memaccess {
    unsigned addr;
    char rw;
};

//page replacement algorithms
void fifo(char* tracefile, int nframes, char* type);
void lru(char* tracefile, int nframes, char* type);
void segmentedfifo(char* tracefile, int nframes, int percent, char* type);

//helper functions

int main(int argc, char* argv[]) {
    try {
        if(argc == 5 || argc == 6) {
            if(strcmp(argv[3],"fifo")==0) {
                if(strcmp(argv[4],"debug")==0 || strcmp(argv[4],"quiet")==0) {
                    int nframes = atoi(argv[2]);
                    fifo(argv[1], nframes, argv[4]);
                }
                else
                    throw;
            }
            else if(strcmp(argv[3],"lru")==0) {
                if(strcmp(argv[4],"debug")==0 || strcmp(argv[4],"quiet")==0) {
                    int nframes = atoi(argv[2]);
                    lru(argv[1], nframes, argv[4]);
                }
                else
                    throw;
            }
            else if(strcmp(argv[3],"vms")==0) {
                int percent = atoi(argv[4]);
                if( percent <= 100 && percent >= 1) {
                    if(strcmp(argv[4],"debug")==0 || strcmp(argv[4],"quiet")==0) {
                        int nframes = atoi(argv[2]);
                        segmentedfifo(argv[1], nframes, percent, argv[5]);
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

void fifo(char* tracefile, int nframes, char* type) {
    
    deque<memaccess> fifo;
    int events = 0;
    int reads = 0;
    int writes = 0;
    bool debug = (strcmp(type,"debug") == 0);

    //open file
    FILE* file;
    file = fopen(tracefile,"r");
    if(file == NULL) {
        cout << "File not found!\n";
    }
    
    memaccess temp;
    //read trace
    while(fscanf(file,"%x %c",&temp.addr,&temp.rw) !=EOF) {
        temp.addr /= 4096;
        events++;
        
        //if debug mode print everytime something is read in
        if(debug) {
            cout << "Memory access #" << events << " is " << temp.addr << temp.rw << ". " << endl;
        }

        bool go_next = false;
        //search and update if new memory access exists in FIFO
        for(int i = 0; i<fifo.size(); i++) {
            //memory access already exists in page table
            if(fifo[i].addr == temp.addr) {
                go_next = true;
                //if same memory access now needs to do a write operation instead of read
                if(fifo[i].rw == 'R' && temp.rw == 'W') {
                    fifo[i].rw = 'W';
                }
                break;
            }
        }

        //memory access alredy in page table next read
        if(go_next)
            continue;
        else {
            //if page table has space available
            if(fifo.size() < nframes) {
                fifo.push_back(temp);
                reads++;
            }
            //if page table full
            else {
                //if popped memaccess is dirty
                if(fifo.front().rw == 'W') {
                    writes++;
                } 
                fifo.pop_front();
                fifo.push_back(temp);
                reads++;
            }
        }

    }
    
    fclose(file);
    cout << "total memory frames: " << nframes << endl;
    cout << "events in trace: " << events << endl;
    cout << "total disk reads: " << reads << endl;
    cout << "total disk writes: " << writes << endl;
}

void lru(char* tracefile, int nframes, char* type) {

}

void segmentedfifo(char* tracefile, int nframes, int percent, char* type){
    
    deque<memaccess> fifo;
    //insert data structure for LRU

    int events = 0;
    int reads = 0;
    int writes = 0;
    bool debug = (strcmp(type,"debug") == 0);
    int s_nframes = (nframes*percent)/100; //secondary size for LRU
    int p_nframes = nframes - s_nframes; //primary size for FIFO

    //open file
    FILE* file;
    file = fopen(tracefile,"r");
    if(file == NULL) {
        cout << "File not found!\n";
    }
    
    memaccess temp;
    //read trace
    while(fscanf(file,"%x %c",&temp.addr,&temp.rw) !=EOF) {
        temp.addr /= 4096;
        events++;

        //if debug mode print everytime something is read in
        if(debug) {
            cout << "Memory access #" << events << " is " << temp.addr << temp.rw << ". " << endl;
        }

        //VMS is FIFO
        if(s_nframes == 0) {
            bool go_next = false;
            //search and update if new memory access exists in FIFO
            for(int i = 0; i<fifo.size(); i++) {
                //memory access already exists in page table
                if(fifo[i].addr == temp.addr) {
                    go_next = true;
                    //if same memory access now needs to do a write operation instead of read
                    if(fifo[i].rw == 'R' && temp.rw == 'W') {
                        fifo[i].rw = 'W';
                    }
                    break;
                }
            }
            //search and update if memory access exists in LRU
            //insert LRU check

            //memory access alredy in page table next read
            if(go_next)
                continue;
            else {
                //if page table has space available
                if(fifo.size() < p_nframes) {
                    fifo.push_back(temp);
                    reads++;
                }
                //if page table full
                else {
                    //if popped memaccess is dirty
                    if(fifo.front().rw == 'W') {
                        writes++;
                    } 
                    fifo.pop_front();
                    fifo.push_back(temp);
                    reads++;
                }
            }
        }
        //VMS is LRU
        else if(p_nframes == 0) {
            //copy and paste LRU code but using the s_nframes variable as the size of LRU data structure
        }
        //default VMS
        else {
            //check if memory access is in FIFO
            //if in FIFO, update W bit if needed

            //check if memory access is in LRU
            //if in LRU, update W bit if needed then push into FIFO and push bottom in FIFO into LRU

            //if FIFO empty, push into FIFO. Update read
            //if FIFO full, push into LRU. Update read

            //if FIFO full and LRU full, push into FIFO, pop bottom and evict into LRU, eject oldest LRU (increment write if W). 
        }

    }

    fclose(file);
    cout << "total memory frames: " << nframes << endl;
    cout << "events in trace: " << events << endl;
    cout << "total disk reads: " << reads << endl;
    cout << "total disk writes: " << writes << endl;
}