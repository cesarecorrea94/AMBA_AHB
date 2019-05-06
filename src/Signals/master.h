
#ifndef __SIGNALS_MASTER_H__
#define __SIGNALS_MASTER_H__

#include <stdint.h>

typedef enum {
    WRITE   = 0b1,
    READ    = 0b0
} TransferDirection;

typedef enum {
    IDLE    = 0b00,
    BUSY    = 0b01,
    NONSEQ  = 0b10,
    SEQ     = 0b11
} TransferType;

typedef enum {
    BYTE        = 0b000,
    HALFWORD    = 0b001,
    WORD        = 0b010,
    DOUBLEWORD  = 0b011,
    _4WORD      = 0b100,
    _8WORD      = 0b101,
    _512bits    = 0b110,
    _1024bits   = 0b111
} TransferSize;

typedef enum {
    SINGLE  = 0b000,
    INCR    = 0b001,
    WRAP4   = 0b010,
    INCR4   = 0b011,
    WRAP8   = 0b100,
    INCR8   = 0b101,
    WRAP16  = 0b110,
    INCR16  = 0b111
} BurstOperation;

typedef enum {
    LOCKED      = 0b1,
    UNLOCKED    = 0b0
} LockedTransfer;

template<typename BUS_size=uint32_t>
struct MasterHADDR
{
    BUS_size HADDR;
};

template<typename BUS_size=uint32_t>
struct MasterSignals : public MasterHADDR
{
    // Control
    TransferDirection HWRITE;
    TransferSize HSIZE;
    BurstOperation HBURST;
    /* HPROT; */
    TransferType HTRANS;
    LockedTransfer HMASTLOCK;
    // Data
    BUS_size HWDATA;
};

#endif
