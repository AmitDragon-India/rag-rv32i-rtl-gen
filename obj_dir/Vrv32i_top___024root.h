// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vrv32i_top.h for the primary calling header

#ifndef VERILATED_VRV32I_TOP___024ROOT_H_
#define VERILATED_VRV32I_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vrv32i_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vrv32i_top___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        CData/*6:0*/ rv32i_top__DOT__id_opcode;
        CData/*0:0*/ rv32i_top__DOT__id_reg_write;
        CData/*0:0*/ rv32i_top__DOT__id_mem_read;
        CData/*0:0*/ rv32i_top__DOT__id_mem_write;
        CData/*0:0*/ rv32i_top__DOT__id_branch;
        CData/*0:0*/ rv32i_top__DOT__id_jump;
        CData/*0:0*/ rv32i_top__DOT__id_alu_srcA;
        CData/*1:0*/ rv32i_top__DOT__id_alu_srcB;
        CData/*1:0*/ rv32i_top__DOT__id_wb_sel;
        CData/*0:0*/ rv32i_top__DOT__id_ex_reg_write;
        CData/*0:0*/ rv32i_top__DOT__id_ex_mem_read;
        CData/*0:0*/ rv32i_top__DOT__id_ex_mem_write;
        CData/*0:0*/ rv32i_top__DOT__id_ex_branch;
        CData/*0:0*/ rv32i_top__DOT__id_ex_jump;
        CData/*0:0*/ rv32i_top__DOT__id_ex_alu_srcA;
        CData/*1:0*/ rv32i_top__DOT__id_ex_alu_srcB;
        CData/*1:0*/ rv32i_top__DOT__id_ex_wb_sel;
        CData/*3:0*/ rv32i_top__DOT__id_ex_alu_op;
        CData/*4:0*/ rv32i_top__DOT__id_ex_rd;
        CData/*4:0*/ rv32i_top__DOT__id_ex_rs1;
        CData/*4:0*/ rv32i_top__DOT__id_ex_rs2;
        CData/*2:0*/ rv32i_top__DOT__id_ex_funct3;
        CData/*6:0*/ rv32i_top__DOT__id_ex_opcode;
        CData/*1:0*/ rv32i_top__DOT__forward_s;
        CData/*0:0*/ rv32i_top__DOT__ex_redirect_en;
        CData/*0:0*/ rv32i_top__DOT__stall_pc;
        CData/*0:0*/ rv32i_top__DOT__stall_if_id;
        CData/*0:0*/ rv32i_top__DOT__flush_if_id;
        CData/*0:0*/ rv32i_top__DOT__flush_id_ex;
        CData/*0:0*/ rv32i_top__DOT__ex_mem_reg_write;
        CData/*0:0*/ rv32i_top__DOT__ex_mem_mem_read;
        CData/*0:0*/ rv32i_top__DOT__ex_mem_mem_write;
        CData/*1:0*/ rv32i_top__DOT__ex_mem_wb_sel;
        CData/*4:0*/ rv32i_top__DOT__ex_mem_rd;
        CData/*2:0*/ rv32i_top__DOT__ex_mem_funct3;
        CData/*3:0*/ rv32i_top__DOT__dmem_byte_en;
        CData/*0:0*/ rv32i_top__DOT__dmem_we;
        CData/*0:0*/ rv32i_top__DOT__mem_wb_reg_write;
        CData/*1:0*/ rv32i_top__DOT__mem_wb_wb_sel;
        CData/*4:0*/ rv32i_top__DOT__mem_wb_rd;
        CData/*3:0*/ __VdfgRegularize_h6e95ff9d_0_1;
        CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_5;
        CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_6;
        CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_7;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        IData/*31:0*/ rv32i_top__DOT__if_pc_plus4;
        IData/*31:0*/ rv32i_top__DOT__if_id_instr;
        IData/*31:0*/ rv32i_top__DOT__if_id_pc;
        IData/*31:0*/ rv32i_top__DOT__if_id_pc_plus4;
        IData/*31:0*/ rv32i_top__DOT__rs1_data;
        IData/*31:0*/ rv32i_top__DOT__rs2_data;
        IData/*31:0*/ rv32i_top__DOT__id_imm;
        IData/*31:0*/ rv32i_top__DOT__id_ex_rs1_data;
        IData/*31:0*/ rv32i_top__DOT__id_ex_rs2_data;
        IData/*31:0*/ rv32i_top__DOT__id_ex_store_data;
        IData/*31:0*/ rv32i_top__DOT__id_ex_imm;
        IData/*31:0*/ rv32i_top__DOT__id_ex_pc;
    };
    struct {
        IData/*31:0*/ rv32i_top__DOT__id_ex_pc_plus4;
        IData/*31:0*/ rv32i_top__DOT__ex_redirect_pc;
        IData/*31:0*/ rv32i_top__DOT__ex_mem_alu_result;
        IData/*31:0*/ rv32i_top__DOT__ex_mem_store_data;
        IData/*31:0*/ rv32i_top__DOT__ex_mem_pc_plus4;
        IData/*31:0*/ rv32i_top__DOT__ex_mem_imm_u;
        IData/*31:0*/ rv32i_top__DOT__ex_mem_fwd_data;
        IData/*31:0*/ rv32i_top__DOT__dmem_wdata;
        IData/*31:0*/ rv32i_top__DOT__mem_rdata_out;
        IData/*31:0*/ rv32i_top__DOT__mem_wb_alu_result;
        IData/*31:0*/ rv32i_top__DOT__mem_wb_mem_rdata;
        IData/*31:0*/ rv32i_top__DOT__mem_wb_pc_plus4;
        IData/*31:0*/ rv32i_top__DOT__mem_wb_imm_u;
        IData/*31:0*/ rv32i_top__DOT__wb_data;
        IData/*31:0*/ rv32i_top__DOT__if_stage__DOT__pc;
        IData/*31:0*/ rv32i_top__DOT__ex_stage__DOT__opA_fwd;
        IData/*31:0*/ rv32i_top__DOT__ex_stage__DOT__opB_fwd;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<IData/*31:0*/, 8192> rv32i_top__DOT__dmem;
        VlUnpacked<IData/*31:0*/, 8192> rv32i_top__DOT__if_stage__DOT__imem;
        VlUnpacked<IData/*31:0*/, 32> rv32i_top__DOT__regfile__DOT__registers;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };
    std::string rv32i_top__DOT__unnamedblk1__DOT__load_file;
    std::string rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file;

    // INTERNAL VARIABLES
    Vrv32i_top__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vrv32i_top___024root(Vrv32i_top__Syms* symsp, const char* namep);
    ~Vrv32i_top___024root();
    VL_UNCOPYABLE(Vrv32i_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
