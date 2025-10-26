/*
 * Copyright (C) 2025 NadnerbrendaN <albertdock@duck.com>
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <limits>
#include <cstdint>
#include <iostream>

struct State { // current state of the CPU, etc
    std::uint32_t reg[16]; // 16 32-bit registers
    bool N = false; // Negative
    bool Z = false; // Zero
    bool C = false; // Carry
    bool V = false; // oVerflow
};

bool check(State& state, std::uint8_t cond) { // check if the given condition should execute based on the status of flags
    switch (cond) {
        case 0: // EQ -- equal to 0
            return state.Z;
        case 1: // NE -- not equal to 0
            return !state.Z;
        case 2: // CS or HS -- C set / Higher or same
            return state.C;
        case 3: // CC or LO -- C clear / Lower
            return !state.C;
        case 4: // MI -- Minus
            return state.N;
        case 5: // PL -- Plus
            return !state.N;
        case 6: // VS -- V set
            return state.V;
        case 7: // VC -- V clear
            return !state.V;
        case 8: // HI -- Higher
            return state.C && !state.Z;
        case 9: // LS -- Lower or same
            return !state.C || state.Z;
        case 10: // GE -- signed >=
            return state.N == state.V;
        case 11: // LT -- signed <
            return state.N != state.V;
        case 12: // GT -- signed >
            return state.N == state.V && !state.Z;
        case 13: // LE -- signed <=
            return state.N != state.V && state.Z;
        case 14: // AL -- always
            return true;
        default:
            return false;
    }
}

void ins_add(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) { // if we shouldn't execute
        return;
    }
    std::uint32_t res = state.reg[rn] + state.reg[rm];
    if (rn == 15 || rm == 15) { // add 8 if either is PC -- TODO: confirm that's right
        res += 8;
    }
    if (s) { // set condition flags?
        state.N = (res & 0x80000000) != 0; // check most significant bit
        state.Z = res == 0;
        state.C = (res < state.reg[rn] || res < state.reg[rm]); // carry -> if result of addition is less than one or more of the operands, we overflowed and thus carry
        state.V = state.C;
    }
    state.reg[rd] = res;
}

void ins_adc(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) {
        return;
    }
    if (rd == 15 || rn == 15 || rm == 15) { // avoid using PC
        return;
    }
    std::uint32_t res = state.reg[rn] + state.reg[rm];
    if (state.C) { // carry if carry flag is set
        ++res;
    }
    if (s) {
        state.N = (res & 0x80000000) != 0;
        state.Z = res == 0;
        state.C = (state.reg[rn] >
                (std::numeric_limits<std::uint32_t>::max() - state.reg[rm] - (state.C? 1:0)));
        // check if one register is greater than the difference between the 32-bit limit
        // and the other register, accounting for carry if necessary
        state.V = state.C;
    }
    state.reg[rd] = res;
}

void ins_and(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) {
        return;
    }
    std::uint32_t res = state.reg[rn] & state.reg[rm]; // bitwise and
    if (s) {
        state.N = (res & 0x80000000) != 0;
        state.Z = res == 0;
        state.V = false;
        // carry is unchanged, while overflow is set to false
    }
    state.reg[rd] = res;
}

void ins_mov(State& state, bool s, std::uint8_t cond, int rd, int rn) {
    if (!check(state, cond)) {
        return;
    }
    if (s) {
        state.N = (state.reg[rn] & 0x80000000) != 0;
        state.Z = state.reg[rn] == 0;
        // neither carry nor overflow are changed
    }
    state.reg[rd] = state.reg[rn]; // rd <- rn
}

void ins_mov(State& state, std::uint8_t cond, int rd, std::uint32_t value) {
    if (!check(state, cond)) {
        return;
    }
    // no need to set flags, as the value is known
    state.reg[rd] = value; // rd <- given immediate value
}

int main() {
    State state = {0}; // initialize all parts of state to default values (0, false, etc)
    std::uint32_t len = 1; // # of instructions
    std::uint32_t instructions[len] = {
        0b00000000100100000000000000010001
    };

    while (state.reg[15] < len) { // ensure PC does not go over # of instructions
        //std::cout << state.reg[15] << "\n";
        std::uint32_t ins = instructions[0]; // isolate current instruction for easy later use
        //std::cout << ins << "\n";
        std::uint8_t cond = (ins >> 28) & 0b1111; // isolate bits 28-31, which are the condition
        //std::cout << (int)(cond) << "\n";
        if (cond != 0b1111 && (ins >> 26 & 0b11) == 0) { // data processing instructions (bits 26&27 are 00)
            if ((ins >> 26 & 0b1) == 0) { // check op0 bit 1 in general instructions
                if ((ins >> 4 & 0b1) == 0) {// check op4 in general instructions
                    if ((ins >> 24 & 0b1) == 0) {// check op0 bit 1 in DP
                    }
                }
            }
        }

        ++state.reg[15]; // increment PC
    }

    return 0;
}
