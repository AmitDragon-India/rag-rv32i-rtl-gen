#include "Vrv32i_top.h"
#include "Vrv32i_top___024root.h"
#include "verilated.h"
#include <iostream>
#include <string>
#include <cstdint>

#define MAX_CYCLES  500000
#define DMEM_DEPTH  8192
#define TOHOST_IDX  (0x1000 >> 2)

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::string hexfile = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.rfind("+load=", 0) == 0)
            hexfile = arg.substr(6);
    }

    if (hexfile.empty()) {
        std::cerr << "Usage: ./sim +load=<test.hex>" << std::endl;
        return 1;
    }

    Vrv32i_top* top = new Vrv32i_top;

    // Reset
    top->rst_n = 0;
    top->clk   = 0;
    for (int i = 0; i < 20; i++) {
        top->clk = !top->clk;
        top->eval();
    }
    top->rst_n = 1;

    int cycles = 0;

    while (cycles < MAX_CYCLES) {
        top->clk = 1;
        top->eval();

        

        // Check tohost
        uint32_t tohost_val = top->rootp->rv32i_top__DOT__dmem[TOHOST_IDX];
        if (tohost_val != 0) {
            if (tohost_val == 1) {
                std::cout << "PASS: " << hexfile
                          << " (" << cycles << " cycles)" << std::endl;
                delete top;
                return 0;
            } else {
                std::cout << "FAIL: " << hexfile
                          << " (tohost=" << tohost_val
                          << ", test=" << (tohost_val >> 1) << ")"
                          << " (" << cycles << " cycles)" << std::endl;
                delete top;
                return 1;
            }
        }

        // Intercept ecall
        uint32_t id_ex_opcode = top->rootp->rv32i_top__DOT__id_ex_opcode;
        uint32_t id_ex_imm    = top->rootp->rv32i_top__DOT__id_ex_imm;
        uint32_t id_ex_funct3 = top->rootp->rv32i_top__DOT__id_ex_funct3;

        if (id_ex_opcode == 0x73 && id_ex_funct3 == 0 && id_ex_imm == 0) {
            uint32_t gp = top->rootp->rv32i_top__DOT__regfile__DOT__registers[3];
            if (gp == 1) {
                std::cout << "PASS: " << hexfile
                          << " (ecall, " << cycles << " cycles)" << std::endl;
                delete top;
                return 0;
            } else if (gp != 0) {
                std::cout << "FAIL: " << hexfile
                          << " (ecall, gp=" << gp
                          << ", test=" << (gp >> 1) << ")"
                          << " (" << cycles << " cycles)" << std::endl;
                delete top;
                return 1;
            }
        }

        top->clk = 0;
        top->eval();
        cycles++;
    }

    std::cout << "TIMEOUT: " << hexfile
              << " (" << cycles << " cycles)" << std::endl;
    delete top;
    return 2;
}
