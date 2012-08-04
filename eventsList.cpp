#include "eventsList.h"
#include "wx/artprov.h"

//captures the events emitted by the events list 
//associates each id with a given function
BEGIN_EVENT_TABLE(EventsList, wxScrolled<wxWindow>)
	//EVT_TREELIST_ITEM_CONTEXT_MENU(wxID_ANY, EventsList::rightClick)
	EVT_TREELIST_ITEM_ACTIVATED(wxID_ANY, EventsList::doubleClick)
END_EVENT_TABLE()

//intialises the events list
EventsList::EventsList(wxWindow *parent,wxString Column):MyScrolledWindowSmart(parent)
{
	//sets the parent of the events list which is the current frame
	this->parent = parent;

	//indicates that both the horizontal and vertical scroll bars will be displayed
	this->ShowScrollbars(wxSHOW_SB_ALWAYS,wxSHOW_SB_ALWAYS);

	//initialises the image list
	this->InitImageList();

	//intialises the tree that holds the list of events
	list = new wxTreeListCtrl(this, wxID_ANY,
                                  wxPoint( 0, 0 ),
									wxSize( 293, 206) ,
                                  wxTL_DEFAULT_STYLE);

	list->SetImageList(imageList);

	//appends the first and only column of the list which holds the 
	//title of the list
	list->AppendColumn(Column);

	//places the list in a sizer which is necessary for the list to be displayed
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(list,wxSizerFlags(2).Expand());
    SetSizer(sizer);
}

//jma342--used to expand all folders
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

//jma342--used to collapse all folders
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

//jma342---this is used to add elements to the trees(IT IS CURRENTLY NOT BEING USED AND DEEMED NOT NECESSARY BY PROF. FLETCHER)---but it was left in
//to indicate how elements are added to the tree which will be used when the scenario file is loaded
void EventsList::rightClick(wxTreeListEvent& event)
{
	//used to retrieve the tree item that triggered this event
	const wxTreeListItem item = event.GetItem();

	//used to retrieve the data for the item that triggered the event
	//this data is stored in an object
	eventsListItemData *getData = (eventsListItemData*)list->GetItemData(item);

    wxMenu menu;

	//this builds the context menu if the root of the tree or a folder of the tree triggered the event
	if(list->GetRootItem().GetID() == event.GetItem().GetID() || getData->GetLocalData() != "File")
	{
		menu.Append(addNewItemID, "Add New Item");
		menu.Append(addNewFolderID,"Add New Folder");

		if(list->GetRootItem().GetID() != event.GetItem().GetID())
			menu.Append(removeItemID, "Remove Folder");
	}

	//this builds the context menu that is displayed if a file of the tree triggered the event
	else if(getData->GetLocalData() == "File")
	{
		menu.Append(administerItemID, "Administer item");
		menu.Append(removeItemID, "Remove Item");
	}

	wxString text;

	//captrues the option chosen by the user from the trees context menu
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
				//function called to add an item to the tree...the eventsListItemData object indicates the type of item it is
				list->AppendItem(item, text,iconFile,-1,new eventsListItemData("File"));

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
				list->AppendItem(item,text,iconFolderClosed,iconFolderOpened,new eventsListItemData("Folder"));

			break;

		case removeItemID:

			//if the item chosen is a folder it is only removed if it is empty
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
			//when an item in the events list is administered it is logged in the events log.
			//this function call utilises the events log pointer as well as the scenario controls pointer to 
			//get the time stamp
			eventsLog->updateLog(eventsLog->EVENT_LIST_MSG,list->GetItemText(item),sctrl->getScenarioMilliSecondsElapsed());
			break;


		default:
            wxMessageBox( "Unexpected menu selection" );

        case wxID_NONE:
            return;
    }
}

//jma342--used to initialise the image list which consists of images
//that are associated with each of the items(file/folder) added to a tree
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
        imageList->Add(wxArtProvider::GetIcon(icons[n], wxART_LIST, iconSize));
    }
}

//jma342--used to administer items during a scenario
//each administering of an item is logged into the events log
void EventsList::doubleClick(wxTreeListEvent& event)
{
	eventsListItemData *getData = (eventsListItemData*)list->GetItemData(event.GetItem());

	//only file items are administerable
	if(getData->GetLocalData() == "File")
	{
		//when an item in the events list is administered it is logged in the events log.
		//this function call utilises the events log pointer as well as the scenario controls pointer to 
		//get the time stamp
		eventsLog->updateLog(eventsLog->EVENT_LIST_MSG,list->GetItemText(event.GetItem()),sctrl->getScenarioMilliSecondsElapsed());
	}
}

//jma342--initialises the scenario controls pointer
void EventsList::setScenarioControls(ScenarioControls *sctrl)
{
	this->sctrl = (ScenarioControls*)sctrl;
}

//jma342--initialises the events log pointer
void EventsList::setEventsLog(EventsLog *eventsLog)
{
	this->eventsLog = (EventsLog*)eventsLog;
}


