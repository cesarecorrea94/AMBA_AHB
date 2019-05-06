
#include <stdint.h>
#include <stdio.h>
#include "master.h"
#include "slave.h"
#include "decoder.h"
#include "mux.h"

bool Module::current_bus = 0;

int main() {
    Master<uint32_t> master;
    Slave<uint32_t> slaves[] {
        Slave<uint32_t>(0x10000, 0x10000),
        Slave<uint32_t>(0x20000, 0x10000),
        Slave<uint32_t>(0x30000, 0x10000),
    };
    Mux<uint32_t, 3> mux;
    Decoder<uint32_t, 3> decoder;

    master.setMux(&mux);
    mux.input(&decoder);
    //mux.input(slaves);
    decoder.master(&master);
    for(unsigned i = 0; i < 3; ++i) {
        slaves[i].input(&master);
        slaves[i].input(&mux);
        slaves[i].input(&decoder);
    }
    slaves[0].posEdgeClock();
    printf("Bye Bye World\n");
    return 0;
}