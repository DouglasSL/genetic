/*
 * image_benchmark.cpp
 * Evolve - Image generating environment which benchmarks the library
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
#include "image.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <sstream>
#include <ctime>
#include <cairo/cairo.h>
#include "gnuplot/gnuplot.h"


//
// Constants
//

// Time to test
const int BENCHMARK_SECONDS = 5;
const std::string IMAGE_PLOT = "image_benchmark";



//////////////////////
// CLASS DEFINITION //
//////////////////////

// Environment
class EnvImgBenchmark : public EnvImage
{
    public:
        // Construction & destruction
        EnvImgBenchmark();

        // Required functions
		void update(const DNA& inputDNA);
		bool condition();

		// Additional functions
		void reset();
		void setVector(std::vector<double>* dataTime, std::vector<double>* dataFitness);
		void setTime(int inputTime);

    private:
        int runtime;
        std::vector<double>* dataTime;
        std::vector<double>* dataFitness;
		int counter;
		clock_t start;
};



////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

EnvImgBenchmark::EnvImgBenchmark()
{
    runtime = 0;
	reset();
}


//
// Required functions
//

// Update call
void EnvImgBenchmark::update(const DNA& inputDNA)
{
    // Ge the fitness and elapsed time
    double tempFitness = 100000*fitness(inputDNA);
    double tempTime = double(clock()-start)/CLOCKS_PER_SEC;

    // Add values to map
    dataTime->push_back(tempTime);
    dataFitness->push_back(tempFitness);
}

// Condition call
bool EnvImgBenchmark::condition()
{
    double ms = 1000*(clock()-start)/CLOCKS_PER_SEC;
    return ms < runtime*1000;
}


//
// Additional functions
//

// Reset the state
void EnvImgBenchmark::reset()
{
    counter = 0;
	start = clock();
}

// Add the output map
void EnvImgBenchmark::setVector(std::vector<double>* inputTime, std::vector<double>* inputFitness)
{
    dataTime = inputTime;
    dataFitness = inputFitness;
}

// Set the run time
void EnvImgBenchmark::setTime(int inputTime)
{
    runtime = inputTime;
}


///////////
// OTHER //
///////////

void wait_for_key()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::cout << std::endl << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}



//////////
// MAIN //
//////////

int main(int argc, char** argv)
{
	//
	// Configure application
	//

	// Input file
	if (argc < 2)
	{
        std::cout << "! Input filename missing" << std::endl;
        return 1;
	}
	std::string inputFileInput = argv[1];

	// Output file
	std::string inputFileOutput;
	inputFileOutput = argc>=3 ? argv[2] : IMAGE_PLOT;

	// Time of runs
	int inputTime = argc>=4 ? atoi(argv[3]) : BENCHMARK_SECONDS;



	// Message
	std::cout << "* Application configured" << std::endl;


	//
	// Configure environment
	//

	// Create object
	EnvImgBenchmark dataEnvironment;
	dataEnvironment.setTime(inputTime);

	// Load base image
	if (!dataEnvironment.loadImage(inputFileInput))
	{
	    std::cout << "! Could not load image" << std::endl;
	    return 1;
	}

	// Initial DNA (triangle)
	std::deque<int> tempQueue;
	tempQueue.push_back(255);	// Start of DNA
	tempQueue.push_back(50);	// Semi transparent grey brush (RGB = 50 50 50, with 50% opacity)
	tempQueue.push_back(50);
	tempQueue.push_back(50);
	tempQueue.push_back(128);
	tempQueue.push_back(1);     // Point one: (1, 254)
	tempQueue.push_back(254);
	tempQueue.push_back(128);	// Point two: (128, 1)
	tempQueue.push_back(1);
	tempQueue.push_back(254);	// Point three: (254, 254)
	tempQueue.push_back(254);
	tempQueue.push_back(255);	// End of DNA
	DNA tempDNA(tempQueue);

	// Create object
	Population dataPopulation(&dataEnvironment);

	// Message
	std::cout << "* Environment configured" << std::endl;


	//
	// Get data
	//

	std::cout << "* Starting evolution" << std::endl;

	// Single straight
	std::cout << "\t- Testing SINGLE STRAIGHT evolution" << std::endl;
	std::vector<double> dataSingleStraightTime;
	std::vector<double> dataSingleStraightFitness;
	dataEnvironment.reset();
	dataEnvironment.setVector(&dataSingleStraightTime, &dataSingleStraightFitness);
	dataPopulation.set(tempDNA);
	dataPopulation.evolve_single_straight();

	// Boxed straight
	std::cout << "\t- Testing BOXED STRAIGHT evolution" << std::endl;
	std::vector<double> dataBoxedStraightTime;
	std::vector<double> dataBoxedStraightFitness;
	dataEnvironment.reset();
	dataEnvironment.setVector(&dataBoxedStraightTime, &dataBoxedStraightFitness);
	dataPopulation.set(tempDNA);
	dataPopulation.evolve_box_straight();

	// Boxed together
	std::cout << "\t- Testing BOXED TOGETHER evolution" << std::endl;
	std::vector<double> dataBoxedTogetherTime;
	std::vector<double> dataBoxedTogetherFitness;
	dataEnvironment.reset();
	dataEnvironment.setVector(&dataBoxedTogetherTime, &dataBoxedTogetherFitness);
	dataPopulation.set(tempDNA);
	dataPopulation.evolve_box_together();

	// Boxed mixed
	std::cout << "\t- Testing BOXED MIXED evolution" << std::endl;
	std::vector<double> dataBoxedMixedTime;
	std::vector<double> dataBoxedMixedFitness;
	dataEnvironment.reset();
	dataEnvironment.setVector(&dataBoxedMixedTime, &dataBoxedMixedFitness);
	dataPopulation.set(tempDNA);
	dataPopulation.evolve_box_mix();


	//
	// Plot
	//

	std::cout << "* Plotting" << std::endl;

	// Create Gnuplot object
	Gnuplot plot = Gnuplot();

	// Configure plot
	plot.set_style("lines");
	plot.set_smooth("bezier");

    // Add data sets
    plot.plot_xy(dataSingleStraightTime, dataSingleStraightFitness, "single-straight evolution");
    plot.plot_xy(dataBoxedStraightTime, dataBoxedStraightFitness, "boxed-straight evolution");
    plot.plot_xy(dataBoxedTogetherTime, dataBoxedTogetherFitness, "boxed-together evolution");
    plot.plot_xy(dataBoxedMixedTime, dataBoxedMixedFitness, "boxed-mixed evolution");

    // Save to file
    plot.savetops(inputFileOutput);
    plot.replot();
    std::cout << "  Result has been saved to " << inputFileOutput << ".ps" << std::endl;

    // Wait for using input
    wait_for_key();

	return 0;
}