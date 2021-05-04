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

#ifndef RME_AREA_BRUSH_H
#define RME_AREA_BRUSH_H

#include "brush.h"
#include "areas.h"

//=============================================================================
// AreaBrush, draws areas simply

class AreaBrush : public Brush
{
public:
	AreaBrush();
	virtual ~AreaBrush();

	bool isArea() const { return true; }

	AreaBrush* asArea() { return static_cast<AreaBrush*>(this); }

	// Not used
	virtual bool load(pugi::xml_node node, wxArrayString& warnings) { return true; }

	virtual bool canDraw(BaseMap* map, const Position& position) const;

	virtual void draw(BaseMap* map, Tile* tile, void* parameter);
	virtual void undraw(BaseMap* map, Tile* tile);

	virtual bool canDrag() const { return true; }

	virtual int getLookID() const { return 0; } // We don't have a graphic
	
	// Public functions
	virtual void setArea(Area* area) { 
		if (area != nullptr)
			areaptr = area;
	}

	virtual uint32_t getAreaID() const {
		if (areaptr)
			return areaptr->getID();

		return 0;
	}

	Area* getArea() const { return areaptr; };
	virtual std::string getName() const { 
		if (areaptr != nullptr)
			return areaptr->getName();

		return "New area";
	}

protected:
	Area* areaptr;
};

//=============================================================================
// SubAreaBrush, draws subareas simply

class SubAreaBrush : public Brush
{
public:
	SubAreaBrush();
	virtual ~SubAreaBrush();

	bool isSubarea() const { return true; }

	SubAreaBrush* asSubarea() { return static_cast<SubAreaBrush*>(this); }

	// Not used
	virtual bool load(pugi::xml_node node, wxArrayString& warnings) { return true; }

	virtual bool canDraw(BaseMap* map, const Position& position) const;

	virtual void draw(BaseMap* map, Tile* tile, void* parameter);

	virtual void undraw(BaseMap* map, Tile* tile);

	virtual bool canDrag() const { return true; }

	virtual int getLookID() const { return 0; } // We don't have a graphic

	// Public functions
	virtual void setSubarea(Area* subarea) {
		if (subarea != nullptr)
			subareaptr = subarea;
	}

	virtual void setArea(Area* area) {
		if (area != nullptr)
			areaptr = area;
	}

	virtual uint32_t getAreaID() const {
		if (areaptr)
			return areaptr->getID();

		return 0;
	}

	virtual uint32_t getSubareaID() const {
		if (subareaptr)
			return subareaptr->getID();

		return 0;
	}

	Area* getArea() const { return areaptr; };

	Area* getSubarea() const { return subareaptr; };

	virtual std::string getName() const {
		if (subareaptr != nullptr)
			return subareaptr->getName();

		return "New subarea";
	}

protected:
	Area* areaptr;
	Area* subareaptr;
};

#endif
