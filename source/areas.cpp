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

#include "areas.h"
#include "map.h"

Position Area::getMainPosition() const
{
	if (pos != Position(0, 0, 0) && pos.isValid()) {
		return pos;
	} else if (tiles.size() > 0 && tiles.front()) {
		Tile* tile = tiles.front();
		if (tile) {
			return Position(tile->getX(), tile->getY(), tile->getZ());
		}
	}

	return Position(0, 0, 0);
}

void Areas::addArea(Area* area)
{
	removeArea(area->getID());
	areas.push_back(area);
}

Area* Areas::getArea(uint16_t areaid)
{
	for (Area* area : areas) {
		if (area->getID() == areaid) {
			return area;
		}
	}
	return nullptr;
}

Area* Areas::getArea(std::string name)
{
	to_lower_str(name);
	for (Area* area : areas) {
		if (area->getName() == name) {
			return area;
		}
	}
	return nullptr;
}

void Areas::removeArea(uint16_t areaid)
{
	for (AreaVector::iterator it = areas.begin(); it != areas.end(); it++) {
		Area* area = *it;
		if (area && area->getID() == areaid) {
			areas.erase(it);
			areas.shrink_to_fit();
			break;
		}
	}
}
