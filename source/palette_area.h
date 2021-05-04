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

#ifndef RME_PALETTE_AREA_H_
#define RME_PALETTE_AREA_H_

#include "palette_common.h"

class Area;

class AreaPalettePanel : public PalettePanel {
public:
	AreaPalettePanel(wxWindow* parent, wxWindowID id = wxID_ANY);
	~AreaPalettePanel();

	PaletteType GetType() const;

	// Select the first brush
	void SelectFirstBrush();
	// Returns the currently selected brush (first brush if panel is not loaded)
	Brush* GetSelectedBrush() const;
	// Returns the currently selected brush size
	int GetSelectedBrushSize() const;
	// Select the brush in the parameter, this only changes the look of the panel
	bool SelectBrush(const Brush* whatbrush);

	// Called sometimes?
	void OnUpdate();
	// Called when this page is about to be displayed
	void OnSwitchIn();

	void OnLayoutFixTimer(wxTimerEvent& event);

	void SetMap(Map* map);
protected:
	// Internal use
	void SaveArea();
	void SelectAreaList(size_t index);
	void SelectArea(size_t index);
	void SelectSubarea(size_t index);

	Area* GetCurrentlySelectedSubarea() const;
	Area* GetCurrentlySelectedArea() const;

	void SelectAreaBrush();
	void SelectSubareaBrush();
public:
	// wxWidgets event handling
	void OnAreaChange(wxCommandEvent& event);
	void OnAreaListBoxChange(wxCommandEvent& event);
	void OnAreaListBoxDoubleClick(wxCommandEvent& event);
	void OnClickAreaBrushButton(wxCommandEvent& event);
	void OnClickSubAreaBrushButton(wxCommandEvent& event);
	
	void OnSubareaListBoxChange(wxCommandEvent& event);
	void OnSubareaListBoxDoubleClick(wxCommandEvent& event);
	void OnClickAddArea(wxCommandEvent& event);
	void OnClickEditArea(wxCommandEvent& event);
	void OnClickRemoveArea(wxCommandEvent& event);

	void OnClickSubAddArea(wxCommandEvent& event);
	void OnClickSubEditArea(wxCommandEvent& event);
	void OnClickSubRemoveArea(wxCommandEvent& event);

	#ifdef __APPLE__
	//Used for detecting a deselect
	void OnListBoxClick(wxMouseEvent& event);
	#endif

protected:
	Map* map;
	//wxChoice* area_choice;
	SortableListBox* area_list;
	SortableListBox* subarea_list;
	wxToggleButton* area_brush_button;
	wxToggleButton* subarea_brush_button;
	wxButton* add_area_button;
	wxButton* edit_area_button;
	wxButton* remove_area_button;
	wxButton* add_subarea_button;
	wxButton* edit_subarea_button;
	wxButton* remove_subarea_button;

	// Used for ugly hack
	bool do_resize_on_display;
	wxTimer fix_size_timer;

	DECLARE_EVENT_TABLE()
};

class EditAreaDialog : public wxDialog
{
public:
	EditAreaDialog(wxWindow* parent, Map* map, Area* area);
	virtual ~EditAreaDialog();

	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);
protected:
	Map* map;
	Area* what_area;

	wxString area_name, area_id, area_type, area_pos_x, area_pos_y, area_pos_z, area_alias;
	wxCheckBox* donate_field;
	wxTextCtrl* type_field;
	wxTextCtrl* posx_field;
	wxTextCtrl* posy_field;
	wxTextCtrl* posz_field;
	wxTextCtrl* alias_field;
	wxTextCtrl* name_field;
	wxTextCtrl* id_field;

	DECLARE_EVENT_TABLE();
};

#endif
