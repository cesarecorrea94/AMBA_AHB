
#ifndef __DECODER_H__
#define __DECODER_H__

template<typename BUS_size, unsigned nSlaves>
class Decoder;
template<typename BUS_size>
struct Decoder2SlaveNMux;
#include "module.h"
#include "Signals/master.h"
#include "slave.h"

template<typename BUS_size=uint32_t>
struct Decoder2SlaveNMux {
    using _Slave = Slave<BUS_size>;
    virtual bool isSelected(_Slave * slave) = 0;
};

template<typename BUS_size=uint32_t, unsigned nSlaves=3>
class Decoder : public Module, public Decoder2SlaveNMux<BUS_size>
{
    using _Master2Decoder = Master2Decoder<BUS_size>;
    using _Slave = Slave<BUS_size>;
private:
    _Slave * _slave[nSlaves];
    _Master2Decoder * _input;
    
public:
    bool isSelected(_Slave * slave) {
        return (slave->address() - this->_input->getMasterHADDR().HADDR) < slave->size();
    }

    Decoder() {}
    ~Decoder() {}
    virtual void posEdgeClock() override;

    void master(_Master2Decoder * mstr) { _input = mstr; }
};

template<typename BUS_size, unsigned nSlaves>
void Decoder<BUS_size, nSlaves>::posEdgeClock() {
    
}

#endif
