/*
 *  Copyright 2018 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __DEBUGGER_SRC_CPU_ARM_PLUGIN_INSTRUCTIONS_H__
#define __DEBUGGER_SRC_CPU_ARM_PLUGIN_INSTRUCTIONS_H__

#include <inttypes.h>
#include "generic/cpu_generic.h"

namespace debugger {

class CpuCortex_Functional;

bool check_cond(CpuCortex_Functional *icpu, uint32_t cond);

class ArmInstruction : public GenericInstruction {
 public:
    ArmInstruction(CpuCortex_Functional *icpu, const char *name,
                    const char *bits);

    // IInstruction interface:
    virtual const char *name() { return name_.to_string(); }
    virtual int exec(Reg64Type *payload);

    /** conditions return true */
    virtual int exec_checked(Reg64Type *payload) = 0;

    virtual bool parse(uint32_t *payload) {
        return ((payload[0] & mask_) == opcode_);
    }

    virtual uint32_t hash() {
        return (opcode_ >> 24) & 0xF;
    }

    virtual uint32_t shift12(DataProcessingType::reg_bits_type instr,
                             uint32_t reg, uint64_t Rs);

    virtual uint32_t imm12(DataProcessingType::imm_bits_type instr);

 protected:
    virtual IFace *getInterface(const char *name);

 protected:
    AttributeType name_;
    CpuCortex_Functional *icpu_;
    Axi4TransactionType trans_;
    uint32_t mask_;
    uint32_t opcode_;
    uint64_t *R;
};

class ThumbInstruction : public GenericInstruction {
 public:
    ThumbInstruction(CpuCortex_Functional *icpu, const char *name,
                    const char *bits);

    // IInstruction interface:
    virtual const char *name() { return name_.to_string(); }
    virtual int exec(Reg64Type *payload) = 0;

 protected:
    virtual IFace *getInterface(const char *name);

 protected:
    AttributeType name_;
    CpuCortex_Functional *icpu_;
    Axi4TransactionType trans_;
    uint16_t mask_;
    uint16_t opcode_;
    uint64_t *R;
};


/** opcodes:
    0000 = AND - Rd:= Op1 AND Op2
    0001 = EOR - Rd:= Op1 EOR Op2
    0010 = SUB - Rd:= Op1 - Op2
    0011 = RSB - Rd:= Op2 - Op1
    0100 = ADD - Rd:= Op1 + Op2
    0101 = ADC - Rd:= Op1 + Op2 + C
    0110 = SBC - Rd:= Op1 - Op2 + C
    0111 = RSC - Rd:= Op2 - Op1 + C
    1000 = TST - set condition codes on Op1 AND Op2
    1001 = TEQ - set condition codes on Op1 EOR Op2
    1010 = CMP - set condition codes on Op1 - Op2
    1011 = CMN - set condition codes on Op1 + Op2
    1100 = ORR - Rd:= Op1 OR Op2
    1101 = MOV - Rd:= Op2
    1110 = BIC - Rd:= Op1 AND NOT Op2
    1111 = MVN - Rd:= NOT Op2
*/
class ArmDataProcessingInstruction : public ArmInstruction {
 public:
    ArmDataProcessingInstruction(CpuCortex_Functional *icpu, const char *name,
        const char *bits) : ArmInstruction(icpu, name, bits) {}

    virtual int exec_checked(Reg64Type *payload);
 protected:
    enum EOperationResult {
        OP_Drop,
        OP_Write
    };

    /** Return true if need to write result into Rd register */
    virtual EOperationResult do_operation(uint32_t A, uint32_t M,
                                          uint32_t *pRes) = 0;
    virtual bool is_flags_changed(DataProcessingType u);
    virtual void set_flags(uint32_t A, uint32_t M, uint32_t Res);
};

}  // namespace debugger

#endif  // __DEBUGGER_SRC_CPU_ARM_PLUGIN_INSTRUCTIONS_H__
