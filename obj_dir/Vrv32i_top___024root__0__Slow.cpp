// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrv32i_top.h for the primary calling header

#include "Vrv32i_top__pch.h"

VL_ATTR_COLD void Vrv32i_top___024root___eval_static(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_static\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file = ""s;
    vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file = ""s;
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vrv32i_top___024root___eval_static__TOP(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_static__TOP\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file = ""s;
    vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file = ""s;
}

VL_ATTR_COLD void Vrv32i_top___024root___eval_initial(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_initial\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__eval_initial__TOP_rv32i_top__DOT____VlemExpr_0;
    IData/*31:0*/ __Vinline__eval_initial__TOP_rv32i_top__DOT__if_stage__DOT____VlemExpr_0;
    // Body
    __Vinline__eval_initial__TOP_rv32i_top__DOT____VlemExpr_0 
        = VL_VALUEPLUSARGS_INN(64, "load=%s"s, vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file);
    if (VL_UNLIKELY((__Vinline__eval_initial__TOP_rv32i_top__DOT____VlemExpr_0))) {
        VL_READMEM_N(true, 32, 8192, 0, vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file
                     ,  &(vlSelfRef.rv32i_top__DOT__dmem)
                     , 0, ~0ULL);
    }
    vlSelfRef.rv32i_top__DOT__dmem[1024U] = 0U;
    __Vinline__eval_initial__TOP_rv32i_top__DOT__if_stage__DOT____VlemExpr_0 
        = VL_VALUEPLUSARGS_INN(64, "load=%s"s, vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file);
    if (VL_UNLIKELY((__Vinline__eval_initial__TOP_rv32i_top__DOT__if_stage__DOT____VlemExpr_0))) {
        VL_READMEM_N(true, 32, 8192, 0, vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file
                     ,  &(vlSelfRef.rv32i_top__DOT__if_stage__DOT__imem)
                     , 0, ~0ULL);
    }
}

VL_ATTR_COLD void Vrv32i_top___024root___eval_initial__TOP(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_initial__TOP\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ rv32i_top__DOT____VlemExpr_0;
    IData/*31:0*/ rv32i_top__DOT__if_stage__DOT____VlemExpr_0;
    // Body
    rv32i_top__DOT____VlemExpr_0 = VL_VALUEPLUSARGS_INN(64, "load=%s"s, 
                                                        vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file);
    if (VL_UNLIKELY((rv32i_top__DOT____VlemExpr_0))) {
        VL_READMEM_N(true, 32, 8192, 0, vlSelfRef.rv32i_top__DOT__unnamedblk1__DOT__load_file
                     ,  &(vlSelfRef.rv32i_top__DOT__dmem)
                     , 0, ~0ULL);
    }
    vlSelfRef.rv32i_top__DOT__dmem[1024U] = 0U;
    rv32i_top__DOT__if_stage__DOT____VlemExpr_0 = VL_VALUEPLUSARGS_INN(64, "load=%s"s, 
                                                                       vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file);
    if (VL_UNLIKELY((rv32i_top__DOT__if_stage__DOT____VlemExpr_0))) {
        VL_READMEM_N(true, 32, 8192, 0, vlSelfRef.rv32i_top__DOT__if_stage__DOT__unnamedblk1__DOT__load_file
                     ,  &(vlSelfRef.rv32i_top__DOT__if_stage__DOT__imem)
                     , 0, ~0ULL);
    }
}

VL_ATTR_COLD void Vrv32i_top___024root___eval_final(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_final\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrv32i_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vrv32i_top___024root___eval_phase__stl(Vrv32i_top___024root* vlSelf);

VL_ATTR_COLD void Vrv32i_top___024root___eval_settle(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_settle\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vrv32i_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("outputs/generated_rtl/rv32i_top.sv", 1, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vrv32i_top___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vrv32i_top___024root___eval_triggers_vec__stl(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_triggers_vec__stl\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vrv32i_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrv32i_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vrv32i_top___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vrv32i_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___trigger_anySet__stl\n"); );
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

extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h538da069_0;
extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h76216006_0;
extern const VlUnpacked<CData/*0:0*/, 4> Vrv32i_top__ConstPool__TABLE_h6746615a_0;

VL_ATTR_COLD void Vrv32i_top___024root___stl_sequent__TOP__0(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___stl_sequent__TOP__0\n"); );
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
    CData/*1:0*/ rv32i_top__DOT__mem_stage__DOT__byte_off;
    rv32i_top__DOT__mem_stage__DOT__byte_off = 0;
    IData/*31:0*/ rv32i_top__DOT__mem_stage__DOT__loaded_data;
    rv32i_top__DOT__mem_stage__DOT__loaded_data = 0;
    CData/*1:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
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
    vlSelfRef.rv32i_top__DOT__if_pc_plus4 = ((IData)(4U) 
                                             + vlSelfRef.rv32i_top__DOT__if_stage__DOT__pc);
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
    rv32i_top__DOT__ex_stage__DOT__jal_target = (vlSelfRef.rv32i_top__DOT__id_ex_imm 
                                                 + vlSelfRef.rv32i_top__DOT__id_ex_pc);
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
    rv32i_top__DOT__id_stage__DOT__imm_i = (((- (IData)(
                                                        (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                                         >> 0x0000001fU))) 
                                             << 0x0000000cU) 
                                            | (vlSelfRef.rv32i_top__DOT__if_id_instr 
                                               >> 0x00000014U));
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
    vlSelfRef.rv32i_top__DOT__ex_mem_fwd_data = ((2U 
                                                  == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel))
                                                  ? vlSelfRef.rv32i_top__DOT__ex_mem_pc_plus4
                                                  : 
                                                 ((3U 
                                                   == (IData)(vlSelfRef.rv32i_top__DOT__ex_mem_wb_sel))
                                                   ? vlSelfRef.rv32i_top__DOT__ex_mem_imm_u
                                                   : vlSelfRef.rv32i_top__DOT__ex_mem_alu_result));
    vlSelfRef.rv32i_top__DOT__wb_data = ((2U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                          ? ((1U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                              ? vlSelfRef.rv32i_top__DOT__mem_wb_imm_u
                                              : vlSelfRef.rv32i_top__DOT__mem_wb_pc_plus4)
                                          : ((1U & (IData)(vlSelfRef.rv32i_top__DOT__mem_wb_wb_sel))
                                              ? vlSelfRef.rv32i_top__DOT__mem_wb_mem_rdata
                                              : vlSelfRef.rv32i_top__DOT__mem_wb_alu_result));
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

VL_ATTR_COLD void Vrv32i_top___024root___eval_stl(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_stl\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vrv32i_top___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vrv32i_top___024root___eval_phase__stl(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___eval_phase__stl\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vrv32i_top___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vrv32i_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vrv32i_top___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vrv32i_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vrv32i_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrv32i_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vrv32i_top___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vrv32i_top___024root___ctor_var_reset(Vrv32i_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrv32i_top___024root___ctor_var_reset\n"); );
    Vrv32i_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->rv32i_top__DOT__if_pc_plus4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14490469742831719791ull);
    vlSelf->rv32i_top__DOT__if_id_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 459023400572325343ull);
    vlSelf->rv32i_top__DOT__if_id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13508189161178898508ull);
    vlSelf->rv32i_top__DOT__if_id_pc_plus4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5866020688005842202ull);
    vlSelf->rv32i_top__DOT__rs1_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14790687110368136455ull);
    vlSelf->rv32i_top__DOT__rs2_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10135093663550080588ull);
    vlSelf->rv32i_top__DOT__id_opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 8165110588616791441ull);
    vlSelf->rv32i_top__DOT__id_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5348463933181359854ull);
    vlSelf->rv32i_top__DOT__id_reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11651555775641111749ull);
    vlSelf->rv32i_top__DOT__id_mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7758707078432208350ull);
    vlSelf->rv32i_top__DOT__id_mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13759160715925028561ull);
    vlSelf->rv32i_top__DOT__id_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5634611057024919764ull);
    vlSelf->rv32i_top__DOT__id_jump = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2368102967952442823ull);
    vlSelf->rv32i_top__DOT__id_alu_srcA = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2764530087326271589ull);
    vlSelf->rv32i_top__DOT__id_alu_srcB = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6533742484932636979ull);
    vlSelf->rv32i_top__DOT__id_wb_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7061929421092729794ull);
    vlSelf->rv32i_top__DOT__id_ex_reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7754798026620127284ull);
    vlSelf->rv32i_top__DOT__id_ex_mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6200992188161335866ull);
    vlSelf->rv32i_top__DOT__id_ex_mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9394373528232183637ull);
    vlSelf->rv32i_top__DOT__id_ex_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17140310390876163935ull);
    vlSelf->rv32i_top__DOT__id_ex_jump = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6751479452073690001ull);
    vlSelf->rv32i_top__DOT__id_ex_alu_srcA = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5947029197856555154ull);
    vlSelf->rv32i_top__DOT__id_ex_alu_srcB = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16915713105403293631ull);
    vlSelf->rv32i_top__DOT__id_ex_wb_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7636285963177781505ull);
    vlSelf->rv32i_top__DOT__id_ex_alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10222784371525659044ull);
    vlSelf->rv32i_top__DOT__id_ex_rs1_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2398255446375749241ull);
    vlSelf->rv32i_top__DOT__id_ex_rs2_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1243416048948295398ull);
    vlSelf->rv32i_top__DOT__id_ex_store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2736844232586899624ull);
    vlSelf->rv32i_top__DOT__id_ex_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1004347572670462731ull);
    vlSelf->rv32i_top__DOT__id_ex_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5384332275286638927ull);
    vlSelf->rv32i_top__DOT__id_ex_pc_plus4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13706321696246765805ull);
    vlSelf->rv32i_top__DOT__id_ex_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 11151496394219800664ull);
    vlSelf->rv32i_top__DOT__id_ex_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1077936583145081199ull);
    vlSelf->rv32i_top__DOT__id_ex_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9576589172134090460ull);
    vlSelf->rv32i_top__DOT__id_ex_funct3 = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5255129771545564251ull);
    vlSelf->rv32i_top__DOT__id_ex_opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6902739328580229969ull);
    vlSelf->rv32i_top__DOT__forward_s = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7745339502001446769ull);
    vlSelf->rv32i_top__DOT__ex_redirect_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11041729145814625684ull);
    vlSelf->rv32i_top__DOT__ex_redirect_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14201732372800117752ull);
    vlSelf->rv32i_top__DOT__stall_pc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2392190021331288891ull);
    vlSelf->rv32i_top__DOT__stall_if_id = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5132906623273276410ull);
    vlSelf->rv32i_top__DOT__flush_if_id = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2285566338648689520ull);
    vlSelf->rv32i_top__DOT__flush_id_ex = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4654517548798919978ull);
    vlSelf->rv32i_top__DOT__ex_mem_reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13801985691258173986ull);
    vlSelf->rv32i_top__DOT__ex_mem_mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8973472385419741484ull);
    vlSelf->rv32i_top__DOT__ex_mem_mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3002203719085768100ull);
    vlSelf->rv32i_top__DOT__ex_mem_wb_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12113514294307400438ull);
    vlSelf->rv32i_top__DOT__ex_mem_alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15963544530216871376ull);
    vlSelf->rv32i_top__DOT__ex_mem_store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15980707226790131123ull);
    vlSelf->rv32i_top__DOT__ex_mem_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5848981347223551047ull);
    vlSelf->rv32i_top__DOT__ex_mem_funct3 = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1969893034477438555ull);
    vlSelf->rv32i_top__DOT__ex_mem_pc_plus4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1250881215253596942ull);
    vlSelf->rv32i_top__DOT__ex_mem_imm_u = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9446376983237946362ull);
    vlSelf->rv32i_top__DOT__ex_mem_fwd_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9600325877450322179ull);
    vlSelf->rv32i_top__DOT__dmem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1190673261485143149ull);
    vlSelf->rv32i_top__DOT__dmem_byte_en = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9395178638577477478ull);
    vlSelf->rv32i_top__DOT__dmem_we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13995679873868670517ull);
    vlSelf->rv32i_top__DOT__mem_rdata_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11744213681452209591ull);
    vlSelf->rv32i_top__DOT__mem_wb_reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11585963396381950476ull);
    vlSelf->rv32i_top__DOT__mem_wb_wb_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7125249378300038261ull);
    vlSelf->rv32i_top__DOT__mem_wb_alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10102233718027954002ull);
    vlSelf->rv32i_top__DOT__mem_wb_mem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3943625225768103788ull);
    vlSelf->rv32i_top__DOT__mem_wb_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 18253348922277558375ull);
    vlSelf->rv32i_top__DOT__mem_wb_pc_plus4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15245585927459832376ull);
    vlSelf->rv32i_top__DOT__mem_wb_imm_u = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7708652096631933470ull);
    vlSelf->rv32i_top__DOT__wb_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7239581206698866043ull);
    for (int __Vi0 = 0; __Vi0 < 8192; ++__Vi0) {
        vlSelf->rv32i_top__DOT__dmem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9988177559576866646ull);
    }
    for (int __Vi0 = 0; __Vi0 < 8192; ++__Vi0) {
        vlSelf->rv32i_top__DOT__if_stage__DOT__imem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18117167879419819735ull);
    }
    vlSelf->rv32i_top__DOT__if_stage__DOT__pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11218167436396508844ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->rv32i_top__DOT__regfile__DOT__registers[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11654586704313580787ull);
    }
    vlSelf->rv32i_top__DOT__ex_stage__DOT__opA_fwd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15074477367580909894ull);
    vlSelf->rv32i_top__DOT__ex_stage__DOT__opB_fwd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 639601641820385710ull);
    vlSelf->__VdfgRegularize_h6e95ff9d_0_1 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_5 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_6 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_7 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
