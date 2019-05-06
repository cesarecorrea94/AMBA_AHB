
#ifndef __SLAVE_H__
#define __SLAVE_H__

template<typename BUS_size>
class Slave;
#include "module.h"
#include "Signals/master.h"
#include "Signals/slave_N_mux.h"
#include "decoder.h"

template<typename BUS_size=uint32_t>
class Slave : public Module
{
    using _SlaveSignals = SlaveNMuxSignals<BUS_size>;
    using _Decoder2Slave = Decoder2SlaveNMux<BUS_size>;
    using _Master2Slave = Master2Slave<BUS_size>;
    using _Mux2Slave = Mux2Slave;
    
private:
    BUS_size _address;
    unsigned _size;
    struct Input {
        _Master2Slave * master = 0;
        _Decoder2Slave * HSELx = 0;
        _Mux2Slave * HREADYIN = 0;
    } _input;
    _SlaveSignals _output[2]; // Old and Current Signals

    _SlaveSignals & output() { return this->_output[!Module::current_bus]; }
    void output(_SlaveSignals out)    { this->_output[Module::current_bus] = out; }
    void output(TransferResponse out) { this->_output[Module::current_bus].HRESP = out; }
    void output(TransferStatus out)   { this->_output[Module::current_bus].HREADY = out; }
    void output(BUS_size out)         { this->_output[Module::current_bus].HRDATA = out; }

public:
    Slave(BUS_size addr, unsigned size) : _address(addr), _size(size) {}
    ~Slave() {}
    virtual void posEdgeClock() override;

    _SlaveSignals & getSlaveSignals() { return this->output(); }

    BUS_size address()  { return this->_address; }
    unsigned size()     { return this->_size; }
    void input(_Master2Slave * mstr) { this->_input.master = mstr; }
    void input(_Decoder2Slave * dec) { this->_input.HSELx = dec; }
    void input(_Mux2Slave * mux)     { this->_input.HREADYIN = mux; }
};

#include <cstdlib>
#include <cstdio>

template<typename BUS_size>
void Slave<BUS_size>::posEdgeClock() {
    static bool HSELx = false;
    static unsigned wait;
    if(!HSELx) {
        if(this->_input.HREADYIN->getTransferStatus() == PENDING) return;
        if(this->_input.HSELx->isSelected(this)) HSELx = true;
        wait = rand() % 3;
    }
    if(wait) {
        wait--;
        this->output(OKAY);
        this->output(PENDING);
        return;
    }
    switch (this->_input.master->getMasterSignals().HWRITE) {
    case WRITE:
        printf("Device at %x read %x \n",
            this->_address,
            this->_input.master->getMasterSignals().HWDATA);
        this->output(OKAY);
        this->output(DONE);
        break;
    case READ:
        this->output(this->_address);
        this->output(OKAY);
        this->output(DONE);
        break;
    default:
        break;
    }
    HSELx = false;
}

#endif
