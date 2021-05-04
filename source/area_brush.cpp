//////////////////////////////////////////////////////////////////////
// This file is part of Remere's Map Editor edited by Marcosvf132
//////////////////////////////////////////////////////////////////////
// Remere's Map Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Remere's Map Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// This file part and feature is a customized version made by Marcosvf132
// to give more options to customize the tibia 12 client assets or any other
// that use this system.
//////////////////////////////////////////////////////////////////////

#include "main.h"

#include "area_brush.h"
#include "areas.h"
#include "basemap.h"

//=============================================================================
// Area Brush

AreaBrush::AreaBrush() :
	Brush()
{
	////
}

AreaBrush::~AreaBrush()
{
	////
}

bool AreaBrush::canDraw(BaseMap* map, const Position& position) const
{
	return map->getTile(position) != nullptr;
}

void AreaBrush::undraw(BaseMap* map, Tile* tile)
{
	tile->setArea(nullptr);
}

void AreaBrush::draw(BaseMap* map, Tile* tile, void* parameter)
{
	ASSERT(areaptr);
	tile->setArea(areaptr);
}

//=============================================================================
// Subarea Brush

SubAreaBrush::SubAreaBrush() :
	Brush()
{
	////
}

SubAreaBrush::~SubAreaBrush()
{
	////
}

bool SubAreaBrush::canDraw(BaseMap* map, const Position& position) const
{
	Tile* tile = map->getTile(position);
	if (tile && areaptr && tile->getAreaID() == areaptr->getID())
		return true;

	return false;
}

void SubAreaBrush::undraw(BaseMap* map, Tile* tile)
{
	tile->setSubarea(nullptr);
}

void SubAreaBrush::draw(BaseMap* map, Tile* tile, void* parameter)
{
	ASSERT(subareaptr);
	if (tile && areaptr && tile->getAreaID() == areaptr->getID())
		tile->setSubarea(subareaptr);
}
