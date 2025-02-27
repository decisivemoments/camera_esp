# Programming ULP FSM Coprocessor Using C Macros (Legacy)


In addition to the existing binutils port for the ESP32-S3 ULP coprocessor, it is possible to generate programs for the ULP FSM coprocessor by embedding assembly-like macros into an ESP32-S3 application. Here is an example how this can be done:



```
const ulp_insn_t program[] = {
    I_MOVI(R3, 16),         // R3 <- 16
    I_LD(R0, R3, 0),        // R0 <- RTC_SLOW_MEM[R3 + 0]
    I_LD(R1, R3, 1),        // R1 <- RTC_SLOW_MEM[R3 + 1]
    I_ADDR(R2, R0, R1),     // R2 <- R0 + R1
    I_ST(R2, R3, 2),        // R2 -> RTC_SLOW_MEM[R2 + 2]
    I_HALT()
};
size_t load_addr = 0;
size_t size = sizeof(program)/sizeof(ulp_insn_t);
ulp_process_macros_and_load(load_addr, program, &size);
ulp_run(load_addr);

```


The `program` array is an array of `ulp_insn_t`, i.e., ULP coprocessor instructions. Each `I_XXX` preprocessor define translates into a single 32-bit instruction. Arguments of these preprocessor defines can be register numbers (`R0 — R3`) and literal constants. See the API reference section at the end of this guide for descriptions of instructions and arguments they take.



Note


Because some of the instruction macros expand to inline function calls, defining such array in global scope will cause the compiler to produce an "initializer element is not constant" error. To fix this error, move the definition of instructions array into local scope.




Note


Load, store and move instructions use **addresses expressed in 32-bit words**. Address 0 corresponds to the first word of `RTC_SLOW_MEM`.
This is different to how address arguments are handled in assembly code of the same instructions. See the section Note About Addressing for more details for reference.



To generate branch instructions, special `M_` preprocessor defines are used. `M_LABEL` define can be used to define a branch target. Label identifier is a 16-bit integer. `M_Bxxx` defines can be used to generate branch instructions with target set to a particular label.


Implementation note: these `M_` preprocessor defines will be translated into two ulp_insn_t values: one is a token value which contains label number, and the other is the actual instruction. `ulp_process_macros_and_load` function resolves the label number to the address, modifies the branch instruction to use the correct address, and removes the extra `ulp_insn_t` token which contains the label number.


Here is an example of using labels and branches:



```
const ulp_insn_t program[] = {
    I_MOVI(R0, 34),         // R0 <- 34
    M_LABEL(1),             // label_1
    I_MOVI(R1, 32),         // R1 <- 32
    I_LD(R1, R1, 0),        // R1 <- RTC_SLOW_MEM[R1]
    I_MOVI(R2, 33),         // R2 <- 33
    I_LD(R2, R2, 0),        // R2 <- RTC_SLOW_MEM[R2]
    I_SUBR(R3, R1, R2),     // R3 <- R1 - R2
    I_ST(R3, R0, 0),        // R3 -> RTC_SLOW_MEM[R0 + 0]
    I_ADDI(R0, R0, 1),      // R0++
    M_BL(1, 64),            // if (R0 < 64) goto label_1
    I_HALT(),
};
RTC_SLOW_MEM[32] = 42;
RTC_SLOW_MEM[33] = 18;
size_t load_addr = 0;
size_t size = sizeof(program)/sizeof(ulp_insn_t);
ulp_process_macros_and_load(load_addr, program, &size);
ulp_run(load_addr);

```



## API Reference



### Header File


* components/ulp/ulp_fsm/include/esp32s3/ulp.h
* This header file can be included with:



> ```
> #include "ulp.h"
> 
> ```
* This header file is a part of the API provided by the `ulp` component. To declare that your component depends on `ulp`, add the following to your CMakeLists.txt:



> ```
> REQUIRES ulp
> 
> ```
> 
> 
> or
> 
> 
> 
> ```
> PRIV_REQUIRES ulp
> 
> ```




### Functions


#### SOC_REG_TO_ULP_PERIPH_SEL


```


static inline uint32_t SOC_REG_TO_ULP_PERIPH_SEL(uint32_t reg)
Map SoC peripheral register to periph_sel field of RD_REG and WR_REG instructions.

Parameters
reg -- peripheral register in RTC_CNTL_, RTC_IO_, SENS_, RTC_I2C peripherals. 

Returns
periph_sel value for the peripheral to which this register belongs. 



```



### Unions


#### ulp_insn


```


union ulp_insn

#include <ulp.h>
Instruction format structure. 
All ULP instructions are 32 bit long. This union contains field layouts used by all of the supported instructions. This union also includes a special "macro" instruction layout. This is not a real instruction which can be executed by the CPU. It acts as a token which is removed from the program by the ulp_process_macros_and_load function.
These structures are not intended to be used directly. Preprocessor definitions provided below fill the fields of these structure with the right arguments. 

Public Members
cycles

uint32_t cycles
Number of cycles to sleep
TBD, cycles used for measurement 

unused

uint32_t unused
Unused 

opcode

uint32_t opcode
Opcode (OPCODE_DELAY)
Opcode (OPCODE_ST)
Opcode (OPCODE_LD)
Opcode (OPCODE_HALT)
Opcode (OPCODE_BRANCH)
Opcode (OPCODE_ALU)
Opcode (OPCODE_WR_REG)
Opcode (OPCODE_RD_REG)
Opcode (OPCODE_ADC)
Opcode (OPCODE_TSENS)
Opcode (OPCODE_I2C)
Opcode (OPCODE_END)
Opcode (OPCODE_MACRO) 

delay

struct ulp_insn::[anonymous] delay
Format of DELAY instruction 

dreg

uint32_t dreg
Register which contains data to store
Register where the data should be loaded to
Register which contains target PC, expressed in words (used if .reg == 1)
Destination register
Register where to store ADC result
Register where to store temperature measurement result 

sreg

uint32_t sreg
Register which contains address in RTC memory (expressed in words)
Register with operand A 

label

uint32_t label
Data label, 2-bit user defined unsigned value
Label number 

upper

uint32_t upper
0: write the low half-word; 1: write the high half-word 

wr_way

uint32_t wr_way
0: write the full-word; 1: with the label; 3: without the label 

unused1

uint32_t unused1
Unused 

offset

uint32_t offset
Offset to add to sreg
Absolute value of target PC offset w.r.t. current PC, expressed in words 

unused2

uint32_t unused2
Unused 

sub_opcode

uint32_t sub_opcode
Sub opcode (SUB_OPCODE_ST)
Sub opcode (SUB_OPCODE_BX)
Sub opcode (SUB_OPCODE_B)
Sub opcode (SUB_OPCODE_ALU_REG)
Sub opcode (SUB_OPCODE_ALU_IMM)
Sub opcode (SUB_OPCODE_ALU_CNT)
Sub opcode (SUB_OPCODE_WAKEUP)
SUB_OPCODE_MACRO_LABEL or SUB_OPCODE_MACRO_BRANCH 

st

struct ulp_insn::[anonymous] st
Format of ST instruction 

rd_upper

uint32_t rd_upper
0: read the high half-word; 1: read the low half-word 

ld

struct ulp_insn::[anonymous] ld
Format of LD instruction 

halt

struct ulp_insn::[anonymous] halt
Format of HALT instruction 

addr

uint32_t addr
Target PC, expressed in words (used if .reg == 0)
Address within either RTC_CNTL, RTC_IO, or SARADC 

reg

uint32_t reg
Target PC in register (1) or immediate (0) 

type

uint32_t type
Jump condition (BX_JUMP_TYPE_xxx) 

bx

struct ulp_insn::[anonymous] bx
Format of BRANCH instruction (absolute address) 

imm

uint32_t imm
Immediate value to compare against
Immediate value of operand B
Immediate value 

cmp

uint32_t cmp
Comparison to perform: B_CMP_L or B_CMP_GE 

sign

uint32_t sign
Sign of target PC offset: 0: positive, 1: negative 

b

struct ulp_insn::[anonymous] b
Format of BRANCH instruction (relative address) 

treg

uint32_t treg
Register with operand B 

sel

uint32_t sel
Operation to perform, one of ALU_SEL_xxx 

alu_reg

struct ulp_insn::[anonymous] alu_reg
Format of ALU instruction (both sources are registers) 

alu_imm

struct ulp_insn::[anonymous] alu_imm
Format of ALU instruction (one source is an immediate) 

unused3

uint32_t unused3
Unused 

alu_cnt

struct ulp_insn::[anonymous] alu_cnt
Format of ALU instruction with stage count register and an immediate 

periph_sel

uint32_t periph_sel
Select peripheral: RTC_CNTL (0), RTC_IO(1), SARADC(2) 

data

uint32_t data
8 bits of data to write
Data to read or write 

low

uint32_t low
Low bit 

high

uint32_t high
High bit 

wr_reg

struct ulp_insn::[anonymous] wr_reg
Format of WR_REG instruction 

rd_reg

struct ulp_insn::[anonymous] rd_reg
Format of RD_REG instruction 

mux

uint32_t mux
Select SARADC pad (mux + 1) 

sar_sel

uint32_t sar_sel
Select SARADC0 (0) or SARADC1 (1) 

adc

struct ulp_insn::[anonymous] adc
Format of ADC instruction 

wait_delay

uint32_t wait_delay
Cycles to wait after measurement is done 

reserved

uint32_t reserved
Reserved, set to 0 

tsens

struct ulp_insn::[anonymous] tsens
Format of TSENS instruction 

i2c_addr

uint32_t i2c_addr
I2C slave address 

low_bits

uint32_t low_bits
TBD 

high_bits

uint32_t high_bits
TBD 

i2c_sel

uint32_t i2c_sel
TBD, select reg_i2c_slave_address[7:0] 

rw

uint32_t rw
Write (1) or read (0) 

i2c

struct ulp_insn::[anonymous] i2c
Format of I2C instruction 

wakeup

uint32_t wakeup
Set to 1 to wake up chip 

end

struct ulp_insn::[anonymous] end
Format of END instruction with wakeup 

macro

struct ulp_insn::[anonymous] macro
Format of tokens used by LABEL and BRANCH macros 



```



### Macros


#### R0


```


R0
general purpose register 0 

```

#### R1


```


R1
general purpose register 1 

```

#### R2


```


R2
general purpose register 2 

```

#### R3


```


R3
general purpose register 3 

```

#### OPCODE_WR_REG


```


OPCODE_WR_REG
Instruction: write peripheral register (RTC_CNTL/RTC_IO/SARADC) (not implemented yet) 

```

#### OPCODE_RD_REG


```


OPCODE_RD_REG
Instruction: read peripheral register (RTC_CNTL/RTC_IO/SARADC) (not implemented yet) 

```

#### RD_REG_PERIPH_RTC_CNTL


```


RD_REG_PERIPH_RTC_CNTL
Identifier of RTC_CNTL peripheral for RD_REG and WR_REG instructions 

```

#### RD_REG_PERIPH_RTC_IO


```


RD_REG_PERIPH_RTC_IO
Identifier of RTC_IO peripheral for RD_REG and WR_REG instructions 

```

#### RD_REG_PERIPH_SENS


```


RD_REG_PERIPH_SENS
Identifier of SARADC peripheral for RD_REG and WR_REG instructions 

```

#### RD_REG_PERIPH_RTC_I2C


```


RD_REG_PERIPH_RTC_I2C
Identifier of RTC_I2C peripheral for RD_REG and WR_REG instructions 

```

#### OPCODE_I2C


```


OPCODE_I2C
Instruction: read/write I2C (not implemented yet) 

```

#### OPCODE_DELAY


```


OPCODE_DELAY
Instruction: delay (nop) for a given number of cycles 

```

#### OPCODE_ADC


```


OPCODE_ADC
Instruction: SAR ADC measurement (not implemented yet) 

```

#### OPCODE_ST


```


OPCODE_ST
Instruction: store indirect to RTC memory 

```

#### SUB_OPCODE_ST_AUTO


```


SUB_OPCODE_ST_AUTO
Automatic Storage Mode - Access continuous addresses. Use SUB_OPCODE_ST_OFFSET to configure the initial address before using this instruction. 

```

#### SUB_OPCODE_ST_OFFSET


```


SUB_OPCODE_ST_OFFSET
Automatic Storage Mode - Configure the initial address. 

```

#### SUB_OPCODE_ST


```


SUB_OPCODE_ST
Manual Storage Mode. Store 32 bits, 16 MSBs contain PC, 16 LSBs contain value from source register 

```

#### OPCODE_ALU


```


OPCODE_ALU
Arithmetic instructions 

```

#### SUB_OPCODE_ALU_REG


```


SUB_OPCODE_ALU_REG
Arithmetic instruction, both source values are in register 

```

#### SUB_OPCODE_ALU_IMM


```


SUB_OPCODE_ALU_IMM
Arithmetic instruction, one source value is an immediate 

```

#### SUB_OPCODE_ALU_CNT


```


SUB_OPCODE_ALU_CNT
Arithmetic instruction between counter register and an immediate (not implemented yet) 

```

#### ALU_SEL_ADD


```


ALU_SEL_ADD
Addition 

```

#### ALU_SEL_SUB


```


ALU_SEL_SUB
Subtraction 

```

#### ALU_SEL_AND


```


ALU_SEL_AND
Logical AND 

```

#### ALU_SEL_OR


```


ALU_SEL_OR
Logical OR 

```

#### ALU_SEL_MOV


```


ALU_SEL_MOV
Copy value (immediate to destination register or source register to destination register 

```

#### ALU_SEL_LSH


```


ALU_SEL_LSH
Shift left by given number of bits 

```

#### ALU_SEL_RSH


```


ALU_SEL_RSH
Shift right by given number of bits 

```

#### ALU_SEL_STAGE_INC


```


ALU_SEL_STAGE_INC
Increment stage count register 

```

#### ALU_SEL_STAGE_DEC


```


ALU_SEL_STAGE_DEC
Decrement stage count register 

```

#### ALU_SEL_STAGE_RST


```


ALU_SEL_STAGE_RST
Reset stage count register 

```

#### OPCODE_BRANCH


```


OPCODE_BRANCH
Branch instructions 

```

#### SUB_OPCODE_B


```


SUB_OPCODE_B
Branch to a relative offset 

```

#### SUB_OPCODE_BX


```


SUB_OPCODE_BX
Branch to absolute PC (immediate or in register) 

```

#### SUB_OPCODE_BS


```


SUB_OPCODE_BS
Branch to a relative offset by comparing the stage_cnt register 

```

#### BX_JUMP_TYPE_DIRECT


```


BX_JUMP_TYPE_DIRECT
Unconditional jump 

```

#### BX_JUMP_TYPE_ZERO


```


BX_JUMP_TYPE_ZERO
Branch if last ALU result is zero 

```

#### BX_JUMP_TYPE_OVF


```


BX_JUMP_TYPE_OVF
Branch if last ALU operation caused and overflow 

```

#### B_CMP_L


```


B_CMP_L
Branch if R0 is less than an immediate 

```

#### B_CMP_G


```


B_CMP_G
Branch if R0 is greater than an immediate 

```

#### B_CMP_E


```


B_CMP_E
Branch if R0 is equal to an immediate 

```

#### BS_CMP_L


```


BS_CMP_L
Branch if stage_cnt is less than an immediate 

```

#### BS_CMP_GE


```


BS_CMP_GE
Branch if stage_cnt is greater than or equal to an immediate 

```

#### BS_CMP_LE


```


BS_CMP_LE
Branch if stage_cnt is less than or equal to an immediate 

```

#### OPCODE_END


```


OPCODE_END
Stop executing the program 

```

#### SUB_OPCODE_END


```


SUB_OPCODE_END
Stop executing the program and optionally wake up the chip 

```

#### SUB_OPCODE_SLEEP


```


SUB_OPCODE_SLEEP
Stop executing the program and run it again after selected interval 

```

#### OPCODE_TSENS


```


OPCODE_TSENS
Instruction: temperature sensor measurement (not implemented yet) 

```

#### OPCODE_HALT


```


OPCODE_HALT
Halt the coprocessor 

```

#### OPCODE_LD


```


OPCODE_LD
Indirect load lower 16 bits from RTC memory 

```

#### OPCODE_MACRO


```


OPCODE_MACRO
Not a real opcode. Used to identify labels and branches in the program 

```

#### SUB_OPCODE_MACRO_LABEL


```


SUB_OPCODE_MACRO_LABEL
Label macro 

```

#### SUB_OPCODE_MACRO_BRANCH


```


SUB_OPCODE_MACRO_BRANCH
Branch macro 

```

#### SUB_OPCODE_MACRO_LABELPC


```


SUB_OPCODE_MACRO_LABELPC
Label pointer macro 

```

#### I_DELAY


```


I_DELAY(cycles_)
Delay (nop) for a given number of cycles 

```

#### I_HALT


```


I_HALT()
Halt the coprocessor.
This instruction halts the coprocessor, but keeps ULP timer active. As such, ULP program will be restarted again by timer. To stop the program and prevent the timer from restarting the program, use I_END(0) instruction. 

```

#### I_WR_REG


```


I_WR_REG(reg, low_bit, high_bit, val)
Write literal value to a peripheral register
reg[high_bit : low_bit] = val This instruction can access RTC_CNTL_, RTC_IO_, SENS_, and RTC_I2C peripheral registers. 

```

#### I_RD_REG


```


I_RD_REG(reg, low_bit, high_bit)
Read from peripheral register into R0
R0 = reg[high_bit : low_bit] This instruction can access RTC_CNTL_, RTC_IO_, SENS_, and RTC_I2C peripheral registers. 

```

#### I_WR_REG_BIT


```


I_WR_REG_BIT(reg, shift, val)
Set or clear a bit in the peripheral register.
Sets bit (1 << shift) of register reg to value val. This instruction can access RTC_CNTL_, RTC_IO_, SENS_, and RTC_I2C peripheral registers. 

```

#### I_WAKE


```


I_WAKE()
Wake the SoC from deep sleep.
This instruction initiates wake up from deep sleep. Use esp_deep_sleep_enable_ulp_wakeup to enable deep sleep wakeup triggered by the ULP before going into deep sleep. Note that ULP program will still keep running until the I_HALT instruction, and it will still be restarted by timer at regular intervals, even when the SoC is woken up.
To stop the ULP program, use I_HALT instruction.
To disable the timer which start ULP program, use I_END() instruction. I_END instruction clears the RTC_CNTL_ULP_CP_SLP_TIMER_EN_S bit of RTC_CNTL_ULP_CP_TIMER_REG register, which controls the ULP timer. 

```

#### I_END


```


I_END()
Stop ULP program timer.
This is a convenience macro which disables the ULP program timer. Once this instruction is used, ULP program will not be restarted anymore until ulp_run function is called.
ULP program will continue running after this instruction. To stop the currently running program, use I_HALT(). 

```

#### I_TSENS


```


I_TSENS(reg_dest, delay)
Perform temperature sensor measurement and store it into reg_dest.
Delay can be set between 1 and ((1 << 14) - 1). Higher values give higher measurement resolution. 

```

#### I_ADC


```


I_ADC(reg_dest, adc_idx, pad_idx)
Perform ADC measurement and store result in reg_dest.
adc_idx selects ADC (0 or 1). pad_idx selects ADC pad (0 - 7). 

```

#### I_ST_MANUAL


```


I_ST_MANUAL(reg_val, reg_addr, offset_, label_, upper_, wr_way_)
Store lower half-word, upper half-word or full-word data from register reg_val into RTC memory address.
This instruction can be used to write data to discontinuous addresses in the RTC_SLOW_MEM. The value is written to an offset calculated by adding the value of reg_addr register and offset_ field (this offset is expressed in 32-bit words). The storage method is dictated by the wr_way and upper field settings as summarized in the following table:
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* | wr_way | upper |                                        data                                            |          operation         |
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* |        |       |                                                                                        | Write full-word, including |
* |   0    |   X   | RTC_SLOW_MEM[addr + offset_]{31:0} = {insn_PC[10:0], 3’b0, label_[1:0], reg_val[15:0]} | the PC and the data        |
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* |        |       |                                                                                        | Store the data with label  |
* |   1    |   0   | RTC_SLOW_MEM[addr + offset_]{15:0} = {label_[1:0], reg_val[13:0]}                      | in the low half-word       |
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* |        |       |                                                                                        | Store the data with label  |
* |   1    |   1   | RTC_SLOW_MEM[addr + offset_]{31:16} = {label_[1:0], reg_val[13:0]}                     | in the high half-word      |
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* |        |       |                                                                                        | Store the data without     |
* |   3    |   0   | RTC_SLOW_MEM[addr + offset_]{15:0} = reg_val[15:0]                                     | label in the low half-word |
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
* |        |       |                                                                                        | Store the data without     |
* |   3    |   1   | RTC_SLOW_MEM[addr + offset_]{31:16} = reg_val[15:0]                                    | label in the high half-word|
* |--------|-------|----------------------------------------------------------------------------------------|----------------------------|
*


SUB_OPCODE_ST = manual_en:1, offset_set:0, wr_auto:0 

```

#### I_ST


```


I_ST(reg_val, reg_addr, offset_)
Store value from register reg_val into RTC memory.
I_ST() instruction provides backward compatibility for code written for esp32 to be run on esp32s2. This instruction is equivalent to calling I_ST_MANUAL() instruction with label = 0, upper = 0 and wr_way = 3. 

```

#### I_STL


```


I_STL(reg_val, reg_addr, offset_)
Store value from register reg_val to lower 16 bits of the RTC memory address.
This instruction is equivalent to calling I_ST_MANUAL() instruction with label = 0, upper = 0 and wr_way = 3. 

```

#### I_STH


```


I_STH(reg_val, reg_addr, offset_)
Store value from register reg_val to upper 16 bits of the RTC memory address.
This instruction is equivalent to calling I_ST_MANUAL() instruction with label = 0, upper = 1 and wr_way = 3. 

```

#### I_ST32


```


I_ST32(reg_val, reg_addr, offset_, label_)
Store value from register reg_val to full 32 bit word of the RTC memory address.
This instruction is equivalent to calling I_ST_MANUAL() instruction with wr_way = 0. 

```

#### I_STL_LABEL


```


I_STL_LABEL(reg_val, reg_addr, offset_, label_)
Store value from register reg_val with label to lower 16 bits of RTC memory address.
This instruction is equivalent to calling I_ST_MANUAL() instruction with label = label_, upper = 0 and wr_way = 1. 

```

#### I_STH_LABEL


```


I_STH_LABEL(reg_val, reg_addr, offset_, label_)
Store value from register reg_val with label to upper 16 bits of RTC memory address.
This instruction is equivalent to calling I_ST_MANUAL() instruction with label = label_, upper = 1 and wr_way = 1. 

```

#### I_ST_AUTO


```


I_ST_AUTO(reg_val, reg_addr, label_, wr_way_)
Store lower half-word, upper half-word or full-word data from register reg_val into RTC memory address with auto-increment of the offset value.
This instruction can be used to write data to continuous addresses in the RTC_SLOW_MEM. The initial address must be set using the SUB_OPCODE_ST_OFFSET instruction before the auto store instruction is called. The data written to the RTC memory address could be written to the full 32 bit word or to the lower half-word or the upper half-word. The storage method is dictated by the wr_way field and the number of times the SUB_OPCODE_ST_AUTO instruction is called. write_cnt indicates the later. The following table summarizes the storage method:
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* | wr_way | write_cnt |                                        data                                            |          operation         |
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* |        |           |                                                                                        | Write full-word, including |
* |   0    |     X     | RTC_SLOW_MEM[addr + offset_]{31:0} = {insn_PC[10:0], 3’b0, label_[1:0], reg_val[15:0]} | the PC and the data        |
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* |        |           |                                                                                        | Store the data with label  |
* |   1    |    odd    | RTC_SLOW_MEM[addr + offset_]{15:0} = {label_[1:0], reg_val[13:0]}                      | in the low half-word       |
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* |        |           |                                                                                        | Store the data with label  |
* |   1    |    even   | RTC_SLOW_MEM[addr + offset_]{31:16} = {label_[1:0], reg_val[13:0]}                     | in the high half-word      |
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* |        |           |                                                                                        | Store the data without     |
* |   3    |    odd    | RTC_SLOW_MEM[addr + offset_]{15:0} = reg_val[15:0]                                     | label in the low half-word |
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
* |        |           |                                                                                        | Store the data without     |
* |   3    |    even   | RTC_SLOW_MEM[addr + offset_]{31:16} = reg_val[15:0]                                    | label in the high half-word|
* |--------|-----------|----------------------------------------------------------------------------------------|----------------------------|
*


The initial address offset is incremented after each store operation as follows:
When a full-word is written, the offset is automatically incremented by 1 after each SUB_OPCODE_ST_AUTO operation.
When a half-word is written (lower half-word first), the offset is automatically incremented by 1 after two SUB_OPCODE_ST_AUTO operations.
SUB_OPCODE_ST_AUTO = manual_en:0, offset_set:0, wr_auto:1 




```

#### I_STO


```


I_STO(offset_)
Set the initial address offset for auto-store operation
This instruction sets the initial address of the RTC_SLOW_MEM to be used by the auto-store operation. The offset is incremented automatically. Refer I_ST_AUTO() for detailed explaination.
SUB_OPCODE_ST_OFFSET = manual_en:0, offset_set:1, wr_auto:1 

```

#### I_STI


```


I_STI(reg_val, reg_addr)
Store value from register reg_val to 32 bit word of the RTC memory address.
This instruction is equivalent to calling I_ST_AUTO() instruction with label = 0 and wr_way = 3. The data in reg_val will be either written to the lower half-word or the upper half-word of the RTC memory address depending on the count of the number of times the I_STI() instruction is called. The initial offset is automatically incremented with I_STI() is called twice. Refer I_ST_AUTO() for detailed explaination. 

```

#### I_STI_LABEL


```


I_STI_LABEL(reg_val, reg_addr, label_)
Store value from register reg_val with label to 32 bit word of the RTC memory address.
This instruction is equivalent to calling I_ST_AUTO() instruction with label = label_ and wr_way = 1. The data in reg_val will be either written to the lower half-word or the upper half-word of the RTC memory address depending on the count of the number of times the I_STI_LABEL() instruction is called. The initial offset is automatically incremented with I_STI_LABEL() is called twice. Refer I_ST_AUTO() for detailed explaination. 

```

#### I_STI32


```


I_STI32(reg_val, reg_addr, label_)
Store value from register reg_val to full 32 bit word of the RTC memory address.
This instruction is equivalent to calling I_ST_AUTO() instruction with label = label_ and wr_way = 0. The data in reg_val will be written to the RTC memory address along with the label and the PC. The initial offset is automatically incremented each time the I_STI32() instruction is called. Refer I_ST_AUTO() for detailed explaination. 

```

#### I_LD_MANUAL


```


I_LD_MANUAL(reg_dest, reg_addr, offset_, rd_upper_)
Load lower half-word, upper half-word or full-word data from RTC memory address into the register reg_dest.
This instruction reads the lower half-word or upper half-word of the RTC memory address depending on the value of rd_upper_. The following table summarizes the loading method:
* |----------|------------------------------------------------------|-------------------------|
* | rd_upper |                       data                           |        operation        |
* |----------|------------------------------------------------------|-------------------------|
* |          |                                                      | Read lower half-word of |
* |    0     | reg_dest{15:0} = RTC_SLOW_MEM[addr + offset_]{31:16} | the memory              |
* |----------|------------------------------------------------------|-------------------------|
* |          |                                                      | Read upper half-word of |
* |    1     | reg_dest{15:0} = RTC_SLOW_MEM[addr + offset_]{15:0}  | the memory              |
* |----------|------------------------------------------------------|-------------------------|
*



```

#### I_LD


```


I_LD(reg_dest, reg_addr, offset_)
Load lower 16 bits value from RTC memory into reg_dest register.
Loads 16 LSBs (rd_upper = 1) from RTC memory word given by the sum of value in reg_addr and value of offset_. I_LD() instruction provides backward compatibility for code written for esp32 to be run on esp32s2. 

```

#### I_LDL


```


I_LDL(reg_dest, reg_addr, offset_)
Load lower 16 bits value from RTC memory into reg_dest register.
I_LDL() instruction and I_LD() instruction can be used interchangably. 

```

#### I_LDH


```


I_LDH(reg_dest, reg_addr, offset_)
Load upper 16 bits value from RTC memory into reg_dest register.
Loads 16 MSBs (rd_upper = 0) from RTC memory word given by the sum of value in reg_addr and value of offset_. 

```

#### I_BL


```


I_BL(pc_offset, imm_value)
Branch relative if R0 register less than the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_BG


```


I_BG(pc_offset, imm_value)
Branch relative if R0 register greater than the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_BE


```


I_BE(pc_offset, imm_value)
Branch relative if R0 register is equal to the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_BXR


```


I_BXR(reg_pc)
Unconditional branch to absolute PC, address in register.
reg_pc is the register which contains address to jump to. Address is expressed in 32-bit words. 

```

#### I_BXI


```


I_BXI(imm_pc)
Unconditional branch to absolute PC, immediate address.
Address imm_pc is expressed in 32-bit words. 

```

#### I_BXZR


```


I_BXZR(reg_pc)
Branch to absolute PC if ALU result is zero, address in register.
reg_pc is the register which contains address to jump to. Address is expressed in 32-bit words. 

```

#### I_BXZI


```


I_BXZI(imm_pc)
Branch to absolute PC if ALU result is zero, immediate address.
Address imm_pc is expressed in 32-bit words. 

```

#### I_BXFR


```


I_BXFR(reg_pc)
Branch to absolute PC if ALU overflow, address in register
reg_pc is the register which contains address to jump to. Address is expressed in 32-bit words. 

```

#### I_BXFI


```


I_BXFI(imm_pc)
Branch to absolute PC if ALU overflow, immediate address
Address imm_pc is expressed in 32-bit words. 

```

#### I_BSLE


```


I_BSLE(pc_offset, imm_value)
Branch relative if stage_cnt is less than or equal to the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_BSGE


```


I_BSGE(pc_offset, imm_value)
Branch relative if stage_cnt register is greater than or equal to the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_BSL


```


I_BSL(pc_offset, imm_value)
Branch relative if stage_cnt register is less than the immediate value.
pc_offset is expressed in words, and can be from -127 to 127 imm_value is a 16-bit value to compare R0 against 

```

#### I_ADDR


```


I_ADDR(reg_dest, reg_src1, reg_src2)
Addition: dest = src1 + src2 

```

#### I_SUBR


```


I_SUBR(reg_dest, reg_src1, reg_src2)
Subtraction: dest = src1 - src2 

```

#### I_ANDR


```


I_ANDR(reg_dest, reg_src1, reg_src2)
Logical AND: dest = src1 & src2 

```

#### I_ORR


```


I_ORR(reg_dest, reg_src1, reg_src2)
Logical OR: dest = src1 | src2 

```

#### I_MOVR


```


I_MOVR(reg_dest, reg_src)
Copy: dest = src 

```

#### I_LSHR


```


I_LSHR(reg_dest, reg_src, reg_shift)
Logical shift left: dest = src << shift 

```

#### I_RSHR


```


I_RSHR(reg_dest, reg_src, reg_shift)
Logical shift right: dest = src >> shift 

```

#### I_ADDI


```


I_ADDI(reg_dest, reg_src, imm_)
Add register and an immediate value: dest = src1 + imm 

```

#### I_SUBI


```


I_SUBI(reg_dest, reg_src, imm_)
Subtract register and an immediate value: dest = src - imm 

```

#### I_ANDI


```


I_ANDI(reg_dest, reg_src, imm_)
Logical AND register and an immediate value: dest = src & imm 

```

#### I_ORI


```


I_ORI(reg_dest, reg_src, imm_)
Logical OR register and an immediate value: dest = src | imm 

```

#### I_MOVI


```


I_MOVI(reg_dest, imm_)
Copy an immediate value into register: dest = imm 

```

#### I_LSHI


```


I_LSHI(reg_dest, reg_src, imm_)
Logical shift left register value by an immediate: dest = src << imm 

```

#### I_RSHI


```


I_RSHI(reg_dest, reg_src, imm_)
Logical shift right register value by an immediate: dest = val >> imm 

```

#### I_STAGE_INC


```


I_STAGE_INC(reg_dest, reg_src, imm_)
Increment stage_cnt register by an immediate: stage_cnt = stage_cnt + imm 

```

#### I_STAGE_DEC


```


I_STAGE_DEC(reg_dest, reg_src, imm_)
Decrement stage_cnt register by an immediate: stage_cnt = stage_cnt - imm 

```

#### I_STAGE_RST


```


I_STAGE_RST(reg_dest, reg_src, imm_)
Reset stage_cnt register by an immediate: stage_cnt = 0 

```

#### M_LABEL


```


M_LABEL(label_num)
Define a label with number label_num.
This is a macro which doesn't generate a real instruction. The token generated by this macro is removed by ulp_process_macros_and_load function. Label defined using this macro can be used in branch macros defined below. 

```

#### M_BRANCH


```


M_BRANCH(label_num)
Token macro used by M_B and M_BX macros. Not to be used directly. 

```

#### M_BL


```


M_BL(label_num, imm_value)
Macro: branch to label label_num if R0 is less than immediate value.
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```

#### M_BG


```


M_BG(label_num, imm_value)
Macro: branch to label label_num if R0 is greater than immediate value
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```

#### M_BE


```


M_BE(label_num, imm_value)
Macro: branch to label label_num if R0 equal to the immediate value
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```

#### M_BX


```


M_BX(label_num)
Macro: unconditional branch to label
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```

#### M_BXZ


```


M_BXZ(label_num)
Macro: branch to label if ALU result is zero
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```

#### M_BXF


```


M_BXF(label_num)
Macro: branch to label if ALU overflow
This macro generates two ulp_insn_t values separated by a comma, and should be used when defining contents of ulp_insn_t arrays. First value is not a real instruction; it is a token which is removed by ulp_process_macros_and_load function. 

```