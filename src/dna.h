/*
 * dna.h
 * Evolve - DNA datastructure.
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

// Include guard
#ifndef __DNA
#define __DNA

// Headers
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include <initializer_list>


//
// Constants
//


//////////////////////
// CLASS DEFINITION //
//////////////////////

class DNA
{
	public:
		// Construction and destruction
		DNA(const DNA& inputData);
		DNA(const std::initializer_list<unsigned char>);
                DNA(const unsigned char* inputData, int inputSize);
                ~DNA();

		// Informational routines
		unsigned int genes() const;
		unsigned int length() const;

                // Raw modifiers
                void erase(unsigned int i_start, unsigned int i_end);
                void insert(unsigned int i_start, unsigned char* gene, unsigned int size);
                void replace(unsigned int i_start, unsigned char* gene, unsigned int size);
                void push_back(unsigned char* item, unsigned int size);
                void extract(unsigned int i_start, unsigned int i_end, unsigned char*& gene) const;

                // Gene modifiers
                bool erase_gene(unsigned int index);
                bool insert_gene(unsigned int index, unsigned char* gene, unsigned int size);
                bool replace_gene(unsigned int index, unsigned char* gene, unsigned int size);
                bool push_back_gene(unsigned char* item, unsigned int size);
                bool extract_gene(unsigned int index, unsigned char*& gene, unsigned int& size) const;

                // Operators
                bool operator== (const DNA& dna);
                bool operator!= (const DNA& dna);

		// Debugging routines
		void debug() const;
		void debug_raw() const;

                // Auxiliary (should be private, public due to unit testing)
                unsigned int separator(unsigned int index) const;
                unsigned int gene_start(unsigned int index) const;
                unsigned int gene_end(unsigned int index) const;

	private:
                // Member data
		unsigned char* dataGenes;
                unsigned int dataSize;
};



// Include guard
#endif
