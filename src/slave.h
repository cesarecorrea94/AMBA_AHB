
#ifndef __SLAVE_H__
#define __SLAVE_H__

template<typename BUS_size>
class Slave;
#include "module.h"
#include "Signals/master.h"
#include "Signals/slave_N_mux.h"
#include "decoder.h"
#include <cstdlib>
#include <cstdio>

template<typename BUS_size=uint32_t>
class Slave : public Module
{
    using _SlaveSignals = SlaveNMuxSignals<BUS_size>;
    using _Decoder2Slave = Decoder2SlaveNMux<BUS_size>;
    using _Master2Slave = Master2Slave<BUS_size>;
    using _Mux2Slave = Mux2Slave;
    
private:
    struct {
        bool HSELx = false;
        unsigned wait;
    } _state;
    BUS_size _address;
    unsigned _size;
    struct Input {
        _Master2Slave * master = 0;
        _Decoder2Slave * HSELx = 0;
        _Mux2Slave * HREADYIN = 0;
    } _input;
    _SlaveSignals _output[2]; // Old and Current Signals

    _SlaveSignals & output() { return this->_output[0]; }
    void output(_SlaveSignals out)    { this->_output[0] = out; }
    void output(TransferResponse out) { this->_output[0].HRESP = out; }
    void output(TransferStatus out)   { this->_output[0].HREADY = out; }
    void output(BUS_size out)         { this->_output[0].HRDATA = out; }

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

template<typename BUS_size>
void Slave<BUS_size>::posEdgeClock() {
    if(!_state.HSELx) {
        if(this->_input.HREADYIN->getTransferStatus() == PENDING
        || this->_input.master->getMasterSignals().HTRANS == IDLE
        || ! this->_input.HSELx->isSelected(this)) {
            this->output(OKAY);
            this->output(DONE);
            return;
        }
        _state.HSELx = true;
        _state.wait = rand() % 5;
    }
    if(_state.wait) {
        printf("Wait %d\n", _state.wait);
        _state.wait--;
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
    _state.HSELx = false;
}

#endif
