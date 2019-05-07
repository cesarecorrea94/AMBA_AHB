
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
    bool next_transaction = false;
    _Mux2Master * _input;
    _MasterSignals _output[2]; // Old and Current Signals
    _MasterSignals & output() { return this->_output[!next_transaction]; }
    _MasterSignals & next()   { return this->_output[ next_transaction]; }
    void next(_MasterSignals out)    { this->_output[ next_transaction] = out; }
    void updateNext() {
        if(! this->transacoes.empty()) {// Se há transações
            this->next(this->transacoes.front());// Substitui next pela próxima transação
            this->transacoes.pop();
        } else { // Se acabaram as transações
            _MasterSignals trans = this->next();
            trans.HTRANS = IDLE;// Permanece IDLE nos próximos clocks
            this->next(trans);
            Module::stopSimulation = this->output().HTRANS == IDLE;
        }
    }
public:    
    Master();
    ~Master() {}
    virtual void posEdgeClock() override;

    _MasterSignals & getMasterSignals() { return this->output(); }
    _MasterHADDR & getMasterHADDR()     { return this->output(); }

    void input(_Mux2Master * mux) { this->_input = mux; }
};

template<typename BUS_size>
void Master<BUS_size>::posEdgeClock()  {
    this->_input->getMuxSignals().print();
    if( this->output().HTRANS == IDLE
    ||  this->output().HTRANS == BUSY) {// Se atual é IDLE ou BUSY
        this->next_transaction = !this->next_transaction; // Move para a próxima transação
        this->updateNext();
    } else // atual é NONSEQ ou SEQ
    if( this->_input->getMuxSignals().HREADY == DONE) {// Se atual terminou
        if( this->output().HWRITE == READ) {// Se é leitura,
            printf("Master reads %x from %x\n",
                this->_input->getMuxSignals().HRDATA,
                this->output().HADDR);
        }
        this->next_transaction = !this->next_transaction; // Move para a próxima transação
        this->updateNext();
    } else // atual é NONSEQ ou SEQ, e não terminou
    if( this->next().HTRANS == IDLE
    ||  this->next().HTRANS == BUSY) {// Se próxima é IDLE ou BUSY
        this->updateNext();
    } // else: next já está definido (NONSEQ ou SEQ), e não pode mudar (exceção: Erro na transação atual)
    //this->output().print();
    this->next().print();
}

template<typename BUS_size>
Master<BUS_size>::Master() {
    _MasterSignals item;
    item.HTRANS = NONSEQ;
    item.HADDR = 0x10000;
    item.HWRITE = READ;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x10000;
    item.HWRITE = WRITE;
    item.HWDATA = 0xCAFEBABE;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x2FF00;
    item.HWRITE = WRITE;
    item.HWDATA = 0xDEADC0DE;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x3FF00;
    item.HWRITE = READ;
    transacoes.push(item);

    item = _MasterSignals();
    item.HTRANS = NONSEQ;
    item.HADDR = 0x20000;
    item.HWRITE = READ;
    transacoes.push(item);
}

#endif
