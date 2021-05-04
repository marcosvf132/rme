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

#ifndef RME_AREAS_H_
#define RME_AREAS_H_

#include "position.h"
#include "brush_enums.h"

class Area {
public:

	int getID() const { return id; }

	void setID(int newid) { id = newid; }

	void setType(AreaType_t newtype) { type = newtype; }

	AreaType_t getType() { return type; }

	bool isSubarea() const { return type == subarea; }

	bool isArea() const { return type == area; }

	std::string getName() const { return name; }
	void setName(std::string newname) { name = newname; }

	std::string getAlias() const { return akaname; }
	void setAlias(std::string newalias) { akaname = newalias; }

	bool isSubareaOn(Area* area) const { return parentarea == area; }

	void setSubareaOn(Area* area) {
		if (area && isSubarea() && area->isArea()) {
			parentarea = area;
			if (area != nullptr) {
				area->subareas.push_back(this);
			}
		}
	}

	void removeSubarea(Area* area) {
		if (area && isArea() && area->isSubarea()) {
			area->setSubareaOn(nullptr);
			for (std::vector<Area*>::iterator subarea_it = subareas.begin(); subarea_it != subareas.end(); ++subarea_it) {
				Area* subarea = *subarea_it;
				if (subarea && subarea->getID() == area->getID()) {
					subareas.erase(subarea_it);
					break;
				}
			}
			subareas.shrink_to_fit();
		}
	}

	Area* getParentArea() const { return parentarea; }

	std::vector<Area*> getSubareas() const { return subareas; }

	void addTile(Tile* tile) {
		if (tile) {
			tiles.push_back(tile);
			tiles.shrink_to_fit();
		}
	}

	void removeTile(Tile* tile) {
		if (tile) {
			for (std::vector<Tile*>::iterator tile_it = tiles.begin(); tile_it != tiles.end(); ++tile_it) {
				Tile* tile_inc = *tile_it;
				if (tile_inc) {
					tiles.erase(tile_it);
					break;
				}
			}
			tiles.shrink_to_fit();
		}
	}

	TileVector getTiles() const { return tiles; }

	Position getMainPosition() const;

	void setMainPosition(Position mainpos) { pos = mainpos; };

	size_t getSize() const { return tiles.size(); }

	bool isDonation() const { return donate; }

	void setDonation(bool donation) { donate = donation; }


private:
	uint32_t id; // Area ID

	AreaType_t type = area; // Type. Default: area

	Position pos = Position(0, 0, 0); // Main position

	bool donate = true; // Is donation (Client 12 feature)

	std::string name; // Area name
	std::string akaname; // 'Also known as' or alias

	Area* parentarea;

	std::vector<Tile*> tiles; // Tiles included
	std::vector<Area*> subareas; // Subareas
};

typedef std::vector<Area*> AreaVector;

class Areas {
	Map& map;
public:
	Areas(Map& map) : map(map) {}
	~Areas() {}

	void addArea(Area* area);
	Area* getArea(uint16_t areaid);
	Area* getArea(std::string name);
	void removeArea(uint16_t areaid);
	size_t Count() const { return areas.size(); }

	AreaVector areas;

	AreaVector::iterator begin() {return areas.begin();}
	AreaVector::const_iterator begin() const {return areas.begin();}
	AreaVector::iterator end() {return areas.end();}
	AreaVector::const_iterator end() const {return areas.end();}
};

#endif
