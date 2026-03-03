#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int binToInt(char *bits, int len) {
    int val = 0;
    for (int i = 0; i < len; i++)
        val = val * 2 + (bits[i] - '0');
    return val;
}

int signExtend(int val, int bits) {
    if ((val >> (bits - 1)) & 1)
        val -= (1 << bits);
    return val;
}

int main() {
    char instr[33];

    while (1) {
        printf("Enter an instruction:\n");
        scanf("%32s", instr);

        char opcode[8], rd[6], rs1[6], rs2[6], funct3[4], funct7[8];

        strncpy(opcode, instr + 25, 7); opcode[7] = '\0';
        strncpy(rd,     instr + 20, 5); rd[5] = '\0';
        strncpy(funct3, instr + 17, 3); funct3[3] = '\0';
        strncpy(rs1,    instr + 12, 5); rs1[5] = '\0';
        strncpy(rs2,    instr + 7,  5); rs2[5] = '\0';
        strncpy(funct7, instr,      7); funct7[7] = '\0';

        int op  = binToInt(opcode, 7);
        int rdN = binToInt(rd, 5);
        int f3  = binToInt(funct3, 3);
        int r1  = binToInt(rs1, 5);
        int r2  = binToInt(rs2, 5);
        int f7  = binToInt(funct7, 7);

        if (op == 0b0110011) {
            char *name = "";
            if      (f3 == 0 && f7 == 0)  name = "add";
            else if (f3 == 0 && f7 == 32) name = "sub";
            else if (f3 == 7 && f7 == 0)  name = "and";
            else if (f3 == 6 && f7 == 0)  name = "or";
            else if (f3 == 4 && f7 == 0)  name = "xor";
            else if (f3 == 1 && f7 == 0)  name = "sll";
            else if (f3 == 5 && f7 == 0)  name = "srl";
            else if (f3 == 5 && f7 == 32) name = "sra";
            else if (f3 == 2 && f7 == 0)  name = "slt";
            else if (f3 == 3 && f7 == 0)  name = "sltu";

            printf("Instruction Type: R\n");
            printf("Operation: %s\n", name);
            printf("Rs1: x%d\n", r1);
            printf("Rs2: x%d\n", r2);
            printf("Rd: x%d\n", rdN);
            printf("Funct3: %d\n", f3);
            printf("Funct7: %d\n", f7);

        } else if (op == 0b0010011 || op == 0b0000011 || op == 0b1100111) {
            char immBits[13];
            strncpy(immBits, instr, 12); immBits[12] = '\0';
            int immVal = signExtend(binToInt(immBits, 12), 12);

            char *name = "";
            if (op == 0b0010011) {
                if      (f3 == 0) name = "addi";
                else if (f3 == 7) name = "andi";
                else if (f3 == 6) name = "ori";
                else if (f3 == 4) name = "xori";
                else if (f3 == 2) name = "slti";
                else if (f3 == 3) name = "sltiu";
                else if (f3 == 1) name = "slli";
                else if (f3 == 5 && f7 == 0)  name = "srli";
                else if (f3 == 5 && f7 == 32) name = "srai";
            } else if (op == 0b0000011) {
                if      (f3 == 0) name = "lb";
                else if (f3 == 1) name = "lh";
                else if (f3 == 2) name = "lw";
            } else {
                name = "jalr";
            }

            printf("Instruction Type : I\n");
            printf("Operation: %s\n", name);
            printf("Rs1: x%d\n", r1);
            printf("Rd: x%d\n", rdN);
            printf("Immediate: %d (or 0x%X)\n", immVal, immVal & 0xFFF);

        } else if (op == 0b0100011) {
            char combined[13];
            strncpy(combined, instr, 7);
            strncpy(combined + 7, instr + 20, 5);
            combined[12] = '\0';
            int immVal = signExtend(binToInt(combined, 12), 12);

            char *name = "";
            if      (f3 == 0) name = "sb";
            else if (f3 == 1) name = "sh";
            else if (f3 == 2) name = "sw";

            printf("Instruction Type : S\n");
            printf("Operation: %s\n", name);
            printf("Rs1: x%d\n", r1);
            printf("Rs2: x%d\n", r2);
            printf("Immediate: %d (or 0x%X)\n", immVal, immVal & 0xFFF);

        } else if (op == 0b1100011) {
            char combined[14];
            combined[0]  = instr[0];
            combined[1]  = instr[24];
            strncpy(combined + 2, instr + 1, 6);
            strncpy(combined + 8, instr + 20, 4);
            combined[12] = '0';
            combined[13] = '\0';
            int immVal = signExtend(binToInt(combined, 13), 13);

            char *name = "";
            if      (f3 == 0) name = "beq";
            else if (f3 == 1) name = "bne";
            else if (f3 == 4) name = "blt";
            else if (f3 == 5) name = "bge";

            printf("Instruction Type : SB\n");
            printf("Operation: %s\n", name);
            printf("Rs1: x%d\n", r1);
            printf("Rs2: x%d\n", r2);
            printf("Immediate: %d\n", immVal);

        } else if (op == 0b1101111) {
            char combined[22];
            combined[0]  = instr[0];
            strncpy(combined + 1, instr + 12, 8);
            combined[9]  = instr[11];
            strncpy(combined + 10, instr + 1, 10);
            combined[20] = '0';
            combined[21] = '\0';
            int immVal = signExtend(binToInt(combined, 21), 21);

            printf("Instruction Type : UJ\n");
            printf("Operation: jal\n");
            printf("Rd: x%d\n", rdN);
            printf("Immediate: %d (or 0x%X)\n", immVal, immVal & 0xFFFFF);
        }

        printf("\n");
    }

    return 0;
}