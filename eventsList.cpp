#include "eventsList.h"
#include "wx/artprov.h"

BEGIN_EVENT_TABLE(EventsList, wxScrolled<wxWindow>)

	EVT_TREELIST_ITEM_CONTEXT_MENU(wxID_ANY, EventsList::rightClick)
	EVT_TREELIST_ITEM_ACTIVATED(wxID_ANY, EventsList::doubleClick)
	//EVT_MENU(addItemID,EventsList::onAddItem)
	//EVT_TREE_ITEM_MENU(wxID_ANY, EventsList::rightClick)

END_EVENT_TABLE()

EventsList::EventsList(wxWindow *parent,wxString Column):MyScrolledWindowSmart(parent)
{
	this->parent = parent;
	this->ShowScrollbars(wxSHOW_SB_ALWAYS,wxSHOW_SB_ALWAYS);

	 /*m_treeCtrl = new wxTreeCtrl(this, wxID_ANY,
                                  wxDefaultPosition, wxSize(620,300),

								  wxTL_DEFAULT_STYLE);*/

	this->InitImageList();

	 list = new wxTreeListCtrl(this, wxID_ANY,
                                  wxPoint( 0, 0 ),
									wxSize( 293, 206) ,
                                  wxTL_DEFAULT_STYLE);

	 list->SetImageList(imageList);

	/*m_treeCtrl->AddRoot(Column);*/
	list->AppendColumn(Column);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	//sizer->SetDimension(wxDefaultPosition,wxSize(620,206));
	sizer->Add(list,wxSizerFlags(2).Expand());
	//sizer->Fit(this);
    SetSizer(sizer);


	
}

void EventsList::displayFirstChild()
{
	eventsListItemData *getData =(eventsListItemData*) list->GetItemData(list->GetNextItem(list->GetRootItem()));

	wxMessageBox(getData->GetLocalData());
	
}

void EventsList::expandAllFolders()
{
	for ( wxTreeListItem item = list->GetFirstItem(); item.IsOk(); item = list->GetNextItem(item) )
	{
		eventsListItemData *getData = (eventsListItemData*)list->GetItemData(item);

		if(getData->GetLocalData() == "Folder")
		{
			if(!list->IsExpanded(item))
			{
				list->Expand(item);
			}
		}
	}
}

void EventsList::collapseAllFolders()
{
	for ( wxTreeListItem item = list->GetFirstItem(); item.IsOk(); item = list->GetNextItem(item) )
	{
		eventsListItemData *getData = (eventsListItemData*)list->GetItemData(item);

		if(getData->GetLocalData() == "Folder")
		{
			if(list->IsExpanded(item))
			{
				list->Collapse(item);
			}
		}
	}
}

void EventsList::rightClick(wxTreeListEvent& event)
{

	const wxTreeListItem item = event.GetItem();

	eventsListItemData *getData = (eventsListItemData*)list->GetItemData(item);

    wxMenu menu;
 	
	if(list->GetRootItem().GetID() == event.GetItem().GetID() || getData->GetLocalData() != "File")
	{
		menu.Append(addNewItemID, "Add New Item");
		menu.Append(addNewFolderID,"Add New Folder");

		if(list->GetRootItem().GetID() != event.GetItem().GetID())
			menu.Append(removeItemID, "Remove Folder");
	}
	else if(getData->GetLocalData() == "File")
	{
		menu.Append(administerItemID, "Administer item");
		menu.Append(removeItemID, "Remove Item");
	}

	wxTreeItemData *data = new wxTreeItemData();
	wxTreeListItem lastAddedItem;
	wxString text;

    switch ( list->GetPopupMenuSelectionFromUser(menu) )
    {
        case addNewItemID:

			text= wxGetTextFromUser
                          (
                            "Please enter the title for a new event: ",
							"Capturing title for new event item",
                            "",
                            parent
                          );

			if(!text.empty())
				lastAddedItem = list->AppendItem(item, text,iconFile,-1,new eventsListItemData("File"));

            break;

        case addNewFolderID:

			text= wxGetTextFromUser
                          (
                            "Please enter the title for a new event: ",
							"Capturing title for new event item",
                            "",
                            parent
                          );
			if(!text.empty())
				lastAddedItem = list->AppendItem(item,text,iconFolderClosed,iconFolderOpened,new eventsListItemData("Folder"));

			break;

		case removeItemID:

			if(getData->GetLocalData() == "Folder" && list->GetFirstChild(item).IsOk())
			{
				 wxMessageBox( "Folder must be empty in order to be removed" );
			}
			else
			{
				list->DeleteItem(item);
			}

			break;

		case administerItemID:

			eventsLog->updateLog(eventsLog->EVENT_LIST_MSG,list->GetItemText(item),sctrl->getMilliSecondsElapsed());
			break;


		default:
            wxMessageBox( "Unexpected menu selection" );

        case wxID_NONE:
            return;
    }

}

void EventsList::InitImageList()
{
    wxSize iconSize = wxArtProvider::GetSizeHint(wxART_LIST);
    iconSize = wxSize(16, 16);

    imageList = new wxImageList(iconSize.x, iconSize.y);

    // The order should be the same as for the enum elements.
    static const char* const icons[] =
    {
        wxART_NORMAL_FILE,
        wxART_FOLDER,
        wxART_FOLDER_OPEN
    };

    for ( unsigned n = 0; n < WXSIZEOF(icons); n++ )
    {
        imageList->Add
                     (
                        wxArtProvider::GetIcon(icons[n], wxART_LIST, iconSize)
                     );
    }
}

void EventsList::doubleClick(wxTreeListEvent& event)
{
	eventsListItemData *getData = (eventsListItemData*)list->GetItemData(event.GetItem());

	if(getData->GetLocalData() == "File")
	{
		eventsLog->updateLog(eventsLog->EVENT_LIST_MSG,list->GetItemText(event.GetItem()),sctrl->getMilliSecondsElapsed());
	}
}

void EventsList::setScenarioControls(ScenarioControls *sctrl)
{
	this->sctrl = (ScenarioControls*)sctrl;
}

void EventsList::setEventsLog(EventsLog *eventsLog)
{
	this->eventsLog = (EventsLog*)eventsLog;
}


