
#ifndef __DECODER_H__
#define __DECODER_H__

template<typename BUS_size, unsigned nSlaves>
class Decoder;
template<typename BUS_size>
struct Decoder2SlaveNMux;
#include "module.h"
#include "Signals/master.h"
#include "slave.h"
#include <cstdio>

template<typename BUS_size=uint32_t>
struct Decoder2SlaveNMux {
    using _Slave = Slave<BUS_size>;
    virtual unsigned HSELx() = 0;
    virtual bool HSELx(_Slave * slave) = 0;
};

template<typename BUS_size=uint32_t, unsigned nSlaves=3>
class Decoder : public Module, public Decoder2SlaveNMux<BUS_size>
{
    using _Master2Decoder = Master2Decoder<BUS_size>;
    using _Slave = Slave<BUS_size>;
private:
    _Slave * _slave[nSlaves];
    _Master2Decoder * _input;
    unsigned _HSELx = 0;
    
    bool isSlaveArea(_Slave * slave) {
        return (this->_input->getMasterHADDR().HADDR >= slave->address()) &&
                (this->_input->getMasterHADDR().HADDR - slave->address()) < slave->size();
    }
public:
    Decoder() {}
    ~Decoder() {}
    virtual void posEdgeClock() override;

    unsigned HSELx() override { return this->_HSELx; }
    bool HSELx(_Slave * slave) override { return this->_slave[this->_HSELx] == slave; }
    void input(_Master2Decoder * mstr) { this->_input = mstr; }
    void setSlaves(_Slave slv[nSlaves]) {
        for(unsigned i = 0; i < nSlaves; ++i) this->_slave[i] = &slv[i];
    }
};

template<typename BUS_size, unsigned nSlaves>
void Decoder<BUS_size, nSlaves>::posEdgeClock() {
    for(unsigned i = 0; i < nSlaves; ++i)
        if(this->isSlaveArea(this->_slave[i])) {
            this->_HSELx = i;
            return;
        }
    throw false;
}

#endif
