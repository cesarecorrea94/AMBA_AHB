
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
    _SlaveSignals _output;

    _SlaveSignals & output() { return this->_output; }
    void output(_SlaveSignals out)    { this->_output = out; }
    void output(TransferResponse out) { this->_output.HRESP = out; }
    void output(TransferStatus out)   { this->_output.HREADY = out; }
    void output(BUS_size out)         { this->_output.HRDATA = out; }

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
    //this->_input.master->getMasterSignals().print();
    if(!this->_input.HSELx->HSELx(this) // I'm not selected
    ||  this->_input.master->getMasterSignals().HTRANS == IDLE
    ||  this->_input.master->getMasterSignals().HTRANS == BUSY) {// It's an IDLE/BUSY transaction
        this->output(OKAY);
        this->output(DONE);
        return;
    } else // I'm selected, and it's a valid transaction
    if(this->_input.HREADYIN->getTransferStatus() == DONE) {// It's first cycle
        _state.wait = rand() % 3;
        _state.wait *= _state.wait;
    }// else: I'm handling this transaction already
    if(_state.wait) {
        _state.wait--;
        this->output(OKAY);
        this->output(PENDING);
        return;
    }
    switch (this->_input.master->getMasterSignals().HWRITE) {
    case WRITE:
        printf("%x was written at %x\n",
            this->_input.master->getMasterSignals().HWDATA,
            this->_input.master->getMasterSignals().HADDR);
        break;
    case READ:
        this->output(this->_input.master->getMasterSignals().HADDR);
        printf("%x was read from %x\n",
            this->output().HRDATA,
            this->_input.master->getMasterSignals().HADDR);
        break;
    default: throw false;
    }
    this->output(OKAY);
    this->output(DONE);
}

#endif
