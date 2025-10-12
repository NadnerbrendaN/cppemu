/*  Copyright (C) 2025 Luna Dock <lunadock@protonmail.com> 
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

int registers[16];
bool flags[4];

int add(bool s, short cond, int* rn, int* op2) {
    flags[3] = ((*rn < 0) == (*op2 < 0) && (b > 0 && a > std::numeric_limits<int_type>::max() - b
                || b < 0 && a < std::numeric_limits<int_type>::min() - b));
    int res = 0;
    if (!flags[3]) {
        res = *rn + *op2;
    } else {
        
    }
    if (s) {
        flags[0] = res < 0;
        flags[1] = res == 0;
    }
}
