// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VRV32I_TOP__SYMS_H_
#define VERILATED_VRV32I_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vrv32i_top.h"

// INCLUDE MODULE CLASSES
#include "Vrv32i_top___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vrv32i_top__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vrv32i_top* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vrv32i_top___024root           TOP;

    // CONSTRUCTORS
    Vrv32i_top__Syms(VerilatedContext* contextp, const char* namep, Vrv32i_top* modelp);
    ~Vrv32i_top__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
