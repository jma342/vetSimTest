///////////////////////////////////////////////////////////////////////////////
// Name:        auidemo.cpp
// Purpose:     wxaui: wx advanced user interface - sample/test program
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2005-10-03
// RCS-ID:      $Id$
// Copyright:   (C) Copyright 2005, Kirix Corporation, All Rights Reserved.
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////
#include "auidemo.h"
#include "wx\wfstream.h"

const int DEFAULT_SCREEN_MODE = 0;
const int MANUAL_SCREEN_MODE = 1;
const int PREPROGRAMMED_SCREEN_MODE =2;

//toggle this value to enforce the saving/loading of custom layouts with screen settings
const bool SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS = true;

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    wxFrame* frame = new MyFrame(NULL,
                                 wxID_ANY,
                                 wxT("Vet Simulator"),
                                 wxDefaultPosition,
								 wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
                                 //wxSize(800, 600));
    frame->Show();

    return true;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground)
    EVT_SIZE(MyFrame::OnSize)
    
	/*FileMenuEvents*/
	EVT_MENU(ID_StartScenario, MyFrame::onStartScenario)
	EVT_MENU(ID_MonitorSound, MyFrame::onMonitorSound)
	EVT_MENU(ID_ImportVocalSounds, MyFrame::onImportVocalSounds)
	EVT_MENU(ID_ImportMediaFiles, MyFrame::onImportMediaFiles)
	EVT_MENU(ID_AddMediaFiles, MyFrame::onAddMediaFiles)
	EVT_MENU(ID_Exit, MyFrame::onExit)

	/*ViewMenuEvents*/
	EVT_MENU(ID_PatientInstructorMonitor, MyFrame::onPatientInstructorMonitor)

	/*SimulationMenuEvents*/
	EVT_MENU(ID_StartSimulation, MyFrame::onStartSimulation)
	EVT_MENU(ID_PauseSimulation, MyFrame::onPauseSimulation)
	EVT_MENU(ID_FastForward, MyFrame::onFastForward)
	EVT_MENU(ID_HaltScenario, MyFrame::onHaltScenario)
	EVT_MENU(ID_DefaultScenario, MyFrame::onDefaultScenario)

	/*EditMenuEvents*/
	EVT_MENU(ID_MonitorSetup, MyFrame::onMonitorSetup)
	EVT_MENU(ID_VitalSigns, MyFrame::onVitalSigns)

	/*HelpMenuEvents*/
	EVT_MENU(ID_About, MyFrame::onAbout)
	EVT_MENU(ID_Help, MyFrame::onHelp)

	/*ScreenLayoutMenuEvents*/
	EVT_MENU(ID_DefaultMode, MyFrame::onDefaultMode)
	EVT_MENU(ID_ManualMode, MyFrame::onManualMode)
	EVT_MENU(ID_PreProgrammedMode, MyFrame::onPreProgrammedMode)

	/*CustomizeDefaultScreenLayout*/
	EVT_MENU(ID_mannequin, MyFrame::onMannequin)
	EVT_MENU(ID_instructorPatientMonitor, MyFrame::onInstructorPatientMonitor)
	EVT_MENU(ID_eventsLog, MyFrame::onEventsLog)
	EVT_MENU(ID_scenarioControls, MyFrame::onScenarioControls)
	EVT_AUITOOLBAR_TOOL_DROPDOWN(ID_eventsList, MyFrame::OnDropDownToolbarItem_eventsList)
	EVT_MENU(ID_eventsList_ShowAll, MyFrame::onEventsList_ShowAll)
	EVT_MENU(ID_eventsList_HideAll, MyFrame::onEventsList_HideAll)
	EVT_MENU(ID_eventsList_Expand_All_Folders, MyFrame::onExpandAllEventsLists)
	EVT_MENU(ID_eventsList_ABC, MyFrame::onEventsList_ABC)
	EVT_MENU(ID_eventsList_Misc, MyFrame::onEventsList_Misc)
	EVT_MENU(ID_eventsList_Med, MyFrame::onEventsList_Med)

	EVT_MENU(ID_saveLayout, MyFrame::onSaveCustomLayout)
	EVT_MENU(ID_loadLayout, MyFrame::onLoadCustomLayout)

	EVT_MENU(ID_adminOverwritePresetLayouts, MyFrame::onOverwritePresetLayout)

	EVT_AUI_PANE_CLOSE(MyFrame::OnPaneClose)
	EVT_AUI_PANE_MAXIMIZE(MyFrame::onPaneMaximize)
	EVT_AUI_PANE_RESTORE(MyFrame::onRestorePane)
END_EVENT_TABLE()


MyFrame::MyFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style)
{
    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);

    // set frame icon
    SetIcon(wxIcon(sample_xpm));

    // set up default notebook style
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;

    // create menu
    //wxMenuBar* mb = new wxMenuBar;
	menuBar =  new wxMenuBar;

    //wxMenu* file_menu = new wxMenu;
    //file_menu->Append(wxID_EXIT);
	/*FileMenuEvents*/
	menuFile = new wxMenu;
	menuFile->Append(ID_StartScenario, ("Start Scenario"), "Starts an existing scenario");
	menuFile->Append(ID_ImportVocalSounds, ("Import Vocal Sounds"), "Import of customized .wav files containing vocal sounds");
	menuFile->Append(ID_ImportMediaFiles, ("Import Media Files"), "For the importing of media files");
	menuFile->Append(ID_AddMediaFiles, ("Add Media Files"), "Add media files to a simulation");
	menuFile->Append(ID_saveLayout,("Save Layout"), "Saves Current Screen Layout");
	menuFile->Append(ID_loadLayout,("Load Layout"), "Loads Saved Layout");
	menuFile->Append(ID_Exit, ("E&xit\tAlt-X"),"Exit the application");

	/*SimulationMenuEvents*/
	menuSimulation = new wxMenu;
	menuSimulation->Append(ID_StartSimulation, ("Start Simulation"), "Start the select simulation scenario");
	menuSimulation->Append(ID_PauseSimulation, ("Pause Simulation"), "Pause the current simulation scenario");
	menuSimulation->Append(ID_FastForward, ("Fast Forward Simulation "), "Fast Forward the current simulation scenario");
	menuSimulation->Append(ID_HaltScenario, ("Halt Simulation"), "Halt the current simulation scenario");
	menuSimulation->Append(ID_DefaultScenario, ("Default Simulation"), "Select the default simulation scenario");
	menuSimulation->Append(ID_MonitorSound, ("Monitor Sound"), "Enables or disables simulation sounds");

	/*EditMenuEvents*/
	menuEdit = new wxMenu;
	menuEdit->Append(ID_MonitorSetup, ("Patient/Monitor Setup"), "Configure Patient Monitor Setup");
	menuEdit->Append(ID_VitalSigns, ("Vital Setups"), "Change vital signs of mannequin");

	/*HelpMenuEvents*/
	menuHelp = new wxMenu;
	menuHelp->Append(ID_About, ("&About...\tF1"),"About dialog");
	menuHelp->Append(ID_Help, ("&Help...\tF2"),"User Manual/Help");

	/*ScreenLayoutEvents*/
	menuScreenLayout = new wxMenu;
	menuScreenLayout->Append(ID_DefaultMode, ("Default Mode"),"Select the default screen layout");
	menuScreenLayout->Append(ID_ManualMode, ("Manual Mode"),"Select screen Layout for Manual Mode");
	menuScreenLayout->Append(ID_PreProgrammedMode, ("PreProgrammed Mode"),"Select screen Layout for PreProgrammed Mode");

    wxMenu* help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSimulation, _("&Simulation"));
	menuBar->Append(menuEdit, _("&Edit"));
	menuBar->Append(menuScreenLayout,_("Preset Layouts"));
	menuBar->Append(menuHelp, _("&Help"));
     

    SetMenuBar( menuBar);

    CreateStatusBar();
    GetStatusBar()->SetStatusText(_("Ready"));


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    //SetMinSize(wxSize(400,300));
	
	/*wxString screenSize;
	wxClientDC *screen = new wxClientDC(this);

	screenSize.Printf("System Settings: Height %d Width %d",
		wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));

	wxMessageBox(screenSize);*/

	SetBestSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

	SetMaxSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
	//SetMaxSize(wxSize(1280,800));

	this->Maximize();

	MyFrame::loadAllPresetLayouts();

	DefaultScreenLayout();//the screen needs to be setup before it can be loaded with a new perspective...not sure why

	//loads the saved default screen layout if one is found otherwise the above hard coded default screen layout is used
	if(presetLayouts[DEFAULT_SCREEN_MODE].length() > 0)
	{
		m_mgr.LoadPerspective(presetLayouts[DEFAULT_SCREEN_MODE],true);

		//updates the layout to fit the current screen size if its dimensions weren't built for this screen
		if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL || presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL)
		{
			presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
			presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
		}
		else if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
			presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
		{
			updateLayoutForCurrentScreenSize(DEFAULT_SCREEN_MODE);
		}
	}

	setSubScreensOnToolBar();

}

void MyFrame::updateLayoutForCurrentScreenSize(int chosenScreenLayout)
{
	wxAuiPaneInfoArray *allPanes = &(m_mgr.GetAllPanes());
	int adjustedWidth = 0;
	int adjustedHeight = 0;

	int size = m_mgr.GetPane("eventsList_Misc").best_size.GetWidth();//best_size.GetWidth();

	for(int pane = 0;pane < allPanes->size();pane++)
	{
		if((*allPanes)[pane].name != "SubScreensBar" && (*allPanes)[pane].name != "OverwritePresetLayoutBar")
		{
			/*int hold = presetLayoutsNativeScreenWidth[chosenScreenLayout];*/
			adjustedWidth = ceil((*allPanes)[pane].best_size.GetWidth() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/(double)presetLayoutsNativeScreenWidth[chosenScreenLayout]));
			adjustedHeight = ceil((*allPanes)[pane].best_size.GetHeight() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/(double)presetLayoutsNativeScreenHeight[chosenScreenLayout]));

			//attempting to resize each pane based on the current screen size and the
			//screen size each pane was initially built for...on searching the internet it appears
			//that the only way for this to work is to fix each pane update the screen and then
			//make each pane resizable again followed by another screen update...this seems convoluted to 
			//me...I believe there is a better way I just haven't been able to find one
			(*allPanes)[pane].BestSize(adjustedWidth,adjustedHeight);
			(*allPanes)[pane].Fixed();

			m_mgr.Update();

			(*allPanes)[pane].Resizable();

			m_mgr.Update();
		}	
	}

	size = m_mgr.GetPane("eventsList_Misc").best_size.GetWidth();//.best_size.GetWidth();

	presetLayoutsNativeScreenWidth[chosenScreenLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	presetLayoutsNativeScreenHeight[chosenScreenLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	/*m_mgr.Update();*/

	presetLayouts[chosenScreenLayout] = m_mgr.SavePerspective();

}

void MyFrame::updateCustomLayoutForCurrentScreenSize()
{
	wxAuiPaneInfoArray *allPanes = &(m_mgr.GetAllPanes());
	int adjustedWidth = 0;
	int adjustedHeight = 0;

	int size = m_mgr.GetPane("eventsList_Misc").best_size.GetWidth();//best_size.GetWidth();

	for(int pane = 0;pane < allPanes->size();pane++)
	{
		if((*allPanes)[pane].name != "SubScreensBar" && (*allPanes)[pane].name != "OverwritePresetLayoutBar")
		{
			/*int hold = presetLayoutsNativeScreenWidth[chosenScreenLayout];*/
			adjustedWidth = ceil((*allPanes)[pane].best_size.GetWidth() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/(double)customLayoutNativeScreenWidth));
			adjustedHeight = ceil((*allPanes)[pane].best_size.GetHeight() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/(double)customLayoutNativeScreenHeight));

			//attempting to resize each pane based on the current screen size and the
			//screen size each pane was initially built for...on searching the internet it appears
			//that the only way for this to work is to fix each pane update the screen and then
			//make each pane resizable again followed by another screen update...this seems convoluted to 
			//me...I believe there is a better way I just haven't been able to find one
			(*allPanes)[pane].BestSize(adjustedWidth,adjustedHeight);
			(*allPanes)[pane].Fixed();

			m_mgr.Update();

			(*allPanes)[pane].Resizable();

			m_mgr.Update();
		}	
	}

	size = m_mgr.GetPane("eventsList_Misc").best_size.GetWidth();//.best_size.GetWidth();

	customLayoutNativeScreenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	customLayoutNativeScreenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

}

void MyFrame::DefaultScreenLayout()
{
	/*eventsList and eventsLog*/
	eventsList_ABC = new EventsList(this,"ABC");
	eventsList_Misc = new EventsList(this,"Miscellaneous");
	eventsList_Med = new EventsList(this,"Medication");

	eventsLog = new EventsLog(this);
	
	m_mgr.AddPane(eventsLog, wxAuiPaneInfo().
		Name(wxT("eventsLog")).Caption(wxT("Events Log")).CloseButton().MaximizeButton().MinimizeButton().PinButton().Left().Position(0).BestSize(wxSize( 450, 206)));

	/*scenario Controls*/
	scenarioControls = new ScenarioControls(this);
	m_mgr.AddPane(scenarioControls, wxAuiPaneInfo().
		Name(wxT("scenarioControls")).Caption(wxT("Scenario Controls")).CloseButton().MaximizeButton().MinimizeButton().PinButton()
		.Left().Position(1).BestSize(wxSize( 185, 161)));

	/*scenario Controls*/

	m_mgr.AddPane(eventsList_ABC, wxAuiPaneInfo().
		Name(wxT("eventsList_ABC")).Caption(wxT("Events List")).CloseButton().MaximizeButton().Center().MinimizeButton().PinButton().
		Row(0).BestSize(wxSize( 293, 206)));

	m_mgr.AddPane(eventsList_Misc, wxAuiPaneInfo().
		Name(wxT("eventsList_Misc")).Caption(wxT("Events List")).CloseButton().MaximizeButton().Center().MinimizeButton().PinButton().Row(1)
		.BestSize(wxSize( 293, 206)));


	m_mgr.AddPane(eventsList_Med, wxAuiPaneInfo().
		Name(wxT("eventsList_Med")).Caption(wxT("Events List")).CloseButton().MaximizeButton().MinimizeButton().Center().PinButton().Row(2)
		.BestSize(wxSize( 293, 206)));

	/*eventsList and eventsLog*/

	/*manneqin and instructor/patient control*/
	mannequin = new MyScrolledWindowSmart(this);
	instructorPatientMonitor = new MyScrolledWindowSmart(this);

	m_mgr.AddPane(mannequin, wxAuiPaneInfo().
		Name(wxT("mannequin")).Caption(wxT("Mannequin Avatar")).CloseButton().MaximizeButton().MinimizeButton().PinButton().
		BestSize(wxSize( 450, 206)).Top().Position(0));

	m_mgr.AddPane(instructorPatientMonitor, wxAuiPaneInfo().
		Name(wxT("instructorPatientMonitor")).Caption(wxT("Instructor Patient Monitor")).CloseButton().MaximizeButton().MinimizeButton().PinButton()
		.BestSize(wxSize( 450, 206)).Top().Position(1));

	/*manneqin and instructor/patient control*/

	/*subscreens toolbar*/
	subScreensBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE |
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
    subScreensBar->SetToolBitmapSize(wxSize(16,16));
    wxBitmap subScreensBar_bmp1 = wxArtProvider::GetBitmap(wxART_NEW, wxART_OTHER, wxSize(16,16));

    subScreensBar->AddTool(ID_mannequin, wxT("Mannequin"),subScreensBar_bmp1,wxT("Mannequin"),wxITEM_CHECK);
	subScreensBar->AddTool(ID_instructorPatientMonitor, wxT("Instructor/Patient Monitor"),subScreensBar_bmp1,wxT("Instructor/Patient Monitor"),wxITEM_CHECK);
	subScreensBar->AddTool(ID_eventsLog, wxT("Events Log"),subScreensBar_bmp1,wxT("Events Log"),wxITEM_CHECK);
	subScreensBar->AddTool(ID_eventsList, wxT("Events List"),subScreensBar_bmp1,wxT("Events List"),wxITEM_CHECK);
	subScreensBar->AddTool(ID_scenarioControls, wxT("Scenario Controls"),subScreensBar_bmp1,wxT("Scenario Controls"),wxITEM_CHECK);
    subScreensBar->SetToolDropDown(ID_eventsList, true);

	subScreensBar->ToggleTool(ID_mannequin,true);
	subScreensBar->ToggleTool(ID_instructorPatientMonitor,true);
	subScreensBar->ToggleTool(ID_eventsLog,true);
	subScreensBar->ToggleTool(ID_eventsList,true);
	subScreensBar->ToggleTool(ID_scenarioControls,true);

	adminOverWritePresetLayoutBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE |
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
    adminOverWritePresetLayoutBar->SetToolBitmapSize(wxSize(16,16));
    wxBitmap adminOverWritePresetLayoutBar_bmp1 = wxArtProvider::GetBitmap(wxART_NEW, wxART_OTHER, wxSize(16,16));

    adminOverWritePresetLayoutBar->AddTool(ID_adminOverwritePresetLayouts, wxT("Overwrite Preset Layouts"),adminOverWritePresetLayoutBar_bmp1,wxT("Overwrite Preset Layouts"),wxITEM_CHECK);
	
    layoutChoice = new wxChoice(adminOverWritePresetLayoutBar,ID_adminOverwritePresetLayouts_choice);

    layoutChoice->AppendString(wxT("Default Layout"));
    layoutChoice->AppendString(wxT("Manual Mode"));
	layoutChoice->AppendString(wxT("PreProgrammed Mode"));
    adminOverWritePresetLayoutBar->AddControl(layoutChoice);

    m_mgr.AddPane(subScreensBar, wxAuiPaneInfo().
                  Name(wxT("SubScreensBar")).Caption(wxT("SubScreens Toolbar")).
                  ToolbarPane().Top());

	m_mgr.AddPane(adminOverWritePresetLayoutBar, wxAuiPaneInfo().
                  Name(wxT("OverwritePresetLayoutBar")).Caption(wxT("Preset Layout Toolbar")).
                  ToolbarPane().Top());
	/*subscreens toolbar*/

	//create the eventsList popup menu
	menuPopup_EventsList = new wxMenu;

    wxBitmap bmp = wxArtProvider::GetBitmap(wxART_NEW, wxART_OTHER, wxSize(16,16));

	menuPopup_EventsList->AppendCheckItem(ID_eventsList_ABC,"EventsList ABC");
	menuPopup_EventsList->AppendCheckItem(ID_eventsList_Med,"EventsList Medicine");
	menuPopup_EventsList->AppendCheckItem(ID_eventsList_Misc,"EventsList Miscellaneous");
	menuPopup_EventsList->AppendCheckItem(ID_eventsList_Expand_All_Folders,"Expand All EventsLists Folders");
	menuPopup_EventsList->Append(ID_eventsList_ShowAll,"Show All EventsLists");
	menuPopup_EventsList->Append(ID_eventsList_HideAll,"Hide All EventsLists");

	//setting utitlity pointers used by screens to reference pointers to other screens
	((ScenarioControls*)scenarioControls)->setEventsLog((EventsLog*)eventsLog);
		
	((EventsList*)eventsList_ABC)->setEventsLog((EventsLog*) eventsLog);
	((EventsList*)eventsList_ABC)->setScenarioControls((ScenarioControls*)scenarioControls);

	((EventsList*)eventsList_Misc)->setEventsLog((EventsLog*) eventsLog);
	((EventsList*)eventsList_Misc)->setScenarioControls((ScenarioControls*)scenarioControls);

	((EventsList*)eventsList_Med)->setEventsLog((EventsLog*) eventsLog);
	((EventsList*)eventsList_Med)->setScenarioControls((ScenarioControls*)scenarioControls);

	 m_mgr.Update();
}

MyFrame::~MyFrame()
{
    m_mgr.UnInit();
}

void MyFrame::onDefaultMode(wxCommandEvent& WXUNUSED(event))
{
	m_mgr.LoadPerspective(presetLayouts[DEFAULT_SCREEN_MODE]);
	setSubScreensOnToolBar();
	m_mgr.Update();

	//updates the layout to fit the current screen size if its dimensions weren't built for this screen
	if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL || presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL)
	{
		presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	}
	else if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
		presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
	{
		updateLayoutForCurrentScreenSize(DEFAULT_SCREEN_MODE);
	}
}

void MyFrame::onManualMode(wxCommandEvent& WXUNUSED(event))
{
	m_mgr.LoadPerspective(presetLayouts[MANUAL_SCREEN_MODE]);
	setSubScreensOnToolBar();
	m_mgr.Update();

	//updates the layout to fit the current screen size if its dimensions weren't built for this screen
	if(presetLayoutsNativeScreenWidth[MANUAL_SCREEN_MODE] == NULL || presetLayoutsNativeScreenHeight[MANUAL_SCREEN_MODE] == NULL)
	{
		presetLayoutsNativeScreenWidth[MANUAL_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		presetLayoutsNativeScreenHeight[MANUAL_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	}
	else if(presetLayoutsNativeScreenWidth[MANUAL_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
	presetLayoutsNativeScreenHeight[MANUAL_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
	{
		updateLayoutForCurrentScreenSize(MANUAL_SCREEN_MODE);
	}
}

void MyFrame::onPreProgrammedMode(wxCommandEvent& WXUNUSED(event))
{
	m_mgr.LoadPerspective(presetLayouts[PREPROGRAMMED_SCREEN_MODE]);
	setSubScreensOnToolBar();
	m_mgr.Update();

	//updates the layout to fit the current screen size if its dimensions weren't built for this screen
	if(presetLayoutsNativeScreenWidth[PREPROGRAMMED_SCREEN_MODE] == NULL || presetLayoutsNativeScreenHeight[PREPROGRAMMED_SCREEN_MODE] == NULL)
	{
		presetLayoutsNativeScreenWidth[PREPROGRAMMED_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		presetLayoutsNativeScreenHeight[PREPROGRAMMED_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	}
	else if(presetLayoutsNativeScreenWidth[PREPROGRAMMED_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
	presetLayoutsNativeScreenHeight[PREPROGRAMMED_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
	{
		updateLayoutForCurrentScreenSize(PREPROGRAMMED_SCREEN_MODE);
	}

}

void MyFrame::setSubScreensOnToolBar()
{
	if(this->mannequin->IsShown())
	{
		subScreensBar->ToggleTool(ID_mannequin,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_mannequin,false);
	}

	if(this->instructorPatientMonitor->IsShown())
	{
		subScreensBar->ToggleTool(ID_instructorPatientMonitor,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_instructorPatientMonitor,false);
	}

	if(this->eventsLog->IsShown())
	{
		subScreensBar->ToggleTool(ID_eventsLog,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_eventsLog,false);
	}

	if(this->scenarioControls->IsShown())
	{
		subScreensBar->ToggleTool(ID_scenarioControls,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_scenarioControls,false);
	}

	if(this->eventsList_ABC->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_ABC,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_ABC,false);
	}

	if(this->eventsList_Med->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_Med,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_Med,false);
	}

	if(this->eventsList_Misc->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_Misc,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_Misc,false);
	}

	if(this->eventsList_ABC->IsShown() || this->eventsList_Med->IsShown() || this->eventsList_Misc->IsShown())
	{
		subScreensBar->ToggleTool(ID_eventsList,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_eventsList,false);
	}
	
}

void MyFrame::onMannequin(wxCommandEvent& WXUNUSED(event))
{
	toggleMannequin();
	m_mgr.Update();
}

void MyFrame::toggleMannequin()
{
	//if(this->menuBar->IsChecked(ID_mannequin))
	if(subScreensBar->GetToolToggled(ID_mannequin))
	{
		
		m_mgr.GetPane("mannequin").Show();
				
	}
	else
	{
		m_mgr.GetPane("mannequin").Hide();
	}

}

void MyFrame::onInstructorPatientMonitor(wxCommandEvent& WXUNUSED(event))
{
	toggleInstructorPatientMonitor();
	m_mgr.Update();
}

void MyFrame::toggleInstructorPatientMonitor()
{
	//if(this->menuBar->IsChecked(ID_instructorPatientMonitor))
	if(subScreensBar->GetToolToggled(ID_instructorPatientMonitor))
	{
		
		m_mgr.GetPane("instructorPatientMonitor").Show();
				
	}
	else
	{
		m_mgr.GetPane("instructorPatientMonitor").Hide();
	}

}

void MyFrame::onEventsLog(wxCommandEvent& WXUNUSED(event))
{
	toggleEventsLog();
	m_mgr.Update();
}

void MyFrame::toggleEventsLog()
{
	//if(this->menuBar->IsChecked(ID_eventsLog))
	if(subScreensBar->GetToolToggled(ID_eventsLog))
	{
		m_mgr.GetPane("eventsLog").Show();			
	}
	else
	{
		m_mgr.GetPane("eventsLog").Hide();
	}

}

void MyFrame::onEventsList_ABC(wxCommandEvent& WXUNUSED(event))
{

	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_ABC))
	{
		m_mgr.GetPane("eventsList_ABC").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_ABC").Hide();
	}

	//m_mgr.Update();
}

void MyFrame::onEventsList_Med(wxCommandEvent& WXUNUSED(event))
{

	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Med))
	{
		m_mgr.GetPane("eventsList_Med").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_Med").Hide();
	}

	//m_mgr.Update();
}

void MyFrame::onEventsList_Misc(wxCommandEvent& WXUNUSED(event))
{

	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
	{
		m_mgr.GetPane("eventsList_Misc").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_Misc").Hide();
	}

	//m_mgr.Update();
}

void MyFrame::onEventsList_ShowAll(wxCommandEvent& WXUNUSED(event))
{

	//toggleEventsList();

	subScreensBar->ToggleTool(ID_eventsList,true);

	m_mgr.GetPane("eventsList_ABC").Show();			
	m_mgr.GetPane("eventsList_Med").Show();
	m_mgr.GetPane("eventsList_Misc").Show();

	menuPopup_EventsList->Check(ID_eventsList_ABC,true);
	menuPopup_EventsList->Check(ID_eventsList_Med,true);
	menuPopup_EventsList->Check(ID_eventsList_Misc,true);

	//m_mgr.Update();
}

void MyFrame::onEventsList_HideAll(wxCommandEvent& WXUNUSED(event))
{

	//toggleEventsList();

	subScreensBar->ToggleTool(ID_eventsList,false);

	m_mgr.GetPane("eventsList_ABC").Hide();			
	m_mgr.GetPane("eventsList_Med").Hide();
	m_mgr.GetPane("eventsList_Misc").Hide();

	menuPopup_EventsList->Check(ID_eventsList_ABC,false);
	menuPopup_EventsList->Check(ID_eventsList_Med,false);
	menuPopup_EventsList->Check(ID_eventsList_Misc,false);

	//m_mgr.Update();
}

void MyFrame::onExpandAllEventsLists(wxCommandEvent& WXUNUSED(event))
{
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Expand_All_Folders))
	{
		((EventsList*)eventsList_ABC)->expandAllFolders();
		((EventsList*)eventsList_Misc)->expandAllFolders();
		((EventsList*)eventsList_Med)->expandAllFolders();
	}
	else
	{
		((EventsList*)eventsList_ABC)->collapseAllFolders();
		((EventsList*)eventsList_Misc)->collapseAllFolders();
		((EventsList*)eventsList_Med)->collapseAllFolders();
	}
}

void MyFrame::toggleEventsList()
{
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
	{
		m_mgr.GetPane("eventsList_Misc").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_Misc").Hide();
	}

	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Med))
	{
		m_mgr.GetPane("eventsList_Med").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_Med").Hide();
	}

	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
	{
		m_mgr.GetPane("eventsList_ABC").Show();	
	}
	else
	{
		m_mgr.GetPane("eventsList_ABC").Hide();
	}

}

void MyFrame::onScenarioControls(wxCommandEvent& WXUNUSED(event))
{
	toggleScenarioControls();
	m_mgr.Update();
}

void MyFrame::toggleScenarioControls()
{
	//if(this->menuBar->IsChecked(ID_scenarioControls))
	if(subScreensBar->GetToolToggled(ID_scenarioControls))
	{
		m_mgr.GetPane("scenarioControls").Show();			
	}

	else
	{
		m_mgr.GetPane("scenarioControls").Hide();			
	}

}

void MyFrame::onOverwritePresetLayout(wxCommandEvent& WXUNUSED(event))
{
	wxString chosenPresetLayout = this->layoutChoice->GetString(this->layoutChoice->GetSelection());

	if(chosenPresetLayout.Length() > 0)
	{
		 wxMessageDialog dialog(this,
			 " Do you want to overwrite " + chosenPresetLayout + "?",
							   "Overwrite Preset Layout",
							   wxCENTER |
							   wxYES_NO | wxICON_QUESTION);


		switch ( dialog.ShowModal() )
		{
			case wxID_YES:
				overWritePresetLayout(this->layoutChoice->GetSelection());
				break;

			case wxID_NO:
				wxMessageBox("Overwrite of " + chosenPresetLayout + "cancelled.");
				break;

			default:
				wxLogError(wxT("Unexpected wxMessageDialog return code!"));
		}
	
	}

	this->adminOverWritePresetLayoutBar->ToggleTool(ID_adminOverwritePresetLayouts,false);
	m_mgr.Update();
	
}

void MyFrame::overWritePresetLayout(int chosenPresetLayout)
{        
	wxString filePathLayout;
	wxString filePathNativeScreen;
    // save the current contents in the file;
    // this can be done with e.g. wxWidgets output streams:

	if(!wxDir::Exists("PresetLayouts"))
	{
		wxDir::Make("PresetLayouts");
	}

	int layoutID = -1;

	if(chosenPresetLayout == DEFAULT_SCREEN_MODE)
	{
		filePathLayout = "defaultLayout.txt";
		filePathNativeScreen = "defaultLayoutNativeScreen.txt";
		layoutID = ID_DefaultMode;
	}
	else if(chosenPresetLayout == MANUAL_SCREEN_MODE)
	{
		filePathLayout = "manualMode.txt";
		filePathNativeScreen = "manualModeNativeScreen.txt";
		layoutID = ID_ManualMode;
	}
	else if(chosenPresetLayout == PREPROGRAMMED_SCREEN_MODE)
	{
		filePathLayout = "preProgrammedMode.txt";
		filePathNativeScreen = "preProgrammedModeNativeScreen.txt";
		layoutID = ID_PreProgrammedMode;
	}

	//write layout to file
    wxFileOutputStream outputLayout("PresetLayouts\\"+filePathLayout);

	wxTextOutputStream out(outputLayout);
	out.WriteString(m_mgr.SavePerspective());
		
	outputLayout.Close();

	presetLayouts[chosenPresetLayout] = m_mgr.SavePerspective();

	//write the native screen settings for the layout to file
	wxFileOutputStream outputNativeScreen("PresetLayouts\\"+filePathNativeScreen);

		
	wxTextOutputStream outNativeScreen(outputNativeScreen);
	outNativeScreen.WriteDouble((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_X));
	outNativeScreen.WriteString("\n");
	outNativeScreen.WriteDouble((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));
		
	outputNativeScreen.Close();

	
	presetLayoutsNativeScreenWidth[chosenPresetLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	presetLayoutsNativeScreenHeight[chosenPresetLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	if(!this->menuBar->IsEnabled(layoutID))
	{	
		this->menuBar->Enable(layoutID,true);
	}
	
}

void MyFrame::loadAllPresetLayouts()
{
	wxString presetLayoutFiles[] = {"defaultLayout.txt","manualMode.txt","preProgrammedMode.txt"};
	wxString presetLayoutNativeScreenFiles[] = {"defaultLayoutNativeScreen.txt","manualModeNativeScreen.txt","preProgrammedModeNativeScreen.txt"};
	wxString failedLayouts = "";
	int layoutsFailed = 1;

	for(int count = 0; count < 3;count++)
	{
		// proceed loading the file chosen by the user;
		// this can be done with e.g. wxWidgets input streams:
		wxString layoutFilePath = "PresetLayouts\\"+presetLayoutFiles[count];
		wxString nativeScreenfilePath = "PresetLayouts\\"+presetLayoutNativeScreenFiles[count];

		wxString layoutsFailed_str;

		layoutsFailed_str.Printf("%d",layoutsFailed);

		if (!wxFile::Exists(layoutFilePath)/*!layoutInputStream.IsOk()*/)
		{
			//wxMessageBox("Cannot open file '%s'.", filePath);

			if(count == 0)
			{
				this->menuScreenLayout->Enable(ID_DefaultMode,false);
				failedLayouts = "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}
			else if (count == 1)
			{
				this->menuScreenLayout->Enable(ID_ManualMode,false);
				failedLayouts += "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}
			else if(count == 2)
			{
				this->menuScreenLayout->Enable(ID_PreProgrammedMode,false);
				failedLayouts += "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}

			layoutsFailed++;

		}

		else
		{
			wxFileInputStream layoutInputStream(layoutFilePath);
			wxTextInputStream inLayout(layoutInputStream);
			presetLayouts[count] = inLayout.ReadLine();

			
			//if there are no native screen metrics then just use the metrics
			//for the current screen
			if(!wxFile::Exists(nativeScreenfilePath)/*!nativeScreenInputStream.IsOk()*/)
			{
				presetLayoutsNativeScreenWidth[count] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
				presetLayoutsNativeScreenHeight[count] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
			}
			else
			{
				wxFileInputStream nativeScreenInputStream(nativeScreenfilePath);
				wxTextInputStream inLayoutNativeScreen(nativeScreenInputStream);
				presetLayoutsNativeScreenWidth[count] = (int)inLayoutNativeScreen.ReadDouble();
				//inLayoutNativeScreen.ReadLine();
				presetLayoutsNativeScreenHeight[count] = (int)inLayoutNativeScreen.ReadDouble();
			}
			//perspective = in.ReadLine();
		}
	}

	if(failedLayouts.length() > 0)
	{
		wxMessageBox("The following preset layouts failed to load: \n " + failedLayouts + 
			"\n\nPlease place the necessary files in the PresetLayouts folder at this location:\n\n" + wxGetCwd() + 
			" \n\nOR\n\nCreate the missing layouts by using the Overwrite Preset Layouts toolbar");
	}
}


void MyFrame::onSaveCustomLayout(wxCommandEvent& WXUNUSED(event))
{
	if(SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS)
	{
		saveCustomLayoutWithScreenSettings();
	}
	else
	{
		saveCustomLayoutWithOUTScreenSettings();
	}	
}

void MyFrame::saveCustomLayoutWithOUTScreenSettings()
{
		wxFileDialog 
            saveFileDialog(this, _("Save Screen Layout"), "", "",
                           "Screen Layout files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
        
	
    // save the current contents in the file;
	wxFileOutputStream output_stream(saveFileDialog.GetPath());

    if (!output_stream.IsOk())
    {
        wxMessageBox("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }

		
	wxTextOutputStream out(output_stream);
	out.WriteString(m_mgr.SavePerspective());
		
	output_stream.Close();

}

void MyFrame::saveCustomLayoutWithScreenSettings()
{
	wxFileDialog 
            saveFileDialog(this, _("Save Screen Layout"), "", "",
                           "Screen Layout files (*.txt)|*.txt", wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
    
	//prevents users from entering underscores into filenames that are new
	//the system uses underscores to extract the screen settings under which the layout was created
	if(saveFileDialog.GetFilename().Find('_') >= 0 && !wxFile::Exists(saveFileDialog.GetPath()))
	{
		wxMessageBox("Please create filenames without underscores(_)...Save Operation Cancelled");
		return;
	}

	wxRegEx extractDimensions("_[0-9]+_[0-9]+");

	//extractDimensions.GetMatch(openFileDialog.GetPath());
	wxString fileName;
	wxString match;

	//if chosen file path doesn't consist of the
	//naming convention with the screen settings they are
	//inserted here
	if(!extractDimensions.Matches(saveFileDialog.GetFilename()))
	{
		wxString insertResolution;
		wxString path;
		wxString name;
		wxString ext;

		wxFileName::SplitPath(saveFileDialog.GetPath(),&path,&name,&ext);

		insertResolution.Printf("_%d_%d",wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));

		fileName += path + "\\" + name + insertResolution + "." + ext;

	}

	//if the chosen file path consists of screen settings that don't match the existing one
	//they are updated here
	else if(extractDimensions.Matches(saveFileDialog.GetFilename()))
	{
		match = extractDimensions.GetMatch(saveFileDialog.GetPath());

		//gets rid of first underscore
		wxString extractEachDimension = match.substr(1,match.Length());

		int width = atoi(extractEachDimension.substr(0,extractEachDimension.Find('_')));
		int height = atoi(extractEachDimension.substr(extractEachDimension.Find('_') + 1,extractEachDimension.Find('_') + 1 - extractEachDimension.Length()));	

		//extracts the width and height values of the chosen file path
		
		//checks whether screen settings of the chosen file match the current screen settings
		if(width != wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || height != wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
		{
			wxString insertResolution;
			wxString path;
			wxString name;
			wxString ext;

			wxFileName::SplitPath(saveFileDialog.GetPath(),&path,&name,&ext);

			wxString newName = name.substr(0,name.Find('_'));

			insertResolution.Printf("_%d_%d",wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));

			fileName += path + "\\" + newName + insertResolution + "." + ext;
		}
		else
		{
			fileName = saveFileDialog.GetPath();
		}
	}
	else
	{
		fileName = saveFileDialog.GetPath();
	}

	long choice = -1;

	//executes if the user chosen path being written to already exists and
	//it consists of the dimensions for the current screen setting
	if(wxFile::Exists(saveFileDialog.GetPath()) && fileName == saveFileDialog.GetPath())
	{
		wxMessageDialog dialog(this,
			"Do you want to overwrite " + saveFileDialog.GetFilename() + " ?",
						"Overwrite Custom Layout",
						wxCENTER |
						wxYES_NO | wxICON_QUESTION);

		choice = dialog.ShowModal();

		if(choice == wxID_YES)
		{
			wxRemoveFile(saveFileDialog.GetPath());
		}

		else if(choice == wxID_NO)
		{
			wxMessageBox("Save operation cancelled.");
			return;
		}
	}

	//executes if the user path chosen exists but the filename created by the system doesn't
	else if(wxFile::Exists(saveFileDialog.GetPath()) && !wxFile::Exists(fileName))
	{
		wxString path;
		wxString name;
		wxString ext;

		//simply used to extract the name for display in the following dialog
		wxFileName::SplitPath(fileName,&path,&name,&ext);

		wxMessageDialog dialog(this,
			 saveFileDialog.GetFilename() + " already exists.\n\n"
			"Option 1 - Overwrite " + saveFileDialog.GetFilename() +"\n\n" + 
			"Option 2 - Keep " + saveFileDialog.GetFilename() + " and new file(" + name + ")  being created with the current layout.",
						"Overwrite Custom Layout",
						wxCENTER |
						wxYES_NO | wxCANCEL | wxICON_QUESTION);

		if ( dialog.SetYesNoCancelLabels
        (
         "Option 1",
         "Option 2",
         "Cancel"
         ) )

		choice = dialog.ShowModal();

		///option 1
		if(choice == wxID_YES)
		{
			wxRemoveFile(saveFileDialog.GetPath());
		}

		//NB-nothing is done here for option 2 as the file that already exists will remain as is and the new file
		//will be created which is done futher down in the code

		//option 3
		else if(choice == wxID_CANCEL)
		{
			wxMessageBox("Save operation cancelled.");
			return;
		}
	}
	//executes if the user chosen path being written to already exists but
	//it didn't consist of the current screen dimensions therefore a new filename
	//was created that is different to the chosen path
	else if(wxFile::Exists(saveFileDialog.GetPath()) && wxFile::Exists(fileName))
	{
		wxString path;
		wxString name;
		wxString ext;

		//simply used to extract the name for display in the following dialog
		wxFileName::SplitPath(fileName,&path,&name,&ext);
		
		//it is possible that the path chosen to be overwritten and the
		//system built filename both exist...the user is given the options
		//of 
		//1. removing the chosen path and overwriting the system created filename
		//2. keeping the path and overwriting the system created filename
		//3. cancelling the entire operation and leaving the 2 files untouched
		wxMessageDialog dialog(this,
			"Both " + saveFileDialog.GetFilename() + " and " +  name + "." + ext + " exists.\n\n " + 
			"Option 1 - Remove " + saveFileDialog.GetFilename() + " AND " +  "overwrite " + name + "." + ext + "\n\n" + 
			"Option 2 - Keep " + saveFileDialog.GetFilename() + " AND " + " overwrite " + name + "." + ext,
						"Overwrite Custom Layout",
						wxCENTER |
						wxYES_NO | wxCANCEL | wxICON_QUESTION);

		//changes the labels of the yes no cancel buttons
		dialog.SetYesNoCancelLabels
        (
         "Option 1",
         "Option 2",
         "Cancel"
		);

		choice = dialog.ShowModal();

		//option 1
		if(choice == wxID_YES)
		{
			wxRemoveFile(fileName);
			wxRemoveFile(saveFileDialog.GetPath());
		}

		//option 2
		else if(choice == wxID_NO)
		{
			wxRemoveFile(fileName);
		}

		//option 3
		else if(choice == wxID_CANCEL)
		{
			wxMessageBox("Save operation cancelled.");
			return;
		}
	}

    // save the current contents in the file;
	wxFileOutputStream output_stream(fileName);

    if (!output_stream.IsOk())
    {
        wxMessageBox("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }

		
	wxTextOutputStream out(output_stream);
	out.WriteString(m_mgr.SavePerspective());
		
	output_stream.Close();
}

void MyFrame::onLoadCustomLayout(wxCommandEvent& WXUNUSED(event))
{ 
	if(SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS)
	{
		loadCustomLayoutWithScreenSettings();
	}
	else
	{
		loadCustomLayoutWithOUTScreenSettings();
	}	
}

void MyFrame::loadCustomLayoutWithOUTScreenSettings()
{
	    wxFileDialog 
        openFileDialog(this, _("Load Screen Layout"), "", "",
                        "Screen Layouts (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
        
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());

    if (!input_stream.IsOk())
    {
        wxMessageBox("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

	wxString perspective = "";

	wxTextInputStream in(input_stream);
	perspective = in.ReadLine();

	m_mgr.LoadPerspective(perspective);
	setSubScreensOnToolBar();

	m_mgr.Update();
}

void MyFrame::loadCustomLayoutWithScreenSettings()
{
	    wxFileDialog 
        openFileDialog(this, _("Load Screen Layout"), "", "",
                        "Screen Layouts (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
        
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());

    if (!input_stream.IsOk())
    {
        wxMessageBox("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

	wxString perspective = "";

	wxTextInputStream in(input_stream);
	perspective = in.ReadLine();

	m_mgr.LoadPerspective(perspective);
	setSubScreensOnToolBar();

	m_mgr.Update();

	wxRegEx extractDimensions("_[0-9]+_[0-9]+");

	wxString match;

	if(extractDimensions.Matches(openFileDialog.GetFilename()))
	{
		match = extractDimensions.GetMatch(openFileDialog.GetFilename());

		wxString extractEachDimension = match.substr(1,match.Length());

		customLayoutNativeScreenWidth = atoi(extractEachDimension.substr(0,extractEachDimension.Find('_')));
		customLayoutNativeScreenHeight = atoi(extractEachDimension.substr(extractEachDimension.Find('_') + 1,extractEachDimension.Find('_') + 1 - extractEachDimension.Length()));	
	
	}
	else
	{
		customLayoutNativeScreenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		customLayoutNativeScreenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	}

	if(customLayoutNativeScreenWidth!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
		customLayoutNativeScreenHeight!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
	{
		updateCustomLayoutForCurrentScreenSize();
	}
	
}


void MyFrame::OnDropDownToolbarItem_eventsList(wxAuiToolBarEvent& evt)
{
    if (evt.IsDropDownClicked())
    {
        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());

        tb->ToggleTool(ID_eventsList, true);

        // line up our menu with the button
        wxRect rect = tb->GetToolRect(evt.GetId());
        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
        pt = ScreenToClient(pt);

        PopupMenu(menuPopup_EventsList, pt);

		if(!menuPopup_EventsList->IsChecked(ID_eventsList_ABC) && !menuPopup_EventsList->IsChecked(ID_eventsList_Med) && 
			!menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
		{
			//wxMessageBox("here");
			// make sure the button is "un-stuck"
			tb->ToggleTool(ID_eventsList, false);
		}

		m_mgr.Update();

    }
}

/*FileMenuEvents*/
void MyFrame::onStartScenario(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is start Scenario"));
}

void MyFrame::onMonitorSound(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is monitor sound"));
}

void MyFrame::onImportVocalSounds(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is import vocal sounds"));
}

void MyFrame::onImportMediaFiles(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is import media files"));
}

void MyFrame::onAddMediaFiles(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is add media files"));
}

void MyFrame::onExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

/*ViewMenuEvents*/
void MyFrame::onPatientInstructorMonitor(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is patient/instructor monitor"));
}

/*SimulationMenuEvents*/
void MyFrame::onStartSimulation(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is start simulation"));
}

void MyFrame::onPauseSimulation(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is pause simulation"));
}

void MyFrame::onFastForward(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is fast forward"));
}

void MyFrame::onHaltScenario(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is halt scenario"));
}

void MyFrame::onDefaultScenario(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is default scenario"));
}

/*EditMenuEvents*/
void MyFrame::onMonitorSetup(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is monitor setup"));
}
void MyFrame::onVitalSigns(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is vital signs"));
}

/*HelpMenuEvents*/
void MyFrame::onHelp(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is help"));
}
void MyFrame::onAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(("This is wxWidgets hello world sample"),
		("About Hello World"), wxOK | wxICON_INFORMATION,this);
}
wxAuiDockArt* MyFrame::GetDockArt()
{
    return m_mgr.GetArtProvider();
}

//void MyFrame::DoUpdate()
//{
//    m_mgr.Update();
//}
//
void MyFrame::OnEraseBackground(wxEraseEvent& event)
{
    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

void MyFrame::OnPaneClose(wxAuiManagerEvent& evt)
{
	if(evt.pane->name == "mannequin")
	{
		subScreensBar->ToggleTool(ID_mannequin,false);
	}
	else if(evt.pane->name == "instructorPatientMonitor")
	{
		subScreensBar->ToggleTool(ID_instructorPatientMonitor,false);
	}
	else if(evt.pane->name == "eventsLog")
	{
		subScreensBar->ToggleTool(ID_eventsLog,false);
	}
	else if(evt.pane->name == "eventsList_ABC")
	{
		menuPopup_EventsList->Check(ID_eventsList_ABC,false);

	}
	else if(evt.pane->name == "eventsList_Med")
	{
		menuPopup_EventsList->Check(ID_eventsList_Med,false);
	}
	else if(evt.pane->name == "eventsList_Misc")
	{
		menuPopup_EventsList->Check(ID_eventsList_Misc,false);
	}
	else if(evt.pane->name == "scenarioContorls")
	{
		subScreensBar->ToggleTool(ID_scenarioControls,false);
	}

	if(!menuPopup_EventsList->IsChecked(ID_eventsList_ABC) && 
		!menuPopup_EventsList->IsChecked(ID_eventsList_Med) &&
		!menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
	{
			subScreensBar->ToggleTool(ID_eventsList,false);
	}

	if(evt.GetPane()->IsMaximized())
	{
		m_mgr.LoadPerspective(currentPerspective,true);

		//on maximization of these panes the corresponding menu for eventslist is disabled
		//on minimzation the menu options are enabled
		if(evt.GetPane()->name == "eventsList_ABC" || evt.GetPane()->name == "eventsList_Misc" || evt.GetPane()->name == "eventsList_Med")
		{
			this->menuPopup_EventsList->Enable(ID_eventsList_ABC,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_Med,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_Misc,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_ShowAll,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_HideAll,true);
		}
	}

}

void MyFrame::onPaneMaximize(wxAuiManagerEvent& evt)
{
	currentPerspective = m_mgr.SavePerspective();

	evt.GetPane()->Floatable(false);//prevents the pane from becoming floatable when maximized due to 
									//the window becoming unmovable if the pane(eventslists) is unchecked
									//from the events list

	if(evt.GetPane()->name == "eventsList_ABC" || evt.GetPane()->name == "eventsList_Misc" || evt.GetPane()->name == "eventsList_Med")
	{
		this->menuPopup_EventsList->Enable(ID_eventsList_ABC,false);
		this->menuPopup_EventsList->Enable(ID_eventsList_Med,false);
		this->menuPopup_EventsList->Enable(ID_eventsList_Misc,false);
		this->menuPopup_EventsList->Enable(ID_eventsList_ShowAll,false);
		this->menuPopup_EventsList->Enable(ID_eventsList_HideAll,false);
	}

	m_mgr.Update();
}

void MyFrame::onRestorePane(wxAuiManagerEvent& evt)
{
	if(evt.GetPane()->IsMaximized())
	{
		m_mgr.LoadPerspective(currentPerspective,true);

		//on maximization of these panes the corresponding menu for eventslist is disabled
		//on minimzation the menu options are enabled
		if(evt.GetPane()->name == "eventsList_ABC" || evt.GetPane()->name == "eventsList_Misc" || evt.GetPane()->name == "eventsList_Med")
		{
			this->menuPopup_EventsList->Enable(ID_eventsList_ABC,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_Med,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_Misc,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_ShowAll,true);
			this->menuPopup_EventsList->Enable(ID_eventsList_HideAll,true);
		}
	}
}

