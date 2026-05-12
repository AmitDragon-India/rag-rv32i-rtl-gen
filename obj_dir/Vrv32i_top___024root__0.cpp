// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrv32i_top.h for the primary calling header

#include "Vrv32i_top__pch.h"

void Vrv32i_top___024root___eval_triggers_vec__act(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_triggers_vec__act\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((((~ (IData)(vlSelfRef.rst_n)) 
                                                       & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0)) 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

bool Vrv32i_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vrv32i_top___024root___nba_sequent__TOP__0(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___nba_sequent__TOP__0\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ __VdlyVal__rv32i_top__DOT__dmem__v0;
    __VdlyVal__rv32i_top__DOT__dmem__v0 = 0;
    SData/*12:0*/ __VdlyDim0__rv32i_top__DOT__dmem__v0;
    __VdlyDim0__rv32i_top__DOT__dmem__v0 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__dmem__v0;
    __VdlySet__rv32i_top__DOT__dmem__v0 = 0;
    CData/*7:0*/ __VdlyVal__rv32i_top__DOT__dmem__v1;
    __VdlyVal__rv32i_top__DOT__dmem__v1 = 0;
    SData/*12:0*/ __VdlyDim0__rv32i_top__DOT__dmem__v1;
    __VdlyDim0__rv32i_top__DOT__dmem__v1 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__dmem__v1;
    __VdlySet__rv32i_top__DOT__dmem__v1 = 0;
    CData/*7:0*/ __VdlyVal__rv32i_top__DOT__dmem__v2;
    __VdlyVal__rv32i_top__DOT__dmem__v2 = 0;
    SData/*12:0*/ __VdlyDim0__rv32i_top__DOT__dmem__v2;
    __VdlyDim0__rv32i_top__DOT__dmem__v2 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__dmem__v2;
    __VdlySet__rv32i_top__DOT__dmem__v2 = 0;
    CData/*7:0*/ __VdlyVal__rv32i_top__DOT__dmem__v3;
    __VdlyVal__rv32i_top__DOT__dmem__v3 = 0;
    SData/*12:0*/ __VdlyDim0__rv32i_top__DOT__dmem__v3;
    __VdlyDim0__rv32i_top__DOT__dmem__v3 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__dmem__v3;
    __VdlySet__rv32i_top__DOT__dmem__v3 = 0;
    // Body
    __VdlySet__rv32i_top__DOT__dmem__v0 = 0U;
    __VdlySet__rv32i_top__DOT__dmem__v1 = 0U;
    __VdlySet__rv32i_top__DOT__dmem__v2 = 0U;
    __VdlySet__rv32i_top__DOT__dmem__v3 = 0U;
    if (vlSelfRef.rv32i_top__DOT__dmem_we) {
        if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
            __VdlyVal__rv32i_top__DOT__dmem__v0 = (0x000000ffU 
                                                   & vlSelfRef.rv32i_top__DOT__dmem_wdata);
            __VdlyDim0__rv32i_top__DOT__dmem__v0 = 
                (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                >> 2U));
            __VdlySet__rv32i_top__DOT__dmem__v0 = 1U;
        }
        if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
            __VdlyVal__rv32i_top__DOT__dmem__v1 = (0x000000ffU 
                                                   & (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                                                      >> 8U));
            __VdlyDim0__rv32i_top__DOT__dmem__v1 = 
                (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                >> 2U));
            __VdlySet__rv32i_top__DOT__dmem__v1 = 1U;
        }
        if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
            __VdlyVal__rv32i_top__DOT__dmem__v2 = (0x000000ffU 
                                                   & (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                                                      >> 0x10U));
            __VdlyDim0__rv32i_top__DOT__dmem__v2 = 
                (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                >> 2U));
            __VdlySet__rv32i_top__DOT__dmem__v2 = 1U;
        }
        if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
            __VdlyVal__rv32i_top__DOT__dmem__v3 = (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                                                   >> 0x18U);
            __VdlyDim0__rv32i_top__DOT__dmem__v3 = 
                (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                >> 2U));
            __VdlySet__rv32i_top__DOT__dmem__v3 = 1U;
        }
    }
    if (__VdlySet__rv32i_top__DOT__dmem__v0) {
        vlSelfRef.rv32i_top__DOT__dmem[__VdlyDim0__rv32i_top__DOT__dmem__v0] 
            = ((0xffffff00U & vlSelfRef.rv32i_top__DOT__dmem
                [__VdlyDim0__rv32i_top__DOT__dmem__v0]) 
               | (IData)(__VdlyVal__rv32i_top__DOT__dmem__v0));
    }
    if (__VdlySet__rv32i_top__DOT__dmem__v1) {
        vlSelfRef.rv32i_top__DOT__dmem[__VdlyDim0__rv32i_top__DOT__dmem__v1] 
            = ((0xffff00ffU & vlSelfRef.rv32i_top__DOT__dmem
                [__VdlyDim0__rv32i_top__DOT__dmem__v1]) 
               | ((IData)(__VdlyVal__rv32i_top__DOT__dmem__v1) 
                  << 8U));
    }
    if (__VdlySet__rv32i_top__DOT__dmem__v2) {
        vlSelfRef.rv32i_top__DOT__dmem[__VdlyDim0__rv32i_top__DOT__dmem__v2] 
            = ((0xff00ffffU & vlSelfRef.rv32i_top__DOT__dmem
                [__VdlyDim0__rv32i_top__DOT__dmem__v2]) 
               | ((IData)(__VdlyVal__rv32i_top__DOT__dmem__v2) 
                  << 0x00000010U));
    }
    if (__VdlySet__rv32i_top__DOT__dmem__v3) {
        vlSelfRef.rv32i_top__DOT__dmem[__VdlyDim0__rv32i_top__DOT__dmem__v3] 
            = ((0x00ffffffU & vlSelfRef.rv32i_top__DOT__dmem
                [__VdlyDim0__rv32i_top__DOT__dmem__v3]) 
               | ((IData)(__VdlyVal__rv32i_top__DOT__dmem__v3) 
                  << 0x00000018U));
    }
}

extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h538da069_0;
extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h76216006_0;
extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h6746615a_0;

void Vrv32i_top___024root___nba_sequent__TOP__1(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___nba_sequent__TOP__1\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ rv32i_top__DOT__forward_a;
    rv32i_top__DOT__forward_a = 0;
    CData/*1:0*/ rv32i_top__DOT__forward_b;
    rv32i_top__DOT__forward_b = 0;
    CData/*0:0*/ rv32i_top__DOT__ex_branch_taken;
    rv32i_top__DOT__ex_branch_taken = 0;
    IData/*31:0*/ rv32i_top__DOT__id_stage__DOT__imm_i;
    rv32i_top__DOT__id_stage__DOT__imm_i = 0;
    IData/*31:0*/ rv32i_top__DOT__ex_stage__DOT__jal_target;
    rv32i_top__DOT__ex_stage__DOT__jal_target = 0;
    CData/*1:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    IData/*31:0*/ __VdlyVal__rv32i_top__DOT__regfile__DOT__registers__v0;
    __VdlyVal__rv32i_top__DOT__regfile__DOT__registers__v0 = 0;
    CData/*4:0*/ __VdlyDim0__rv32i_top__DOT__regfile__DOT__registers__v0;
    __VdlyDim0__rv32i_top__DOT__regfile__DOT__registers__v0 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v0;
    __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v0 = 0;
    CData/*0:0*/ __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v1;
    __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v1 = 0;
    // Body
    __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v0 = 0U;
    __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v1 = 0U;
    vlSelfRef.rv32i_top__DOT__id_ex_jump = ((1U & (~ 
                                                   ((~ (IData)(vlSelfRef.rst_n)) 
                                                    | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                            && (IData)(vlSelfRef.rv32i_top__DOT__id_jump));
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) {
        vlSelfRef.rv32i_top__DOT__id_ex_opcode = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_rs2_data = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_rs1_data = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_pc = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_rs1 = 0U;
        vlSelfRef.rv32i_top__DOT__id_ex_rs2 = 0U;
    } else {
        vlSelfRef.rv32i_top__DOT__id_ex_opcode = vlSelfRef.rv32i_top__DOT__id_opcode;
        vlSelfRef.rv32i_top__DOT__id_ex_rs2_data = vlSelfRef.rv32i_top__DOT__rs2_data;
        vlSelfRef.rv32i_top__DOT__id_ex_rs1_data = vlSelfRef.rv32i_top__DOT__rs1_data;
        vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB = vlSelfRef.rv32i_top__DOT__id_alu_srcB;
        vlSelfRef.rv32i_top__DOT__id_ex_pc = vlSelfRef.rv32i_top__DOT__if_id_pc;
        vlSelfRef.rv32i_top__DOT__id_ex_rs1 = (0x0000001fU 
                                               & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                  >> 0x0000000fU));
        vlSelfRef.rv32i_top__DOT__id_ex_rs2 = (0x0000001fU 
                                               & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                  >> 0x00000014U));
    }
    if (vlSelfRef.rst_n) {
        if (((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
             & (0U != (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd)))) {
            __VdlyVal__rv32i_top__DOT__regfile__DOT__registers__v0 
                = vlSelfRef.rv32i_top__DOT__wb_data;
            __VdlyDim0__rv32i_top__DOT__regfile__DOT__registers__v0 
                = vlSelfRef.rv32i_top__DOT__mem_wb_rd;
            __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v0 = 1U;
        }
        vlSelfRef.rv32i_top__DOT__mem_wb_mem_rdata 
            = vlSelfRef.rv32i_top__DOT__mem_rdata_out;
        vlSelfRef.rv32i_top__DOT__ex_mem_store_data 
            = ((2U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s))
                ? vlSelfRef.rv32i_top__DOT__ex_mem_fwd_data
                : ((1U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s))
                    ? vlSelfRef.rv32i_top__DOT__wb_data
                    : vlSelfRef.rv32i_top__DOT__id_ex_store_data));
    } else {
        __VdlySet__rv32i_top__DOT__regfile__DOT__registers__v1 = 1U;
        vlSelfRef.rv32i_top__DOT__mem_wb_mem_rdata = 0U;
        vlSelfRef.rv32i_top__DOT__ex_mem_store_data = 0U;
    }
    vlSelfRef.rv32i_top__DOT__id_ex_store_data = ((1U 
                                                   & ((~ (IData)(vlSelfRef.rst_n)) 
                                                      | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                                   ? 0U
                                                   : vlSelfRef.rv32i_top__DOT__rs2_data);
    vlSelfRef.rv32i_top__DOT__ex_mem_funct3 = ((IData)(vlSelfRef.rst_n)
                                                ? (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3)
                                                : 0U);
    vlSelfRef.rv32i_top__DOT__id_ex_funct3 = ((1U & 
                                               ((~ (IData)(vlSelfRef.rst_n)) 
                                                | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                               ? 0U
                                               : (7U 
                                                  & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                     >> 0x0000000cU)));
    if (vlSelfRef.rst_n) {
        vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel = vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel;
        vlSelfRef.rv32i_top__DOT__mem_wb_pc_plus4 = vlSelfRef.rv32i_top__DOT__ex_mem_pc_plus4;
        vlSelfRef.rv32i_top__DOT__mem_wb_imm_u = vlSelfRef.rv32i_top__DOT__ex_mem_imm_u;
        vlSelfRef.rv32i_top__DOT__mem_wb_alu_result 
            = vlSelfRef.rv32i_top__DOT__ex_mem_alu_result;
        vlSelfRef.rv32i_top__DOT__mem_wb_rd = vlSelfRef.rv32i_top__DOT__ex_mem_rd;
        vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel = vlSelfRef.rv32i_top__DOT__id_ex_wb_sel;
    } else {
        vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel = 0U;
        vlSelfRef.rv32i_top__DOT__mem_wb_pc_plus4 = 0U;
        vlSelfRef.rv32i_top__DOT__mem_wb_imm_u = 0U;
        vlSelfRef.rv32i_top__DOT__mem_wb_alu_result = 0U;
        vlSelfRef.rv32i_top__DOT__mem_wb_rd = 0U;
        vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel = 0U;
    }
    vlSelfRef.rv32i_top__DOT__id_ex_wb_sel = ((1U & 
                                               ((~ (IData)(vlSelfRef.rst_n)) 
                                                | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                               ? 0U
                                               : (IData)(vlSelfRef.rv32i_top__DOT__id_wb_sel));
    vlSelfRef.rv32i_top__DOT__ex_mem_pc_plus4 = ((IData)(vlSelfRef.rst_n)
                                                  ? vlSelfRef.rv32i_top__DOT__id_ex_pc_plus4
                                                  : 0U);
    vlSelfRef.rv32i_top__DOT__id_ex_pc_plus4 = ((1U 
                                                 & ((~ (IData)(vlSelfRef.rst_n)) 
                                                    | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                                 ? 0U
                                                 : vlSelfRef.rv32i_top__DOT__if_id_pc_plus4);
    vlSelfRef.rv32i_top__DOT__ex_mem_imm_u = ((IData)(vlSelfRef.rst_n)
                                               ? vlSelfRef.rv32i_top__DOT__id_ex_imm
                                               : 0U);
    vlSelfRef.rv32i_top__DOT__id_ex_imm = ((1U & ((~ (IData)(vlSelfRef.rst_n)) 
                                                  | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                            ? 0U : vlSelfRef.rv32i_top__DOT__id_imm);
    vlSelfRef.rv32i_top__DOT__ex_mem_alu_result = ((IData)(vlSelfRef.rst_n)
                                                    ? 
                                                   ((8U 
                                                     & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                     ? 
                                                    ((- (IData)(
                                                                (1U 
                                                                 & (~ 
                                                                    ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op) 
                                                                     >> 1U))))) 
                                                     & (((1U 
                                                          & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                          ? 
                                                         VL_SHIFTRS_III(32,32,5, vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd, 
                                                                        (0x0000001fU 
                                                                         & vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd))
                                                          : 
                                                         (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                          >> 
                                                          (0x0000001fU 
                                                           & vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd))) 
                                                        & (- (IData)(
                                                                     (1U 
                                                                      & (~ 
                                                                         ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op) 
                                                                          >> 2U)))))))
                                                     : 
                                                    ((4U 
                                                      & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                        ? 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        << 
                                                        (0x0000001fU 
                                                         & vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd))
                                                        : 
                                                       (1U 
                                                        & (- (IData)((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6)))))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                        ? 
                                                       (1U 
                                                        & (- (IData)((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_7))))
                                                        : 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        ^ vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                        ? 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        | vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)
                                                        : 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        & vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd))
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_op))
                                                        ? 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        - vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)
                                                        : 
                                                       (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                        + vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)))))
                                                    : 0U);
    vlSelfRef.rv32i_top__DOT__id_ex_alu_op = ((1U & 
                                               ((~ (IData)(vlSelfRef.rst_n)) 
                                                | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                               ? 0U
                                               : ((0x00000040U 
                                                   & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                   ? 
                                                  (1U 
                                                   & (- (IData)((IData)(
                                                                        (0x00000023U 
                                                                         == 
                                                                         (0x0000003fU 
                                                                          & vlSelfRef.rv32i_top__DOT__if_id_instr))))))
                                                   : 
                                                  ((0x00000020U 
                                                    & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                    ? 
                                                   (((0x00004000U 
                                                      & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                      ? (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1)
                                                      : 
                                                     ((0x00002000U 
                                                       & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                       ? 
                                                      ((0x00001000U 
                                                        & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                        ? 6U
                                                        : 5U)
                                                       : 
                                                      ((0x00001000U 
                                                        & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                        ? 7U
                                                        : 
                                                       (1U 
                                                        & (- (IData)(
                                                                     (1U 
                                                                      & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                                         >> 0x0000001eU)))))))) 
                                                    & (- (IData)(
                                                                 ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_5) 
                                                                  & (0x00000012U 
                                                                     == 
                                                                     (0x00000012U 
                                                                      & vlSelfRef.rv32i_top__DOT__if_id_instr))))))
                                                    : 
                                                   (((0x00004000U 
                                                      & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                      ? (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1)
                                                      : 
                                                     ((0x00002000U 
                                                       & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                       ? 
                                                      ((0x00001000U 
                                                        & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                        ? 6U
                                                        : 5U)
                                                       : 
                                                      (7U 
                                                       & (- (IData)(
                                                                    (1U 
                                                                     & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                                        >> 0x0000000cU))))))) 
                                                    & (- (IData)((IData)(
                                                                         ((0x00000012U 
                                                                           == 
                                                                           (0x00000012U 
                                                                            & vlSelfRef.rv32i_top__DOT__if_id_instr)) 
                                                                          & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_5)))))))));
    vlSelfRef.rv32i_top__DOT__ex_mem_rd = ((IData)(vlSelfRef.rst_n)
                                            ? (IData)(vlSelfRef.rv32i_top__DOT__id_ex_rd)
                                            : 0U);
    vlSelfRef.rv32i_top__DOT__id_ex_rd = ((1U & ((~ (IData)(vlSelfRef.rst_n)) 
                                                 | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))
                                           ? 0U : (0x0000001fU 
                                                   & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                      >> 7U)));
    vlSelfRef.rv32i_top__DOT__id_ex_branch = ((1U & 
                                               (~ (
                                                   (~ (IData)(vlSelfRef.rst_n)) 
                                                   | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                              && (IData)(vlSelfRef.rv32i_top__DOT__id_branch));
    vlSelfRef.rv32i_top__DOT__ex_mem_mem_write = ((IData)(vlSelfRef.rst_n) 
                                                  && (IData)(vlSelfRef.rv32i_top__DOT__id_ex_mem_write));
    vlSelfRef.rv32i_top__DOT__ex_mem_mem_read = ((IData)(vlSelfRef.rst_n) 
                                                 && (IData)(vlSelfRef.rv32i_top__DOT__id_ex_mem_read));
    vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA = ((1U 
                                                 & (~ 
                                                    ((~ (IData)(vlSelfRef.rst_n)) 
                                                     | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                                && (IData)(vlSelfRef.rv32i_top__DOT__id_alu_srcA));
    if (__VdlySet__rv32i_top__DOT__regfile__DOT__registers__v0) {
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[__VdlyDim0__rv32i_top__DOT__regfile__DOT__registers__v0] 
            = __VdlyVal__rv32i_top__DOT__regfile__DOT__registers__v0;
    }
    if (__VdlySet__rv32i_top__DOT__regfile__DOT__registers__v1) {
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[0U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[1U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[2U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[3U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[4U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[5U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[6U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[7U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[8U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[9U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[10U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[11U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[12U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[13U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[14U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[15U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[16U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[17U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[18U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[19U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[20U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[21U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[22U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[23U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[24U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[25U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[26U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[27U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[28U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[29U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[30U] = 0U;
        vlSelfRef.rv32i_top__DOT__regfile__DOT__registers[31U] = 0U;
    }
    vlSelfRef.rv32i_top__DOT__mem_wb_reg_write = ((IData)(vlSelfRef.rst_n) 
                                                  && (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_reg_write));
    vlSelfRef.rv32i_top__DOT__id_ex_mem_write = ((1U 
                                                  & (~ 
                                                     ((~ (IData)(vlSelfRef.rst_n)) 
                                                      | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                                 && (IData)(vlSelfRef.rv32i_top__DOT__id_mem_write));
    vlSelfRef.rv32i_top__DOT__id_ex_mem_read = ((1U 
                                                 & (~ 
                                                    ((~ (IData)(vlSelfRef.rst_n)) 
                                                     | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                                && (IData)(vlSelfRef.rv32i_top__DOT__id_mem_read));
    if ((1U & (~ VL_ONEHOT_I((((IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA) 
                               << 1U) | (1U & (~ (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA)))))))) {
        if ((0U == (((IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA) 
                     << 1U) | (1U & (~ (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:51: Assertion failed in %m: unique case, but none matched for '1'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',1,(IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA));
                VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 51, "");
            }
        } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
            VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:51: Assertion failed in %m: unique case, but multiple matches found for '1'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                         , '#',64,VL_TIME_UNITED_Q(1)
                         , '#',1,(IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA));
            VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 51, "");
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((2U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)) 
                               << 2U) | (((1U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)) 
                                          << 1U) | 
                                         (0U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)))))))) {
        if ((0U != (((2U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)) 
                     << 2U) | (((1U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)) 
                                << 1U) | (0U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:59: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',2,(IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB));
                VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 59, "");
            }
        }
    }
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.rv32i_top__DOT__flush_if_id)))) {
        vlSelfRef.rv32i_top__DOT__if_id_pc = 0U;
        vlSelfRef.rv32i_top__DOT__if_id_pc_plus4 = 0U;
        vlSelfRef.rv32i_top__DOT__if_id_instr = 0x00000013U;
    } else if ((1U & (~ (IData)(vlSelfRef.rv32i_top__DOT__stall_if_id)))) {
        vlSelfRef.rv32i_top__DOT__if_id_pc = vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc;
        vlSelfRef.rv32i_top__DOT__if_id_pc_plus4 = 
            ((IData)(4U) + vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc);
        vlSelfRef.rv32i_top__DOT__if_id_instr = ((IData)(vlSelfRef.rv32i_top__DOT__flush_if_id)
                                                  ? 0x00000013U
                                                  : vlSelfRef.rv32i_top__DOT__if_stage__DOT__imem
                                                 [(0x00001fffU 
                                                   & (vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc 
                                                      >> 2U))]);
    }
    if (vlSelfRef.rst_n) {
        if ((1U & (~ (IData)(vlSelfRef.rv32i_top__DOT__stall_pc)))) {
            vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc 
                = ((IData)(vlSelfRef.rv32i_top__DOT__ex_redirect_en)
                    ? vlSelfRef.rv32i_top__DOT__ex_redirect_pc
                    : vlSelfRef.rv32i_top__DOT__if_pc_plus4);
        }
    } else {
        vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc = 0U;
    }
    vlSelfRef.rv32i_top__DOT__wb_data = ((2U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                          ? ((1U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                              ? vlSelfRef.rv32i_top__DOT__mem_wb_imm_u
                                              : vlSelfRef.rv32i_top__DOT__mem_wb_pc_plus4)
                                          : ((1U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                              ? vlSelfRef.rv32i_top__DOT__mem_wb_mem_rdata
                                              : vlSelfRef.rv32i_top__DOT__mem_wb_alu_result));
    vlSelfRef.rv32i_top__DOT__ex_mem_reg_write = ((IData)(vlSelfRef.rst_n) 
                                                  && (IData)(vlSelfRef.rv32i_top__DOT__id_ex_reg_write));
    vlSelfRef.rv32i_top__DOT__ex_mem_fwd_data = ((2U 
                                                  == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel))
                                                  ? vlSelfRef.rv32i_top__DOT__ex_mem_pc_plus4
                                                  : 
                                                 ((3U 
                                                   == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel))
                                                   ? vlSelfRef.rv32i_top__DOT__ex_mem_imm_u
                                                   : vlSelfRef.rv32i_top__DOT__ex_mem_alu_result));
    vlSelfRef.rv32i_top__DOT__id_ex_reg_write = ((1U 
                                                  & (~ 
                                                     ((~ (IData)(vlSelfRef.rst_n)) 
                                                      | (IData)(vlSelfRef.rv32i_top__DOT__flush_id_ex)))) 
                                                 && (IData)(vlSelfRef.rv32i_top__DOT__id_reg_write));
    vlSelfRef.rv32i_top__DOT__forward_s = 0U;
    if ((((IData)(vlSelfRef.rv32i_top__DOT__ex_mem_reg_write) 
          & (0U != (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd))) 
         & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs2) 
            == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd)))) {
        vlSelfRef.rv32i_top__DOT__forward_s = 2U;
    } else if ((((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
                 & (0U != (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd))) 
                & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs2) 
                   == (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd)))) {
        vlSelfRef.rv32i_top__DOT__forward_s = 1U;
    }
    rv32i_top__DOT__forward_b = 0U;
    if ((0U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB))) {
        if ((((IData)(vlSelfRef.rv32i_top__DOT__ex_mem_reg_write) 
              & (0U != (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd))) 
             & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs2) 
                == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd)))) {
            rv32i_top__DOT__forward_b = 2U;
        } else if ((((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
                     & (0U != (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd))) 
                    & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs2) 
                       == (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd)))) {
            rv32i_top__DOT__forward_b = 1U;
        }
    }
    rv32i_top__DOT__forward_a = 0U;
    if ((((IData)(vlSelfRef.rv32i_top__DOT__ex_mem_reg_write) 
          & (0U != (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd))) 
         & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs1) 
            == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_rd)))) {
        rv32i_top__DOT__forward_a = 2U;
    } else if ((((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
                 & (0U != (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd))) 
                & ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rs1) 
                   == (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd)))) {
        rv32i_top__DOT__forward_a = 1U;
    }
    rv32i_top__DOT__ex_stage__DOT__jal_target = (vlSelfRef.rv32i_top__DOT__id_ex_imm 
                                                 + vlSelfRef.rv32i_top__DOT__id_ex_pc);
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s)) 
                               << 1U) | (2U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s))))))) {
        if ((0U != (((1U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s)) 
                     << 1U) | (2U == (IData)(vlSelfRef.rv32i_top__DOT__forward_s))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:87: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',2,(IData)(vlSelfRef.rv32i_top__DOT__forward_s));
                VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 87, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(rv32i_top__DOT__forward_b)) 
                               << 1U) | (2U == (IData)(rv32i_top__DOT__forward_b))))))) {
        if ((0U != (((1U == (IData)(rv32i_top__DOT__forward_b)) 
                     << 1U) | (2U == (IData)(rv32i_top__DOT__forward_b))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:78: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',2,(IData)(rv32i_top__DOT__forward_b));
                VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 78, "");
            }
        }
    }
    vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd 
        = ((2U == (IData)(rv32i_top__DOT__forward_b))
            ? vlSelfRef.rv32i_top__DOT__ex_mem_fwd_data
            : ((1U == (IData)(rv32i_top__DOT__forward_b))
                ? vlSelfRef.rv32i_top__DOT__wb_data
                : ((0U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB))
                    ? vlSelfRef.rv32i_top__DOT__id_ex_rs2_data
                    : ((1U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB))
                        ? vlSelfRef.rv32i_top__DOT__id_ex_imm
                        : (4U & (- (IData)((2U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcB)))))))));
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(rv32i_top__DOT__forward_a)) 
                               << 1U) | (2U == (IData)(rv32i_top__DOT__forward_a))))))) {
        if ((0U != (((1U == (IData)(rv32i_top__DOT__forward_a)) 
                     << 1U) | (2U == (IData)(rv32i_top__DOT__forward_a))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv32i_ex_stage.sv:69: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.ex_stage", 'T',-12
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',2,(IData)(rv32i_top__DOT__forward_a));
                VL_STOP_MT("outputs/generated_rtl/rv32i_ex_stage.sv", 69, "");
            }
        }
    }
    vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
        = ((2U == (IData)(rv32i_top__DOT__forward_a))
            ? vlSelfRef.rv32i_top__DOT__ex_mem_fwd_data
            : ((1U == (IData)(rv32i_top__DOT__forward_a))
                ? vlSelfRef.rv32i_top__DOT__wb_data
                : ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_alu_srcA)
                    ? vlSelfRef.rv32i_top__DOT__id_ex_pc
                    : vlSelfRef.rv32i_top__DOT__id_ex_rs1_data)));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 = (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                < vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd);
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_7 = VL_LTS_III(32, vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd, vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd);
    rv32i_top__DOT__ex_branch_taken = ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_branch) 
                                       & ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3))
                                           ? ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3))
                                               ? ((1U 
                                                   & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3))
                                                   ? 
                                                  (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                   >= vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)
                                                   : (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6))
                                               : ((1U 
                                                   & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3))
                                                   ? 
                                                  VL_GTES_III(32, vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd, vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)
                                                   : (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_7)))
                                           : ((~ ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3) 
                                                  >> 1U)) 
                                              & ((1U 
                                                  & (IData)(vlSelfRef.rv32i_top__DOT__id_ex_funct3))
                                                  ? 
                                                 (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                  != vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)
                                                  : 
                                                 (vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd 
                                                  == vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opB_fwd)))));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = ((0x00002000U 
                                                 & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                 ? 
                                                ((0x00001000U 
                                                  & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                  ? 2U
                                                  : 3U)
                                                 : 
                                                ((0x00001000U 
                                                  & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                  ? 
                                                 ((0x40000000U 
                                                   & vlSelfRef.rv32i_top__DOT__if_id_instr)
                                                   ? 9U
                                                   : 8U)
                                                  : 4U));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_5 = (IData)(
                                                       (1U 
                                                        == 
                                                        (0x0000000dU 
                                                         & vlSelfRef.rv32i_top__DOT__if_id_instr)));
    vlSelfRef.rv32i_top__DOT__rs1_data = vlSelfRef.rv32i_top__DOT__regfile__DOT__registers
        [(0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                         >> 0x0000000fU))];
    if (((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
         & ((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd) 
            == (0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                               >> 0x0000000fU))))) {
        vlSelfRef.rv32i_top__DOT__rs1_data = vlSelfRef.rv32i_top__DOT__wb_data;
    }
    if ((0U == (0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                               >> 0x0000000fU)))) {
        vlSelfRef.rv32i_top__DOT__rs1_data = 0U;
    }
    vlSelfRef.rv32i_top__DOT__rs2_data = vlSelfRef.rv32i_top__DOT__regfile__DOT__registers
        [(0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                         >> 0x00000014U))];
    if (((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_reg_write) 
         & ((IData)(vlSelfRef.rv32i_top__DOT__mem_wb_rd) 
            == (0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                               >> 0x00000014U))))) {
        vlSelfRef.rv32i_top__DOT__rs2_data = vlSelfRef.rv32i_top__DOT__wb_data;
    }
    if ((0U == (0x0000001fU & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                               >> 0x00000014U)))) {
        vlSelfRef.rv32i_top__DOT__rs2_data = 0U;
    }
    rv32i_top__DOT__id_stage__DOT__imm_i = (((- (IData)(
                                                        (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                         >> 0x0000001fU))) 
                                             << 0x0000000cU) 
                                            | (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                               >> 0x00000014U));
    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
    vlSelfRef.rv32i_top__DOT__id_opcode = (0x0000007fU 
                                           & vlSelfRef.rv32i_top__DOT__if_id_instr);
    if ((0x00000040U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        if ((0x00000020U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            if ((0x00000010U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            } else if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                        if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                            vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                            vlSelfRef.rv32i_top__DOT__id_jump = 1U;
                            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 1U;
                            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                            vlSelfRef.rv32i_top__DOT__id_wb_sel = 2U;
                            vlSelfRef.rv32i_top__DOT__id_imm 
                                = (((- (IData)((vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                >> 0x0000001fU))) 
                                    << 0x00000014U) 
                                   | ((((0x000001feU 
                                         & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                            >> 0x0000000bU)) 
                                        | (1U & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                 >> 0x00000014U))) 
                                       << 0x0000000bU) 
                                      | (0x000007feU 
                                         & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                            >> 0x00000014U))));
                        } else {
                            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                        }
                    } else {
                        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                    }
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                        vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                        vlSelfRef.rv32i_top__DOT__id_jump = 1U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                        vlSelfRef.rv32i_top__DOT__id_wb_sel = 2U;
                        vlSelfRef.rv32i_top__DOT__id_imm 
                            = rv32i_top__DOT__id_stage__DOT__imm_i;
                    } else {
                        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                    }
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 1U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm 
                        = (((- (IData)((vlSelfRef.rv32i_top__DOT__if_id_instr 
                                        >> 0x0000001fU))) 
                            << 0x0000000cU) | ((0x00000800U 
                                                & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                   << 4U)) 
                                               | ((0x000007e0U 
                                                   & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001eU 
                                                     & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                        >> 7U)))));
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            }
        } else {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        }
    } else if ((0x00000020U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        if ((0x00000010U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            } else if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                        vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                        vlSelfRef.rv32i_top__DOT__id_wb_sel = 3U;
                        vlSelfRef.rv32i_top__DOT__id_imm 
                            = (0xfffff000U & vlSelfRef.rv32i_top__DOT__if_id_instr);
                    } else {
                        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                    }
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            }
        } else if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        } else if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 1U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 
                    (((- (IData)((vlSelfRef.rv32i_top__DOT__if_id_instr 
                                  >> 0x0000001fU))) 
                      << 0x0000000cU) | ((0x00000fe0U 
                                          & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                             >> 0x00000014U)) 
                                         | (0x0000001fU 
                                            & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                               >> 7U))));
            } else {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            }
        } else {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        }
    } else if ((0x00000010U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        } else if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 1U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm 
                        = (0xfffff000U & vlSelfRef.rv32i_top__DOT__if_id_instr);
                } else {
                    vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                    vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                    vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                    vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                    vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                    vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                    vlSelfRef.rv32i_top__DOT__id_imm = 0U;
                }
            } else {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            }
        } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = rv32i_top__DOT__id_stage__DOT__imm_i;
            } else {
                vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
                vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
                vlSelfRef.rv32i_top__DOT__id_branch = 0U;
                vlSelfRef.rv32i_top__DOT__id_jump = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
                vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
                vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
                vlSelfRef.rv32i_top__DOT__id_imm = 0U;
            }
        } else {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        }
    } else if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
    } else if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
    } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
        if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__id_opcode))) {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 1U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 1U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 1U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 1U;
            vlSelfRef.rv32i_top__DOT__id_imm = rv32i_top__DOT__id_stage__DOT__imm_i;
        } else {
            vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
            vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
            vlSelfRef.rv32i_top__DOT__id_branch = 0U;
            vlSelfRef.rv32i_top__DOT__id_jump = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
            vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
            vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
            vlSelfRef.rv32i_top__DOT__id_imm = 0U;
        }
    } else {
        vlSelfRef.rv32i_top__DOT__id_reg_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_read = 0U;
        vlSelfRef.rv32i_top__DOT__id_mem_write = 0U;
        vlSelfRef.rv32i_top__DOT__id_branch = 0U;
        vlSelfRef.rv32i_top__DOT__id_jump = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcA = 0U;
        vlSelfRef.rv32i_top__DOT__id_alu_srcB = 0U;
        vlSelfRef.rv32i_top__DOT__id_wb_sel = 0U;
        vlSelfRef.rv32i_top__DOT__id_imm = 0U;
    }
    vlSelfRef.rv32i_top__DOT__ex_redirect_pc = 0U;
    vlSelfRef.rv32i_top__DOT__ex_redirect_en = 0U;
    if (((IData)(vlSelfRef.rv32i_top__DOT__id_ex_jump) 
         & (0x67U == (IData)(vlSelfRef.rv32i_top__DOT__id_ex_opcode)))) {
        vlSelfRef.rv32i_top__DOT__ex_redirect_pc = 
            (0xfffffffeU & (vlSelfRef.rv32i_top__DOT__id_ex_imm 
                            + vlSelfRef.rv32i_top__DOT__ex_stage__DOT__opA_fwd));
        vlSelfRef.rv32i_top__DOT__ex_redirect_en = 1U;
    } else if (vlSelfRef.rv32i_top__DOT__id_ex_jump) {
        vlSelfRef.rv32i_top__DOT__ex_redirect_pc = rv32i_top__DOT__ex_stage__DOT__jal_target;
        vlSelfRef.rv32i_top__DOT__ex_redirect_en = 1U;
    } else if (rv32i_top__DOT__ex_branch_taken) {
        vlSelfRef.rv32i_top__DOT__ex_redirect_pc = rv32i_top__DOT__ex_stage__DOT__jal_target;
        vlSelfRef.rv32i_top__DOT__ex_redirect_en = 1U;
    }
    vlSelfRef.rv32i_top__DOT__if_pc_plus4 = ((IData)(4U) 
                                             + vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc);
    __Vtableidx1 = (((IData)(vlSelfRef.rv32i_top__DOT__ex_redirect_en) 
                     << 1U) | ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_mem_read) 
                               & ((0U != (IData)(vlSelfRef.rv32i_top__DOT__id_ex_rd)) 
                                  & (((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rd) 
                                      == (0x0000001fU 
                                          & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                             >> 0x0000000fU))) 
                                     | ((IData)(vlSelfRef.rv32i_top__DOT__id_ex_rd) 
                                        == (0x0000001fU 
                                            & (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                               >> 0x00000014U)))))));
    vlSelfRef.rv32i_top__DOT__stall_pc = Vrv32i_top__ConstPool__TABLE_h538da069_0
        [__Vtableidx1];
    vlSelfRef.rv32i_top__DOT__stall_if_id = Vrv32i_top__ConstPool__TABLE_h538da069_0
        [__Vtableidx1];
    vlSelfRef.rv32i_top__DOT__flush_if_id = Vrv32i_top__ConstPool__TABLE_h76216006_0
        [__Vtableidx1];
    vlSelfRef.rv32i_top__DOT__flush_id_ex = Vrv32i_top__ConstPool__TABLE_h6746615a_0
        [__Vtableidx1];
}

void Vrv32i_top___024root___nba_comb__TOP__0(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___nba_comb__TOP__0\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ rv32i_top__DOT__mem_stage__DOT__byte_off;
    rv32i_top__DOT__mem_stage__DOT__byte_off = 0;
    IData/*31:0*/ rv32i_top__DOT__mem_stage__DOT__loaded_data;
    rv32i_top__DOT__mem_stage__DOT__loaded_data = 0;
    // Body
    vlSelfRef.rv32i_top__DOT__dmem_wdata = 0U;
    vlSelfRef.rv32i_top__DOT__dmem_byte_en = 0U;
    vlSelfRef.rv32i_top__DOT__dmem_we = 0U;
    vlSelfRef.rv32i_top__DOT__mem_rdata_out = 0U;
    rv32i_top__DOT__mem_stage__DOT__loaded_data = 0U;
    rv32i_top__DOT__mem_stage__DOT__byte_off = (3U 
                                                & vlSelfRef.rv32i_top__DOT__ex_mem_alu_result);
    if (vlSelfRef.rv32i_top__DOT__ex_mem_mem_write) {
        vlSelfRef.rv32i_top__DOT__dmem_we = 1U;
        if ((0U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            vlSelfRef.rv32i_top__DOT__dmem_byte_en 
                = (0x0000000fU & ((IData)(1U) << (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off)));
            vlSelfRef.rv32i_top__DOT__dmem_wdata = 
                VL_SHIFTL_III(32,32,32, vlSelfRef.rv32i_top__DOT__ex_mem_store_data, 
                              VL_SHIFTL_III(32,32,32, (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off), 3U));
        } else if ((1U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            if ((2U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))) {
                vlSelfRef.rv32i_top__DOT__dmem_byte_en = 0x0cU;
                vlSelfRef.rv32i_top__DOT__dmem_wdata 
                    = VL_SHIFTL_III(32,32,32, vlSelfRef.rv32i_top__DOT__ex_mem_store_data, 0x00000010U);
            } else {
                vlSelfRef.rv32i_top__DOT__dmem_byte_en = 3U;
                vlSelfRef.rv32i_top__DOT__dmem_wdata 
                    = vlSelfRef.rv32i_top__DOT__ex_mem_store_data;
            }
        } else if ((2U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            vlSelfRef.rv32i_top__DOT__dmem_byte_en = 0x0fU;
            vlSelfRef.rv32i_top__DOT__dmem_wdata = vlSelfRef.rv32i_top__DOT__ex_mem_store_data;
        } else {
            vlSelfRef.rv32i_top__DOT__dmem_byte_en = 0U;
            vlSelfRef.rv32i_top__DOT__dmem_wdata = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I((((2U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)) 
                                   << 2U) | (((1U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)) 
                                              << 1U) 
                                             | (0U 
                                                == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)))))))) {
            if ((0U != (((2U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)) 
                         << 2U) | (((1U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)) 
                                    << 1U) | (0U == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3)))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv32i_mem_stage.sv:65: Assertion failed in %m: unique case, but multiple matches found for '3'h%X'\n",4, 'M',vlSymsp->name(),"rv32i_top.mem_stage", 'T',-12
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',3,(IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3));
                    VL_STOP_MT("outputs/generated_rtl/rv32i_mem_stage.sv", 65, "");
                }
            }
        }
    }
    if (vlSelfRef.rv32i_top__DOT__ex_mem_mem_read) {
        if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
                vlSelfRef.rv32i_top__DOT__mem_rdata_out = 0U;
            } else if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
                rv32i_top__DOT__mem_stage__DOT__loaded_data 
                    = ((2U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                        ? (vlSelfRef.rv32i_top__DOT__dmem
                           [(0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                            >> 2U))] 
                           >> 0x10U) : (0x0000ffffU 
                                        & vlSelfRef.rv32i_top__DOT__dmem
                                        [(0x00001fffU 
                                          & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                             >> 2U))]));
                vlSelfRef.rv32i_top__DOT__mem_rdata_out 
                    = rv32i_top__DOT__mem_stage__DOT__loaded_data;
            } else {
                rv32i_top__DOT__mem_stage__DOT__loaded_data 
                    = ((2U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                        ? ((1U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                            ? (vlSelfRef.rv32i_top__DOT__dmem
                               [(0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                >> 2U))] 
                               >> 0x18U) : (0x000000ffU 
                                            & (vlSelfRef.rv32i_top__DOT__dmem
                                               [(0x00001fffU 
                                                 & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                    >> 2U))] 
                                               >> 0x10U)))
                        : ((1U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                            ? (0x000000ffU & (vlSelfRef.rv32i_top__DOT__dmem
                                              [(0x00001fffU 
                                                & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                   >> 2U))] 
                                              >> 8U))
                            : (0x000000ffU & vlSelfRef.rv32i_top__DOT__dmem
                               [(0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                >> 2U))])));
                vlSelfRef.rv32i_top__DOT__mem_rdata_out 
                    = rv32i_top__DOT__mem_stage__DOT__loaded_data;
            }
        } else if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            vlSelfRef.rv32i_top__DOT__mem_rdata_out 
                = ((1U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))
                    ? 0U : vlSelfRef.rv32i_top__DOT__dmem
                   [(0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                    >> 2U))]);
        } else if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_funct3))) {
            rv32i_top__DOT__mem_stage__DOT__loaded_data 
                = ((2U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                    ? (((- (IData)((vlSelfRef.rv32i_top__DOT__dmem
                                    [(0x00001fffU & 
                                      (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                       >> 2U))] >> 0x1fU))) 
                        << 0x00000010U) | (vlSelfRef.rv32i_top__DOT__dmem
                                           [(0x00001fffU 
                                             & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                >> 2U))] 
                                           >> 0x10U))
                    : (((- (IData)((1U & (vlSelfRef.rv32i_top__DOT__dmem
                                          [(0x00001fffU 
                                            & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                               >> 2U))] 
                                          >> 0x0fU)))) 
                        << 0x00000010U) | (0x0000ffffU 
                                           & vlSelfRef.rv32i_top__DOT__dmem
                                           [(0x00001fffU 
                                             & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                >> 2U))])));
            vlSelfRef.rv32i_top__DOT__mem_rdata_out 
                = rv32i_top__DOT__mem_stage__DOT__loaded_data;
        } else {
            rv32i_top__DOT__mem_stage__DOT__loaded_data 
                = ((2U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                    ? ((1U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                        ? (((- (IData)((vlSelfRef.rv32i_top__DOT__dmem
                                        [(0x00001fffU 
                                          & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                             >> 2U))] 
                                        >> 0x1fU))) 
                            << 8U) | (vlSelfRef.rv32i_top__DOT__dmem
                                      [(0x00001fffU 
                                        & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                           >> 2U))] 
                                      >> 0x18U)) : 
                       (((- (IData)((1U & (vlSelfRef.rv32i_top__DOT__dmem
                                           [(0x00001fffU 
                                             & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                >> 2U))] 
                                           >> 0x17U)))) 
                         << 8U) | (0x000000ffU & (vlSelfRef.rv32i_top__DOT__dmem
                                                  [
                                                  (0x00001fffU 
                                                   & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                      >> 2U))] 
                                                  >> 0x10U))))
                    : ((1U & (IData)(rv32i_top__DOT__mem_stage__DOT__byte_off))
                        ? (((- (IData)((1U & (vlSelfRef.rv32i_top__DOT__dmem
                                              [(0x00001fffU 
                                                & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                   >> 2U))] 
                                              >> 0x0fU)))) 
                            << 8U) | (0x000000ffU & 
                                      (vlSelfRef.rv32i_top__DOT__dmem
                                       [(0x00001fffU 
                                         & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                            >> 2U))] 
                                       >> 8U))) : (
                                                   ((- (IData)(
                                                               (1U 
                                                                & (vlSelfRef.rv32i_top__DOT__dmem
                                                                   [
                                                                   (0x00001fffU 
                                                                    & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                                       >> 2U))] 
                                                                   >> 7U)))) 
                                                    << 8U) 
                                                   | (0x000000ffU 
                                                      & vlSelfRef.rv32i_top__DOT__dmem
                                                      [
                                                      (0x00001fffU 
                                                       & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                                          >> 2U))]))));
            vlSelfRef.rv32i_top__DOT__mem_rdata_out 
                = rv32i_top__DOT__mem_stage__DOT__loaded_data;
        }
    }
}

void Vrv32i_top___024root___eval_nba(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_nba\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v0;
    __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v0 = 0;
    SData/*12:0*/ __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v0;
    __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v0 = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v0;
    __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v0 = 0;
    CData/*7:0*/ __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v1;
    __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v1 = 0;
    SData/*12:0*/ __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v1;
    __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v1 = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v1;
    __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v1 = 0;
    CData/*7:0*/ __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v2;
    __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v2 = 0;
    SData/*12:0*/ __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v2;
    __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v2 = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v2;
    __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v2 = 0;
    CData/*7:0*/ __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v3;
    __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v3 = 0;
    SData/*12:0*/ __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v3;
    __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v3 = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v3;
    __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v3 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v0 = 0U;
        __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v1 = 0U;
        __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v2 = 0U;
        __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v3 = 0U;
        if (vlSelfRef.rv32i_top__DOT__dmem_we) {
            if ((1U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
                __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v0 
                    = (0x000000ffU & vlSelfRef.rv32i_top__DOT__dmem_wdata);
                __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v0 
                    = (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                      >> 2U));
                __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v0 = 1U;
            }
            if ((2U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
                __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v1 
                    = (0x000000ffU & (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                                      >> 8U));
                __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v1 
                    = (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                      >> 2U));
                __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v1 = 1U;
            }
            if ((4U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
                __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v2 
                    = (0x000000ffU & (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                                      >> 0x10U));
                __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v2 
                    = (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                      >> 2U));
                __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v2 = 1U;
            }
            if ((8U & (IData)(vlSelfRef.rv32i_top__DOT__dmem_byte_en))) {
                __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v3 
                    = (vlSelfRef.rv32i_top__DOT__dmem_wdata 
                       >> 0x18U);
                __Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v3 
                    = (0x00001fffU & (vlSelfRef.rv32i_top__DOT__ex_mem_alu_result 
                                      >> 2U));
                __Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v3 = 1U;
            }
        }
        if (__Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v0) {
            vlSelfRef.rv32i_top__DOT__dmem[__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v0] 
                = ((0xffffff00U & vlSelfRef.rv32i_top__DOT__dmem
                    [__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v0]) 
                   | __Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v0);
        }
        if (__Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v1) {
            vlSelfRef.rv32i_top__DOT__dmem[__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v1] 
                = ((0xffff00ffU & vlSelfRef.rv32i_top__DOT__dmem
                    [__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v1]) 
                   | ((IData)(__Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v1) 
                      << 8U));
        }
        if (__Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v2) {
            vlSelfRef.rv32i_top__DOT__dmem[__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v2] 
                = ((0xff00ffffU & vlSelfRef.rv32i_top__DOT__dmem
                    [__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v2]) 
                   | ((IData)(__Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v2) 
                      << 0x00000010U));
        }
        if (__Vinline__nba_sequent__TOP__0___VdlySet__rv32i_top__DOT__dmem__v3) {
            vlSelfRef.rv32i_top__DOT__dmem[__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v3] 
                = ((0x00ffffffU & vlSelfRef.rv32i_top__DOT__dmem
                    [__Vinline__nba_sequent__TOP__0___VdlyDim0__rv32i_top__DOT__dmem__v3]) 
                   | ((IData)(__Vinline__nba_sequent__TOP__0___VdlyVal__rv32i_top__DOT__dmem__v3) 
                      << 0x00000018U));
        }
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vrv32i_top___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vrv32i_top___024root___nba_comb__TOP__0(vlSelf);
    }
}

void Vrv32i_top___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrv32i_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vrv32i_top___024root___eval_phase__act(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_phase__act\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vrv32i_top___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vrv32i_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vrv32i_top___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vrv32i_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vrv32i_top___024root___eval_phase__nba(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_phase__nba\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vrv32i_top___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vrv32i_top___024root___eval_nba(vlSelf);
        Vrv32i_top___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vrv32i_top___024root___eval(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vrv32i_top___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("outputs/generated_rtl/rv32i_top.sv", 1, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vrv32i_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("outputs/generated_rtl/rv32i_top.sv", 1, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vrv32i_top___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vrv32i_top___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vrv32i_top___024root___eval_debug_assertions(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_debug_assertions\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
}
#endif  // VL_DEBUG
