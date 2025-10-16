/*
 * Copyright (C) 2025 Luna Dock <lunadock@protonmail.com> 
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <limits>
#include <iostream>

unsigned int registers[16]; // 16 32-bit registers
bool N = false; // Negative
bool Z = false; // Zero
bool C = false; // Carry
bool V = false; // oVerflow

bool check(short cond) { // check if the given condition should execute based on the status of our flags
    switch (cond) {
        case 0: // AL -- always execute
            return true;
        case 1: // EQ -- equal to 0
            return Z;
        case 2: // NE -- not equal to 0
            return !Z;
        case 3: // CS or HS -- C set / Higher or same
            return C;
        case 4: // CC or LO -- C clear / Lower
            return !C;
        case 5: // MI -- Minus
            return N;
        case 6: // PL -- Plus
            return !N;
        case 7: // VS -- V set
            return V;
        case 8: // VC -- V clear
            return !V;
        case 9: // HI -- Higher
            return C && !Z;
        case 10: // LS -- Lower or same
            return !C || Z;
        case 11: // GE -- signed >=
            return N == V;
        case 12: // LT -- signed <
            return N != V;
        case 13: // GT -- signed >
            return N == V && !Z;
        case 14: // LE -- signed <=
            return N != V && Z;
        default:
            return true;
    }
}

unsigned int add(bool s, unsigned int* rn, unsigned int* op2) { // ADD
    int res = 0;
    res = *rn + *op2; // add the values -- overflow is defined because it's unsigned
    if (s) { // should we set the condition flags?
        N = res < 0;
        Z = res == 0;
        C = (*op2 > 0 && *rn > std::numeric_limits<unsigned int>::max() - *op2);
        V = C;
    }
    return res;
}

int main() {
    registers[0] = 255;
    registers[1] = 1;
    std::cout << add(false, &registers[0], &registers[1]);

    return 0;
}
