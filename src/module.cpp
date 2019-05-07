
#include <stdint.h>
#include <stdio.h>
#include "master.h"
#include "slave.h"
#include "decoder.h"
#include "mux.h"

int main() {
    // Ordem de criação sensível, ordena a execução dos posEdgeClock
    // O Decoder tem que definir o HSELx, antes da execução dos slaves
    // O Mux tem que repassar a saída, após a execução dos slaves
    Master<uint32_t> master;
    Decoder<uint32_t, 3> decoder;
    Slave<uint32_t> slaves[] {
        Slave<uint32_t>(0x10000, 0x10000),
        Slave<uint32_t>(0x20000, 0x10000),
        Slave<uint32_t>(0x30000, 0x10000),
    };
    Mux<uint32_t, 3> mux;

    master.input(&mux);
    mux.input(&decoder);
    mux.input(slaves);
    decoder.input(&master);
    decoder.setSlaves(slaves);
    for(unsigned i = 0; i < 3; ++i) {
        slaves[i].input(&master);
        slaves[i].input(&mux);
        slaves[i].input(&decoder);
    }
    Module::simulate();

    printf("Bye Bye World\n");
    return 0;
}
