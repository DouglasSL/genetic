/*
 * world.cpp
 * World class.
 *
 * Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
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
#include "world.h"


////////////////////
// CLASS ROUTINES //
////////////////////

//
// Construction and destruction
//

// Constructor
World::World(const unsigned int rows, const unsigned  int columns)
{
	// Save size
	sizeColumns = columns;
	sizeRows = rows;

	reset();

	// Maximum commands per creature
	creatureMaxCommands = 100;
	worldMaxSimulations = 100;
}

// Destructor
World::~World()
{
	// Remove and delete all objects
	clearObj();

	// Remove all creatures (cleanup is handled by client)
	clearCreat();

	// Remove and delete all finishes
	std::list<Obstacle*>::iterator it_finish = dataFinishes.begin();
	std::list<Obstacle*>::iterator ite_finish = dataFinishes.end();
	while (it_finish != ite_finish)
	{
		delete (*it_finish);
		++it_finish;
	}
	dataObstacles.clear();
}


//
// Obstacles
//

// Add an obstacle
//   input: percentage of obstacles (unsigned int)
void World::addObstacles(const unsigned int percentage)
{
	// How many blocks do we need?
	unsigned int obstacles = (sizeColumns*sizeRows) * percentage / 100;

	// Add those blocks!
	while (obstacles > 0)
	{
		Obstacle *p_obst = new Obstacle;
		bool obstacle_valid = false;

		while (!obstacle_valid)
		{
			// Generate new block coördinates
			p_obst->dataX = random_range(0, sizeColumns-1);
			p_obst->dataY = random_range(0, sizeRows-1);

			// Loop other obstacles to detect overlapping
			obstacle_valid = true;
			if (dataObstacles.size() > 0)
			{
				std::list<Obstacle*>::iterator it = dataObstacles.begin();
				std::list<Obstacle*>::iterator ite = dataObstacles.end();
				while (obstacle_valid && it != ite)
				{
					if ((*(*it)).dataX == p_obst->dataX && (*(*it)).dataY == p_obst->dataY)
						obstacle_valid = false;
					++it;
				}
			}
		}

		// Save the generated coördinates to the obstacle container
		dataObstacles.push_back(p_obst);

		obstacles--;
	}
}


//
// Spots
//

// Check spot availability
int World::checkSpot(const unsigned int x, const unsigned int y)
{
	// Check if spot is between main borders
	if (x<0 || y<0 || (unsigned)x>=sizeColumns || (unsigned)y>=sizeRows)
		return WORLD_INVALID;

	// Check for obstacles
	std::list<Obstacle*>::iterator it_obst = dataObstacles.begin();
	std::list<Obstacle*>::iterator ite_obst = dataObstacles.end();
	while (it_obst != ite_obst)
	{
		if ((*it_obst)->dataX == x && (*it_obst)->dataY == y)
			return WORLD_OBSTACLE;
		++it_obst;
	}

	// Check for creatures
	std::list<Creature*>::iterator it_creat = dataCreatures.begin();
	std::list<Creature*>::iterator ite_creat = dataCreatures.end();
	while (it_creat != ite_creat)
	{
		if ((*it_creat)->locationIsSet && (*it_creat)->locationX == x && (*it_creat)->locationY == y)
			return WORLD_CREATURE;
		++it_creat;
	}

	return WORLD_FREE;
}

// Check for finish
bool World::checkFinish(const unsigned int x, const unsigned int y)
{
	// Check for finish
	std::list<Obstacle*>::iterator it_finish = dataFinishes.begin();
	std::list<Obstacle*>::iterator ite_finish = dataFinishes.end();
	while (it_finish != ite_finish)
	{
		if ((*it_finish)->dataX == x && (*it_finish)->dataY == y)
			return true;
		++it_finish;
	}
	return false;
}

// Add a finish
void World::addFinish(const unsigned int x, const unsigned int y)
{
	Obstacle* finish = new Obstacle;
	finish->dataX = x;
	finish->dataY = y,
	dataFinishes.push_back(finish);
}


//
// Creatures
//

// Add a creature to the world
//   input: the creature to place (pointer to Creature)
// NOTE: creature placment is handled by the creature itself, this routine only links the creature to the world
void World::creatureAdd(Creature *inputCreature)
{
	// Save a pointer to the creature
	dataCreatures.push_back(inputCreature);

	// Give our creature access to the world
	inputCreature->pointerWorld = this;
}

// Add a creature to the world
//   input: the creature to place (Creature)
void World::creatureAdd(Creature &inputCreature)
{
	// Save a pointer to the creature
	Creature* pointerCreature = &inputCreature;
	dataCreatures.push_back(pointerCreature);

	// Give our creature access to the world
	inputCreature.pointerWorld = this;
}


//
// Simulation handling
//

// Simulate while not all creatures have finished
bool World::do_simulate()
{
	if (worldSimulations > worldMaxSimulations)
	{
		return false;
	}

	int alive = dataCreatures.size();
	std::list<Creature*>::iterator it = dataCreatures.begin();
	std::list<Creature*>::iterator ite = dataCreatures.end();
	while (it != ite)
	{
		if ((*it)->is_finished() && !(*it)->is_alive)
		{
			alive--;
		}
		++it;
	}
	if (alive == 0)
	{
		return false;
	}

	return true;
}

void World::simulate()
{
	while (do_simulate())
	{
		std::list<Creature*>::iterator it = dataCreatures.begin();
		std::list<Creature*>::iterator ite = dataCreatures.end();
		while (it != ite)
		{
			(*it)->simulate();
			++it;
		}
		worldSimulations++;
	}
}

void World::reset()
{
	worldSimulations = 0;
}

void World::clearObj()
{
	// Remove all objects
	std::list<Obstacle*>::iterator it_obst = dataObstacles.begin();
	std::list<Obstacle*>::iterator ite_obst = dataObstacles.end();
	while (it_obst != ite_obst)
	{
		delete (*it_obst);
		++it_obst;
	}
	dataObstacles.clear();
}

void World::clearCreat()
{
	// Remove all creatures
	dataCreatures.clear();
}