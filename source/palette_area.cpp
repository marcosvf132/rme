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

#include "palette_area.h"

#include "settings.h"

#include "brush.h"
#include "editor.h"
#include "map.h"

#include "application.h"
#include "map_display.h"

#include "area_brush.h"
#include "spawn_brush.h"

// ============================================================================
// Area palette

BEGIN_EVENT_TABLE(AreaPalettePanel, PalettePanel)
	EVT_TIMER(PALETTE_LAYOUT_FIX_TIMER, AreaPalettePanel::OnLayoutFixTimer)

	EVT_LISTBOX(PALETTE_AREA_LISTBOX, AreaPalettePanel::OnAreaListBoxChange)
	EVT_LISTBOX_DCLICK(PALETTE_AREA_LISTBOX, AreaPalettePanel::OnAreaListBoxDoubleClick)

	EVT_LISTBOX(PALETTE_SUBAREA_LISTBOX, AreaPalettePanel::OnSubareaListBoxChange)
	EVT_LISTBOX_DCLICK(PALETTE_SUBAREA_LISTBOX, AreaPalettePanel::OnSubareaListBoxDoubleClick)

	EVT_BUTTON(PALETTE_AREA_ADD_AREA, AreaPalettePanel::OnClickAddArea)
	EVT_BUTTON(PALETTE_AREA_EDIT_AREA, AreaPalettePanel::OnClickEditArea)
	EVT_BUTTON(PALETTE_AREA_REMOVE_AREA, AreaPalettePanel::OnClickRemoveArea)

	EVT_BUTTON(PALETTE_SUBAREA_ADD_AREA, AreaPalettePanel::OnClickSubAddArea)
	EVT_BUTTON(PALETTE_SUBAREA_EDIT_AREA, AreaPalettePanel::OnClickSubEditArea)
	EVT_BUTTON(PALETTE_SUBAREA_REMOVE_AREA, AreaPalettePanel::OnClickSubRemoveArea)

	EVT_TOGGLEBUTTON(PALETTE_AREA_BRUSH_BUTTON, AreaPalettePanel::OnClickAreaBrushButton)
	EVT_TOGGLEBUTTON(PALETTE_SUBAREA_BRUSH_BUTTON, AreaPalettePanel::OnClickSubAreaBrushButton)
END_EVENT_TABLE()

AreaPalettePanel::AreaPalettePanel(wxWindow* parent, wxWindowID id) :
	PalettePanel(parent, id),
	map(nullptr),
	do_resize_on_display(true),
	fix_size_timer(this, PALETTE_LAYOUT_FIX_TIMER)
{
	wxSizer* topsizer = newd wxBoxSizer(wxVERTICAL);
	wxSizer* tmpsizer;

	// Area
	wxSizer* sidesizer = newd wxStaticBoxSizer(wxVERTICAL, this, "Areas");

	area_list = newd SortableListBox(this, PALETTE_AREA_LISTBOX);
#ifdef __APPLE__
	//Used for detecting a deselect
	area_list->Bind(wxEVT_LEFT_UP, &AreaPalettePanel::OnListBoxClick, this);
#endif
	sidesizer->Add(area_list, 1, wxEXPAND);

	tmpsizer = newd wxBoxSizer(wxHORIZONTAL);
	wxSizerFlags sizerFlags(1);
	tmpsizer->Add(add_area_button = newd wxButton(this, PALETTE_AREA_ADD_AREA, "New area", wxDefaultPosition, wxSize(70, -1)), sizerFlags);
	tmpsizer->Add(edit_area_button = newd wxButton(this, PALETTE_AREA_EDIT_AREA, "Edit area", wxDefaultPosition, wxSize(70, -1)), sizerFlags);
	tmpsizer->Add(remove_area_button = newd wxButton(this, PALETTE_AREA_REMOVE_AREA, "Remove area", wxDefaultPosition, wxSize(90, -1)), sizerFlags);
	sidesizer->Add(tmpsizer, wxSizerFlags(0).Right());

	topsizer->Add(sidesizer, 1, wxEXPAND);

	// Subarea
	sidesizer = newd wxStaticBoxSizer(wxVERTICAL, this, "Subareas");

	subarea_list = newd SortableListBox(this, PALETTE_SUBAREA_LISTBOX);
	#ifdef __APPLE__
	//Used for detecting a deselect
	area_list->Bind(wxEVT_LEFT_UP, &AreaPalettePanel::OnListBoxClick, this);
	#endif
	sidesizer->Add(subarea_list, 1, wxEXPAND);

	tmpsizer = newd wxBoxSizer(wxHORIZONTAL);
	tmpsizer->Add(add_subarea_button = newd wxButton(this, PALETTE_SUBAREA_ADD_AREA, "New subarea", wxDefaultPosition, wxSize(70, -1)), sizerFlags);
	tmpsizer->Add(edit_subarea_button = newd wxButton(this, PALETTE_SUBAREA_EDIT_AREA, "Edit subarea", wxDefaultPosition, wxSize(70, -1)), sizerFlags);
	tmpsizer->Add(remove_subarea_button = newd wxButton(this, PALETTE_SUBAREA_REMOVE_AREA, "Remove subarea", wxDefaultPosition, wxSize(90, -1)), sizerFlags);
	sidesizer->Add(tmpsizer, wxSizerFlags(0).Right());

	topsizer->Add(sidesizer, 1, wxEXPAND);

	// Temple position
	sidesizer = newd wxStaticBoxSizer(newd wxStaticBox(this, wxID_ANY, "Brushes", wxDefaultPosition, wxSize(150, 200)), wxVERTICAL);

	//sidesizer->Add(180, 1, wxEXPAND);

	tmpsizer = newd wxBoxSizer(wxHORIZONTAL);
	area_brush_button = newd wxToggleButton(this, PALETTE_AREA_BRUSH_BUTTON, "Area tiles");
	tmpsizer->Add(area_brush_button);
	sidesizer->Add(tmpsizer, wxSizerFlags(1).Center());

	tmpsizer = newd wxBoxSizer(wxHORIZONTAL);
	subarea_brush_button = newd wxToggleButton(this, PALETTE_SUBAREA_BRUSH_BUTTON, "Subarea tiles");
	tmpsizer->Add(subarea_brush_button);
	sidesizer->Add(tmpsizer, wxSizerFlags(1).Center());

	topsizer->Add(sidesizer, 0, wxEXPAND);

	SetSizerAndFit(topsizer);
}

AreaPalettePanel::~AreaPalettePanel()
{
	////
}

void AreaPalettePanel::SetMap(Map* m)
{
	g_gui.area_brush->setArea(nullptr);
	map = m;
	OnUpdate();
}

void AreaPalettePanel::OnSwitchIn()
{
	PalettePanel::OnSwitchIn();
	// Extremely ugly hack to fix layout issue
	if(do_resize_on_display) {
		fix_size_timer.Start(100, true);
		do_resize_on_display = false;
	}
}

void AreaPalettePanel::OnLayoutFixTimer(wxTimerEvent& WXUNUSED(event))
{
	wxWindow* w = this;
	while((w = w->GetParent()) && dynamic_cast<PaletteWindow*>(w) == nullptr);

	if(w) {
		w->SetSize(w->GetSize().GetWidth(), w->GetSize().GetHeight() + 1);
		w->SetSize(w->GetSize().GetWidth(), w->GetSize().GetHeight() - 1);
	}
}

void AreaPalettePanel::SelectFirstBrush()
{
	SelectAreaBrush();
}

Brush* AreaPalettePanel::GetSelectedBrush() const
{
	if (area_brush_button->GetValue()) {
		g_gui.area_brush->setArea(GetCurrentlySelectedArea());
		return (g_gui.area_brush->getAreaID() != 0? g_gui.area_brush : nullptr);
	} else if (subarea_brush_button->GetValue()) {
		g_gui.subarea_brush->setArea(GetCurrentlySelectedArea());
		g_gui.subarea_brush->setSubarea(GetCurrentlySelectedSubarea());
		return (g_gui.subarea_brush->getSubareaID() != 0 ? g_gui.subarea_brush : nullptr);
	}
	return nullptr;
}

bool AreaPalettePanel::SelectBrush(const Brush* whatbrush)
{
	if(!whatbrush || !map)
		return false;

	if(whatbrush->isArea()) {
		const AreaBrush* area_brush = static_cast<const AreaBrush*>(whatbrush);
		for (Area* area : map->areas.areas) {
			if(area && area->getID() == area_brush->getAreaID()) {
				for(uint32_t j = 0; j < area_list->GetCount(); ++j) {
					if(area->getID() == reinterpret_cast<Area*>(area_list->GetClientData(j))->getID()) {
						SelectArea(j);
						return true;
					}
				}
				return true;
			}
		}
	} else if (whatbrush->isSubarea()) {
		const SubAreaBrush* subarea_brush = static_cast<const SubAreaBrush*>(whatbrush);
		for (Area* subarea : map->areas.areas) {
			if (subarea && subarea->getID() == subarea_brush->getAreaID()) {
				for (uint32_t j = 0; j < subarea_list->GetCount(); ++j) {
					if (subarea->getID() == reinterpret_cast<Area*>(subarea_list->GetClientData(j))->getID()) {
						//SelectArea(j, true);
						SelectSubareaBrush();
						g_gui.RefreshView();
						return true;
					}
				}
				return true;
			}
		}
	}
	return false;
}

int AreaPalettePanel::GetSelectedBrushSize() const
{
	return 0;
}

PaletteType AreaPalettePanel::GetType() const
{
	return TILESET_AREA;
}

void AreaPalettePanel::SelectAreaList(size_t index)
{
	ASSERT(area_list->GetCount() >= index);

	if(map == nullptr) {
		add_subarea_button->Enable(false);
		add_area_button->Enable(false);
	} else {
		Area* what_area = reinterpret_cast<Area*>(area_list->GetClientData(index));

		// Clear the old arealist
		area_list->Clear();
		subarea_list->Clear();

		for (Area* area_iter : map->areas.areas) {
			if (area_iter->isArea()) {
				std::ostringstream fo;
				if (!area_iter->getAlias().empty())
					fo << " Alias: " << area_iter->getAlias();

				std::ostringstream os;
				os << area_iter->getName();
				os << " (ID:" << area_iter->getID() << fo.str() << ")";
				area_list->Append(wxstr(os.str()), area_iter);
			}
		}

		area_list->Sort();
		subarea_list->Sort();

		// Select first area
		SelectArea(0);
		//area_choice->SetSelection(index);
		add_subarea_button->Enable(what_area != nullptr);
		add_area_button->Enable(true);
		ASSERT(what_area == nullptr || add_area_button->IsEnabled() || !IsEnabled());
	}
}

void AreaPalettePanel::SelectArea(size_t index)
{
	ASSERT(area_list->GetCount() >= index);
	subarea_list->Clear();

	if(area_list->GetCount() > 0) {
		add_subarea_button->Enable(true);
		edit_subarea_button->Enable(true);
		remove_subarea_button->Enable(true);
		subarea_brush_button->Enable(true);
		// Select the area
		area_list->SetSelection(index);
		SelectAreaBrush();


		// Select subarea
		Area* selected_area = reinterpret_cast<Area*>(area_list->GetClientData(index));
		if (selected_area) {
			for (Area* subarea : selected_area->getSubareas()) {
				std::ostringstream fo;
				if (!subarea->getAlias().empty())
					fo << " Alias: " << subarea->getAlias();

				std::ostringstream os;
				os << subarea->getName();
				os << " (ID:" << subarea->getID() << fo.str() << ")";
				subarea_list->Append(wxstr(os.str()), subarea);
			}
		}
	} else {
		add_subarea_button->Enable(false);
		edit_subarea_button->Enable(false);
		remove_subarea_button->Enable(false);
		subarea_brush_button->Enable(false);
	}


	SelectAreaBrush();
	g_gui.RefreshView();
}

void AreaPalettePanel::SelectSubarea(size_t index)
{
	ASSERT(subarea_list->GetCount() >= index);

	if (subarea_list->GetCount() > 0) {
		subarea_list->SetSelection(index);
	}

	SelectSubareaBrush();
	g_gui.RefreshView();
}

Area* AreaPalettePanel::GetCurrentlySelectedSubarea() const
{
	int selection = subarea_list->GetSelection();
	if(subarea_list->GetCount() > 0 && selection != wxNOT_FOUND) {
		return reinterpret_cast<Area*>(subarea_list->GetClientData(selection));
	}
	return nullptr;
}

Area* AreaPalettePanel::GetCurrentlySelectedArea() const
{
	int selection = area_list->GetSelection();
	if (area_list->GetCount() > 0 && selection != wxNOT_FOUND) {
		return reinterpret_cast<Area*>(area_list->GetClientData(selection));
	}
	return nullptr;
}

void AreaPalettePanel::SelectAreaBrush()
{
	area_brush_button->SetValue(true);
	subarea_brush_button->SetValue(false);
}

void AreaPalettePanel::SelectSubareaBrush()
{
	area_brush_button->SetValue(false);
	subarea_brush_button->SetValue(true);
}

void AreaPalettePanel::OnUpdate()
{
	int old_area_selection = area_list->GetSelection();

	area_list->Clear();

	if(map == nullptr)
		return;

	if(map->areas.Count() != 0) {
		// Create choice control
		for (Area* area_iter : map->areas.areas) {
			if (area_iter->isArea())
				area_list->Append(wxstr(area_iter->getName()), area_iter);
		}
		if(old_area_selection <= 0)
			SelectAreaList(0);
		else if((size_t)old_area_selection <= area_list->GetCount())
			SelectAreaList(old_area_selection);
		else
			SelectAreaList(static_cast<size_t>(old_area_selection) - 1);

		area_list->Enable(true);
	} else {
		subarea_brush_button->Enable(false);
		subarea_brush_button->SetValue(false);
		add_subarea_button->Enable(false);
		edit_subarea_button->Enable(false);
		remove_subarea_button->Enable(false);

		SelectAreaList(0);
	}
}

void AreaPalettePanel::OnAreaChange(wxCommandEvent& event)
{
	SelectAreaList(event.GetSelection());
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnAreaListBoxChange(wxCommandEvent& event)
{
	SelectArea(event.GetSelection());
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnSubareaListBoxChange(wxCommandEvent& event)
{
	SelectSubarea(event.GetSelection());
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnAreaListBoxDoubleClick(wxCommandEvent& event)
{
	Area* area = reinterpret_cast<Area*>(event.GetClientData());
	if(area && area->getMainPosition().isValid()) {
		g_gui.SetScreenCenterPosition(area->getMainPosition());
	}
	SelectArea(event.GetSelection());
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnSubareaListBoxDoubleClick(wxCommandEvent& event)
{
	Area* subarea = reinterpret_cast<Area*>(event.GetClientData());
	if (subarea && subarea->getMainPosition().isValid()) {
		g_gui.SetScreenCenterPosition(subarea->getMainPosition());
	}
	SelectSubarea(event.GetSelection());
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnClickAreaBrushButton(wxCommandEvent& event)
{
	SelectAreaBrush();
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnClickSubAreaBrushButton(wxCommandEvent& event)
{
	SelectSubareaBrush();
	g_gui.SelectBrush();
}

void AreaPalettePanel::OnClickAddArea(wxCommandEvent& event)
{
	if(map == nullptr)
		return;

	Area* new_area = new Area();
	new_area->setType(area);

	int newID = 1;
	while (newID == 1 || map->areas.getArea(newID)) {
		++newID;
	}

	new_area->setID(newID);

	std::ostringstream os;
	os << "Unnamed Area #" << new_area->getID();
	new_area->setName(os.str());

	map->areas.addArea(new_area);
	std::ostringstream foo;
	foo << new_area->getName();
	foo << " (ID:" << new_area->getID() << ")";
	area_list->Append(wxstr(foo.str()), map->areas.getArea(newID));
	subarea_list->Clear();
	SelectArea(area_list->FindString(foo.str()));
	g_gui.SelectBrush();
	refresh_timer.Start(300, true);
}

void AreaPalettePanel::OnClickEditArea(wxCommandEvent& event)
{
	if(area_list->GetCount() == 0)
		return;
	if(map == nullptr)
		return;
	int selection = area_list->GetSelection();
	Area* area = reinterpret_cast<Area*>(area_list->GetClientData(selection));
	if(area) {
		wxDialog* d = newd EditAreaDialog(g_gui.root, map, area);
		int ret = d->ShowModal();
		if(ret == 1) {
			// Something changed, change name of area
			std::ostringstream fo;
			if (!area->getAlias().empty())
				fo << " Alias: " << area->getAlias();

			std::ostringstream os;
			os << area->getName();
			os << " (ID:" << area->getID() << fo.str() << ")";
			area_list->SetString(selection, wxstr(os.str()));
			area_list->Sort();
			refresh_timer.Start(300, true);
		}
	}
}

void AreaPalettePanel::OnClickRemoveArea(wxCommandEvent& event)
{
	int selection = area_list->GetSelection();
	if(selection != wxNOT_FOUND) {
		Area* area = reinterpret_cast<Area*>(area_list->GetClientData(selection));
		if (area) {
			if (area->getSubareas().size() > 0) {
				std::ostringstream warn;
				warn << "The area '" << area->getName() << "' contains " << area->getSubareas().size() << " subareas, deleting it will delete all included subareas permanently. Are you sure you want to continue?";
				int ret = g_gui.PopupDialog(this, "Warning", warn.str(), wxYES | wxNO);
				if (ret == wxID_NO) {
					return;
				}
			}
			for (Area* subarea : area->getSubareas()) {
				if (subarea) {
					area->removeSubarea(subarea);
					map->areas.removeArea(subarea->getID());
					for (Tile* subareatile : subarea->getTiles()) {
						if (subareatile) {
							subareatile->setArea(nullptr);
						}
					}
				}
			}
			for (Tile* areatile : area->getTiles()) {
				if (areatile) {
					areatile->setArea(nullptr);
				}
			}
			map->areas.removeArea(area->getID());
			area_list->Delete(selection);
			subarea_list->Clear();
			refresh_timer.Start(300, true);

			if (int(area_list->GetCount()) <= selection) {
				selection -= 1;
			}

			if (selection >= 0 && area_list->GetCount()) {
				area_list->SetSelection(selection);
			}
			else {
				subarea_brush_button->Enable(false);
				subarea_brush_button->SetValue(false);
				edit_subarea_button->Enable(false);
				remove_subarea_button->Enable(false);
			}
			g_gui.SelectBrush();
		}
	}
	g_gui.RefreshView();
}

void AreaPalettePanel::OnClickSubAddArea(wxCommandEvent& event)
{
	if (map == nullptr)
		return;

	int selection = area_list->GetSelection();
	if (selection != wxNOT_FOUND) {
		Area* area = reinterpret_cast<Area*>(area_list->GetClientData(selection));
		if (area && area->isArea()) {
			Area* new_subarea = new Area();
			new_subarea->setType(subarea);

			int newID = 0;
			while (newID == 0 || map->areas.getArea(newID)) {
				++newID;
			}

			new_subarea->setID(newID);
			std::ostringstream os;
			os << "Unnamed Subarea #" << new_subarea->getID();
			new_subarea->setName(os.str());
			new_subarea->setSubareaOn(area);
			map->areas.addArea(new_subarea);
			std::ostringstream foo;
			foo << new_subarea->getName();
			foo << " (ID:" << new_subarea->getID() << ")";
			subarea_list->Append(wxstr(foo.str()), map->areas.getArea(newID));
			SelectSubareaBrush();
			g_gui.RefreshView();
			g_gui.SelectBrush();
			refresh_timer.Start(300, true);
		}
	}
}

void AreaPalettePanel::OnClickSubEditArea(wxCommandEvent& event)
{

	if (subarea_list->GetCount() == 0)
		return;
	if (map == nullptr)
		return;
	int selection = subarea_list->GetSelection();
	Area* subarea = reinterpret_cast<Area*>(subarea_list->GetClientData(selection));
	if (subarea) {
		wxDialog* d = newd EditAreaDialog(g_gui.root, map, subarea);
		int ret = d->ShowModal();
		if (ret == 1) {
			// Something changed, change name of area
			std::ostringstream fo;
			if (!subarea->getAlias().empty())
				fo << " Alias: " << subarea->getAlias();

			std::ostringstream os;
			os << subarea->getName();
			os << " (ID:" << subarea->getID() << fo.str() << ")";
			subarea_list->SetString(selection, wxstr(os.str()));
			subarea_list->Sort();
			refresh_timer.Start(300, true);
		}
	}
}

void AreaPalettePanel::OnClickSubRemoveArea(wxCommandEvent& event)
{
	int selection = subarea_list->GetSelection();
	if (selection != wxNOT_FOUND) {
		Area* subarea = reinterpret_cast<Area*>(subarea_list->GetClientData(selection));
		if (subarea && subarea->isSubarea()) {
			std::ostringstream warn;
			warn << "Deleting a subarea is permanently and there is no turning back. Are you sure you want to continue?";
			int ret = g_gui.PopupDialog(this, "Warning", warn.str(), wxYES | wxNO);
			if (ret == wxID_NO) {
				return;
			}
			if (subarea->getParentArea())
				subarea->getParentArea()->removeSubarea(subarea);

			for (Tile* areatile : subarea->getTiles()) {
				if (areatile) {
					areatile->setSubarea(nullptr);
				}
			}

			map->areas.removeArea(subarea->getID());
			subarea_list->Delete(selection);
			refresh_timer.Start(300, true);

			if (int(subarea_list->GetCount()) <= selection) {
				selection -= 1;
			}

			if (selection >= 0 && subarea_list->GetCount()) {
				subarea_list->SetSelection(selection);
			}
			g_gui.SelectBrush();
		}
	}
	g_gui.RefreshView();
}

#ifdef __APPLE__
//On wxMac it is possible to deselect a wxListBox. (Unlike on the other platforms)
//EVT_LISTBOX is not triggered when the deselection is happening. http://trac.wxwidgets.org/ticket/15603
//Here we find out if the listbox was deselected using a normal mouse up event so we know when to disable the buttons and brushes.
void AreaPalettePanel::OnListBoxClick(wxMouseEvent& event) {
	if (area_list->GetSelection() == wxNOT_FOUND) {
		select_position_button->Enable(false);
		select_position_button->SetValue(false);
		area_brush_button->Enable(false);
		area_brush_button->SetValue(false);
		edit_area_button->Enable(false);
		remove_area_button->Enable(false);
		g_gui.SelectBrush();
	}
}
#endif

// ============================================================================
// Area Edit Dialog

BEGIN_EVENT_TABLE(EditAreaDialog, wxDialog)
	EVT_BUTTON(wxID_OK, EditAreaDialog::OnClickOK)
	EVT_BUTTON(wxID_CANCEL, EditAreaDialog::OnClickCancel)
END_EVENT_TABLE()

EditAreaDialog::EditAreaDialog(wxWindow* parent, Map* map, Area* thisArea) :
	wxDialog(parent, wxID_ANY, thisArea->isSubarea() ? "Subarea" : "Area", wxDefaultPosition, wxSize(250,160)),
	map(map),
	what_area(thisArea)
{
	ASSERT(map);
	ASSERT(thisArea);

	// Create topsizer
	wxSizer* sizer = newd wxBoxSizer(wxVERTICAL);
	wxSizer* tmpsizer;

	area_name = wxstr(thisArea->getName());
	area_id = i2ws(thisArea->getID());
	std::string areaType = "";
	switch (thisArea->getType()) {
	case undefined: {
		areaType = "Undefined";
		break;
		}
	case area: {
		areaType = "Area";
		break;
	}
	case subarea: {
		areaType = "Subarea";
		break;
	}
	default:
		break;
	}
	area_type = wxstr(areaType);
	area_pos_x = i2ws(thisArea->getMainPosition().x);
	area_pos_y = i2ws(thisArea->getMainPosition().y);
	area_pos_z = i2ws(thisArea->getMainPosition().z);
	area_alias = wxstr(thisArea->getAlias());

	// Area options

	// Name & ID
	tmpsizer = newd wxStaticBoxSizer(wxHORIZONTAL, this, "Name / ID");
	name_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(160,20), 0, wxTextValidator(wxFILTER_ASCII, &area_name));
	tmpsizer->Add(name_field);
	id_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(70, 20), 0, wxTextValidator(wxFILTER_NUMERIC, &area_id));
	id_field->Enable(false);
	tmpsizer->Add(id_field);

	sizer->Add(tmpsizer, wxSizerFlags().Border(wxALL, 10));

	// Alias & type
	tmpsizer = newd wxStaticBoxSizer(wxHORIZONTAL, this, "Alias / Type");
	alias_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(160, 20), 0, wxTextValidator(wxFILTER_ASCII, &area_alias));
	tmpsizer->Add(alias_field);
	type_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(70, 20), 0, wxTextValidator(wxFILTER_ASCII, &area_type));
	type_field->Enable(false);
	tmpsizer->Add(type_field);

	sizer->Add(tmpsizer, wxSizerFlags().Border(wxALL, 10));

	tmpsizer = newd wxStaticBoxSizer(wxHORIZONTAL, this, "Main position");
	posx_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(110,20), 0, wxTextValidator(wxFILTER_NUMERIC, &area_pos_x));
	tmpsizer->Add(posx_field);
	posy_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(110, 20), 0, wxTextValidator(wxFILTER_NUMERIC, &area_pos_y));
	tmpsizer->Add(posy_field);
	posz_field = newd wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(20, 20), 0, wxTextValidator(wxFILTER_NUMERIC, &area_pos_z));
	tmpsizer->Add(posz_field);

	sizer->Add(tmpsizer, wxSizerFlags().Border(wxALL, 10));

	// Area options
	donate_field = newd wxCheckBox(this, wxID_ANY, "Can donate", wxDefaultPosition);

	sizer->Add(donate_field, wxSizerFlags().Border(wxRIGHT | wxLEFT | wxBOTTOM, 20));
	donate_field->SetValue(true);

	// OK/Cancel buttons
	tmpsizer = newd wxBoxSizer(wxHORIZONTAL);
	tmpsizer->Add(newd wxButton(this, wxID_OK, "OK"), wxSizerFlags(1).Center());
	tmpsizer->Add(newd wxButton(this, wxID_CANCEL, "Cancel"), wxSizerFlags(1).Center());
	sizer->Add(tmpsizer, wxSizerFlags(1).Center().Border(wxRIGHT | wxLEFT | wxBOTTOM, 20));

	SetSizerAndFit(sizer);
}

EditAreaDialog::~EditAreaDialog()
{
	////
}

void EditAreaDialog::OnClickOK(wxCommandEvent& WXUNUSED(event))
{
	if(Validate() && TransferDataFromWindow()) {
		// Verify the newd information
		long new_area_pos_x;
		area_pos_x.ToLong(&new_area_pos_x);
		long new_area_pos_y;
		area_pos_y.ToLong(&new_area_pos_y);
		long new_area_pos_z;
		area_pos_z.ToLong(&new_area_pos_z);

		Position setPosition = Position(new_area_pos_x, new_area_pos_y, new_area_pos_z);
		if(setPosition != Position(0, 0, 0) && !setPosition.isValid()) {
			g_gui.PopupDialog(this, "Error", "The main position inserted is invalid.", wxOK);
			return;
		}

		if(area_name.length() == 0) {
			g_gui.PopupDialog(this, "Error", "Name cannot be nil.", wxOK);
			return;
		}

		for (Area* area : map->areas.areas) {
			ASSERT(area);
			if(wxstr(area->getName()) == area_name && area->getID() != what_area->getID()) {
				int ret = g_gui.PopupDialog(this, "Warning", "This name is already in use, are you sure you want to continue?", wxYES | wxNO);
				if(ret == wxID_NO) {
					return;
				}
			}
		}

		// Transfer to area
		what_area->setName(nstr(area_name));
		what_area->setAlias(nstr(area_alias));
		what_area->setDonation(donate_field->GetValue());
		what_area->setMainPosition(Position(new_area_pos_x, new_area_pos_y, new_area_pos_z));

		EndModal(1);
	}
}

void EditAreaDialog::OnClickCancel(wxCommandEvent& WXUNUSED(event))
{
	// Just close this window
	EndModal(0);
}
