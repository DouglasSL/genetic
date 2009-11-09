/*
 * file.cpp
 * Project - Description
 *
 * Copyright (c) 2009 Tim Besard <tim.besard@gmail.com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "simple.h"
#include "../parser.h"
#include "../../dna.h"





//////////
// MAIN //
//////////

int main() {
    try {
        //
        // Grammar setup
        //

        // Create grammar
        SimpleGrammar tGrammar;
        tGrammar.setup();


        //
        // Code creation
        //

        DNA tDNA({
            INSTR_OPEN,
                OTHER_PRINT,
                    ARG_OPEN,
                        MATH_PLUS,
                            ARG_OPEN,
                                DATA_INT, 1,
                            ARG_SEP,
                                DATA_INT, 2,
                            ARG_CLOSE,
                    ARG_CLOSE,
            INSTR_CLOSE,
            0,
            INSTR_OPEN,
            COND_IF,
                ARG_OPEN,
                    TEST_GREATER,
                        ARG_OPEN,
                            DATA_INT, 2,
                        ARG_SEP,
                            DATA_INT, 1,
                        ARG_CLOSE,
                ARG_CLOSE,
                INSTR_OPEN,
                    OTHER_PRINT,
                        ARG_OPEN,
                            DATA_INT, 2,
                        ARG_CLOSE,
                INSTR_SEP,
                    OTHER_PRINT,
                        ARG_OPEN,
                            DATA_INT, 1,
                        ARG_CLOSE,
                INSTR_CLOSE,
            INSTR_CLOSE,
        });
        
        
        //
        // Parser setup
        //
        
        Parser tParser(&tGrammar);


        //
        // Code execution
        //

        tParser.execute(tDNA);


    }
    catch (Exception e) {
        std::cout << e << std::endl;
    }
}
