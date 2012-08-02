#include "eventsLog.h"
#include "wx/treectrl.h"
#include "wx/treelist.h"


#ifndef EVENTSLIST_H
#define EVENTSLIST_H

#include "scenarioControls.h"

class EventsList : public MyScrolledWindowSmart
{
public:
	EventsList(wxWindow *parent,wxString Column);
		
	void rightClick(wxTreeListEvent& event);
	void doubleClick(wxTreeListEvent& event);

	void addItem();
	void administerItem(wxTreeListItem item);
	void removeItem();
	void expandAllFolders();
	void collapseAllFolders();
	void setEventsLog(EventsLog *);
	void setScenarioControls(ScenarioControls *);

private:
	wxWindow *parent;

	enum
	{
		iconFile,
		iconFolderClosed,
		iconFolderOpened,
		addNewItemID,
		addNewFolderID,
		administerItemID,
		removeItemID,
	};

	wxImageList *imageList;
	wxClientData *data;
	wxTreeItemId item;
	EventsLog *eventsLog;
	ScenarioControls *sctrl;
	wxTreeListCtrl *list;

	void onAddItem(wxCommandEvent& WXUNUSED(event));
	void InitImageList();

	DECLARE_EVENT_TABLE()

};

//jma342--this class is used to store data for each of the elements of a tree
//this either stores a file or folder as local data
class eventsListItemData : public wxTreeItemData
{
public:

	//jma342--initialises the local variable localdata
	eventsListItemData(const wxString& data) : localData(data) { }

	//jma342--returns the variable localdata
	const wxString GetLocalData() { return localData; }

private:
	wxString localData;
};

#endif