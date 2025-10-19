/*
 * Copyright (C) 2025 Luna Dock <lunadock@protonmail.com> 
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <limits>
#include <cstdint>
#include <iostream>

struct State {
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

void add(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) {
        return;
    }
    std::uint32_t res = state.reg[rn] + state.reg[rm];
    if (s) { // set condition flags?
        state.N = (res & 0x80000000) != 0; // check most significant bit
        state.Z = res == 0;
        state.C = (state.reg[rm] > 0
                && state.reg[rn] > std::numeric_limits<std::uint32_t>::max() - state.reg[rm]);
        state.V = state.C;
    }
    state.reg[rd] = res;
}

void adc(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) { // conditional execution
        return;
    }
    if (rd == 15 || rn == 15 || rm == 15) { // avoid using PC
        return;
    }
    std::uint32_t res = state.reg[rn] + state.reg[rm];
    if (state.C) {
        ++res;
    }
    if (s) {
        state.N = (res & 0x80000000) != 0;
        state.Z = res == 0;
        state.C = (state.reg[rn] >
                (std::numeric_limits<std::uint32_t>::max() - state.reg[rm] - (state.C? 1:0)));
        state.V = state.C;
    }
    state.reg[rd] = res;
}

void and(State& state, bool s, std::uint8_t cond, int rd, int rn, int rm) {
    if (!check(state, cond)) {
        return;
    }
    std::uint32_t res = (state.reg[rn] + rn==15?8:0) & state.reg[rm];
    if (s) {
        state.N = (res & 0x80000000) != 0;
        state.Z = res == 0;
        state.V = false;
    }
}

int main() {
    State state = {0};
    state.reg[0] = 0;
    state.reg[1] = 0x80000000;
    add(state, true, 14, 2, 0, 1);
    std::cout << state.reg[2] << "\n";

    add(state, false, 4, 3, 0, 1);
    std::cout << state.reg[3] << "\n";

    return 0;
}
