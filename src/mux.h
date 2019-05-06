
#ifndef __MUX_H__
#define __MUX_H__

template<typename BUS_size, unsigned nSlaves>
class Mux;
#include "module.h"
#include "Signals/slave_N_mux.h"
#include "slave.h"
#include "decoder.h"

template<typename BUS_size=uint32_t, unsigned nSlaves=3>
class Mux : public Module,
            public Mux2Master<BUS_size>,
            public Mux2Slave
{
    using _MuxSignals = SlaveNMuxSignals<BUS_size>;
    using _Decoder2Mux = Decoder2SlaveNMux<BUS_size>;
    using _Slave = Slave<BUS_size>;

private:
    struct {
        _Slave * slave[nSlaves];
        _Decoder2Mux * decoder;
    } _input;

    _MuxSignals _output[2]; // Old and Current Signals
    
    _MuxSignals & output() { return this->_output[0]; }
    void output(_MuxSignals out) { this->_output[0] = out; }
public:
    void input(_Decoder2Mux * dec) { this->_input.decoder = dec; }
    void input(_Slave slv[nSlaves]) {
        for(unsigned i = 0; i < nSlaves; ++i) this->_input.slave[i] = &slv[i];
    }

    Mux() {}
    ~Mux() {}
    virtual void posEdgeClock() override;

    _MuxSignals & getMuxSignals() { return this->output(); }
    TransferStatus & getTransferStatus() { return this->output().HREADY; }
    
};

#include <cassert>

template<typename BUS_size, unsigned nSlaves>
void Mux<BUS_size, nSlaves>::posEdgeClock() {
    unsigned HSELx;
    for(HSELx = 0; HSELx < nSlaves; ++HSELx)
        if(this->_input.decoder->isSelected(this->_input.slave[HSELx]))
            break;
    assert(HSELx < nSlaves);
    this->output(this->_input.slave[HSELx]->getSlaveSignals());
}

#endif
