
#ifndef __MASTER_H__
#define __MASTER_H__

template<typename BUS_size>
class Master;
#include <stdint.h>
#include "module.h"
#include "Signals/master.h"
#include "Signals/slave_N_mux.h"
#include <queue>
#include <cstdio>

template<typename BUS_size=uint32_t>
class Master
:   public Module,
    public Master2Slave<BUS_size>,
    public Master2Decoder<BUS_size>
{
    using _Mux2Master = Mux2Master<BUS_size>;
    using _MasterSignals = MasterSignals<BUS_size>;
    using _MasterHADDR = MasterHADDR<BUS_size>;

    std::queue<MasterSignals<BUS_size>> transacoes;
private:
    _Mux2Master * _input;
    _MasterSignals _output[2]; // Old and Current Signals
    _MasterSignals & output() { return this->_output[!Module::current_bus]; }
    void output(_MasterSignals out)  { this->_output[ Module::current_bus] = out; }
public:    
    Master();
    ~Master() {}
    virtual void posEdgeClock() override;

    _MasterSignals & getMasterSignals() { return this->output(); }
    _MasterHADDR & getMasterHADDR() { return this->output(); }

    void input(_Mux2Master * mux) { this->_input = mux; }
};

template<typename BUS_size>
void Master<BUS_size>::posEdgeClock()  {
    if(this->_input->getMuxSignals().HREADY == PENDING) {
        this->output(this->output());
        return;
    } else if(this->output().HTRANS != IDLE &&
                this->output().HWRITE == READ) {
        printf("Master read %x \n",
            this->_input->getMuxSignals().HRDATA);
    }
    if(! this->transacoes.empty()) {
        this->output(this->transacoes.front());
        this->transacoes.pop();
    }
    else {
        _MasterSignals idle = this->output();
        idle.HTRANS = IDLE;
        this->output(idle);
    }
}

template<typename BUS_size>
Master<BUS_size>::Master() {
    _MasterSignals item;
    item.HTRANS = NONSEQ;
    item.HADDR = 0x10000;
    item.HWRITE = WRITE;
    item.HWDATA = 0xCAFE1111;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x30000;
    item.HWRITE = WRITE;
    item.HWDATA = 0xCAFE3333;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x30000;
    item.HWRITE = READ;
    item.HWDATA = 0xCAFE0123;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x10000;
    item.HWRITE = WRITE;
    item.HWDATA = 0xCAFE1111;
    transacoes.push(item);
}

#endif
