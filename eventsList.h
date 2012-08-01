#include "eventsLog.h"
#include "wx/treectrl.h"
#include "wx/treelist.h"


#ifndef EVENTSLIST_H
#define EVENTSLIST_H

#include "scenarioControls.h"
//namespace vetSimulatorUI
//{
	class EventsList : public MyScrolledWindowSmart
	{
	public:
		EventsList(wxWindow *parent,wxString Column);
		
		//virtual void OnDraw(wxDC& dc);

		//void rightClick(wxTreeEvent& event/*wxTreeListEvent& event*/);
		void rightClick(wxTreeListEvent& event);
		void doubleClick(wxTreeListEvent& event);

		void addItem();
		void administerItem(wxTreeListItem item);
		void removeItem();
		void expandAllFolders();
		void collapseAllFolders();
		void setEventsLog(EventsLog *);
		void setScenarioControls(ScenarioControls *);
		/*void AddItemsRecursively(const wxTreeItemId& idParent,
                                     size_t numChildren,
                                     size_t depth,
                                     size_t folder);
		
		void AddTestItemsToTree(size_t numChildren,
                                    size_t depth);*/
		void displayFirstChild();


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
		wxTreeCtrl *m_treeCtrl;

		void onAddItem(wxCommandEvent& WXUNUSED(event));
		void InitImageList();

		DECLARE_EVENT_TABLE()

	};

	class eventsListItemData : public wxTreeItemData
	{
	public:
		eventsListItemData(const wxString& data) : localData(data) { }

		const wxString GetLocalData() { return localData; }

	private:
		wxString localData;
	};

//}

#endif