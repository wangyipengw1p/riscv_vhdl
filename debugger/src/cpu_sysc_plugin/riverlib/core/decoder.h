/**
 * @file
 * @copyright  Copyright 2016 GNSS Sensor Ltd. All right reserved.
 * @author     Sergey Khabarov - sergeykhbr@gmail.com
 * @brief      CPU Instruction Decoder stage.
 */

#ifndef __DEBUGGER_RIVERLIB_DECODER_H__
#define __DEBUGGER_RIVERLIB_DECODER_H__

#include <systemc.h>
#include "../river_cfg.h"

namespace debugger {

const uint8_t OPCODE_LB     = 0x00; // 00000: LB, LH, LW, LD, LBU, LHU, LWU
const uint8_t OPCODE_FENCE  = 0x03; // 00011: FENCE, FENCE_I
const uint8_t OPCODE_ADDI   = 0x04; // 00100: ADDI, ANDI, ORI, SLLI, SLTI, SLTIU, SRAI, SRLI, XORI
const uint8_t OPCODE_AUIPC  = 0x05; // 00101: AUIPC
const uint8_t OPCODE_ADDIW  = 0x06; // 00110: ADDIW, SLLIW, SRAIW, SRLIW
const uint8_t OPCODE_SB     = 0x08; // 01000: SB, SH, SW, SD
const uint8_t OPCODE_ADD    = 0x0C; // 01100: ADD, AND, OR, SLT, SLTU, SLL, SRA, SRL, SUB, XOR, DIV, DIVU, MUL, REM, REMU
const uint8_t OPCODE_LUI    = 0x0D; // 01101: LUI
const uint8_t OPCODE_ADDW   = 0x0E; // 01110: ADDW, SLLW, SRAW, SRLW, SUBW, DIVW, DIVUW, MULW, REMW, REMUW
const uint8_t OPCODE_BEQ    = 0x18; // 11000: BEQ, BNE, BLT, BGE, BLTU, BGEU
const uint8_t OPCODE_JALR   = 0x19; // 11001: JALR
const uint8_t OPCODE_JAL    = 0x1B; // 11011: JAL
const uint8_t OPCODE_CSRR   = 0x1C; // 11100: CSRRC, CSRRCI, CSRRS, CSRRSI, CSRRW, CSRRWI, URET, SRET, HRET, MRET
// Compressed instruction set
const uint8_t OPCODE_C_ADDI4SPN = 0x00;
const uint8_t OPCODE_C_NOP_ADDI = 0x01;
const uint8_t OPCODE_C_SLLI     = 0x02;
const uint8_t OPCODE_C_JAL_ADDIW = 0x05;
const uint8_t OPCODE_C_LW       = 0x08;
const uint8_t OPCODE_C_LI       = 0x09;
const uint8_t OPCODE_C_LWSP     = 0x0A;
const uint8_t OPCODE_C_LD       = 0x0C;
const uint8_t OPCODE_C_ADDI16SP_LUI = 0xD;
const uint8_t OPCODE_C_LDSP     = 0x0E;
const uint8_t OPCODE_C_MATH     = 0x11;
const uint8_t OPCODE_C_JR_MV_EBREAK_JALR_ADD = 0x12;
const uint8_t OPCODE_C_J        = 0x15;
const uint8_t OPCODE_C_SW       = 0x18;
const uint8_t OPCODE_C_BEQZ     = 0x19;
const uint8_t OPCODE_C_SWSP     = 0x1A;
const uint8_t OPCODE_C_SD       = 0x1C;
const uint8_t OPCODE_C_BNEZ     = 0x1D;
const uint8_t OPCODE_C_SDSP     = 0x1E;

SC_MODULE(InstrDecoder) {
    sc_in<bool> i_clk;
    sc_in<bool> i_nrst;                         // Reset active low
    sc_in<bool> i_any_hold;                     // Hold pipeline by any reason
    sc_in<bool> i_f_valid;                      // Fetch input valid
    sc_in<sc_uint<BUS_ADDR_WIDTH>> i_f_pc;      // Fetched pc
    sc_in<sc_uint<32>> i_f_instr;               // Fetched instruction value

    sc_out<bool> o_valid;                       // Current output values are valid
    sc_out<sc_uint<BUS_ADDR_WIDTH>> o_pc;       // Current instruction pointer value
    sc_out<sc_uint<32>> o_instr;                // Current instruction value
    sc_out<bool> o_memop_store;                 // Store to memory operation
    sc_out<bool> o_memop_load;                  // Load from memoru operation
    sc_out<bool> o_memop_sign_ext;              // Load memory value with sign extending
    sc_out<sc_uint<2>> o_memop_size;            // Memory transaction size
    sc_out<bool> o_rv32;                        // 32-bits instruction
    sc_out<bool> o_compressed;                  // C-type instruction
    sc_out<bool> o_unsigned_op;                 // Unsigned operands
    sc_out<sc_bv<ISA_Total>> o_isa_type;        // Instruction format accordingly with ISA
    sc_out<sc_bv<Instr_Total>> o_instr_vec;     // One bit per decoded instruction bus
    sc_out<bool> o_exception;

    void comb();
    void registers();

    SC_HAS_PROCESS(InstrDecoder);

    InstrDecoder(sc_module_name name_);

    void generateVCD(sc_trace_file *i_vcd, sc_trace_file *o_vcd);

private:
    struct RegistersType {
        sc_signal<bool> valid;
        sc_signal<sc_uint<BUS_ADDR_WIDTH>> pc;
        sc_bv<ISA_Total> isa_type;
        sc_bv<Instr_Total> instr_vec;
        sc_signal<sc_uint<32>> instr;
        sc_signal<bool> memop_store;
        sc_signal<bool> memop_load;
        sc_signal<bool> memop_sign_ext;
        sc_signal<sc_uint<2>> memop_size;
        sc_signal<bool> unsigned_op;
        sc_signal<bool> rv32;
        sc_signal<bool> compressed;

        sc_signal<bool> instr_unimplemented;
    } v, r;
};


}  // namespace debugger

#endif  // __DEBUGGER_RIVERLIB_DECODER_H__
