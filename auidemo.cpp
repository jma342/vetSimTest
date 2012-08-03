#include "auidemo.h"
#include "wx\wfstream.h"

const int DEFAULT_SCREEN_MODE = 0;
const int MANUAL_SCREEN_MODE = 1;
const int PREPROGRAMMED_SCREEN_MODE =2;

//toggle this value to enforce the saving/loading of custom layouts with screen settings
const bool SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS = true;

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)

//intialises the frame and displays it
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    wxFrame* frame = new MyFrame(NULL,
                                 wxID_ANY,
                                 wxT("Vet Simulator"),
                                 wxDefaultPosition,
								 wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
    frame->Show();

    return true;
}

//captures the events to be emitted by the frame and binds the respective methods to their ids
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    
	/*FileMenuEvents*/
	EVT_MENU(ID_StartScenario, MyFrame::onStartScenario)
	EVT_MENU(ID_MonitorSound, MyFrame::onMonitorSound)
	EVT_MENU(ID_ImportVocalSounds, MyFrame::onImportVocalSounds)
	EVT_MENU(ID_ImportMediaFiles, MyFrame::onImportMediaFiles)
	EVT_MENU(ID_AddMediaFiles, MyFrame::onAddMediaFiles)
	EVT_MENU(ID_saveLayout, MyFrame::onSaveCustomLayout)
	EVT_MENU(ID_loadLayout, MyFrame::onLoadCustomLayout)
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

	/*CustomizeScreenLayout*/
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
	EVT_MENU(ID_adminOverwritePresetLayouts, MyFrame::onOverwritePresetLayout)

	/*Pane events*/
	EVT_AUI_PANE_CLOSE(MyFrame::OnPaneClose)
	EVT_AUI_PANE_MAXIMIZE(MyFrame::onPaneMaximize)
	EVT_AUI_PANE_RESTORE(MyFrame::onRestorePane)
END_EVENT_TABLE()

//jma342--sets up the frame by building the menus, toolbars and the panes
MyFrame::MyFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style)
{
    // tell wxAuiManager to manage this frame
    mainWindow.SetManagedWindow(this);

    // set frame icon
    SetIcon(wxIcon(sample_xpm));

    // create menu
	menuBar =  new wxMenuBar;

	/*FileMenuEvents--adds each of the necessary options to the file menu*/
	menuFile = new wxMenu;
	menuFile->Append(ID_StartScenario, ("Start Scenario"), "Starts an existing scenario");
	menuFile->Append(ID_ImportVocalSounds, ("Import Vocal Sounds"), "Import of customized .wav files containing vocal sounds");
	menuFile->Append(ID_ImportMediaFiles, ("Import Media Files"), "For the importing of media files");
	menuFile->Append(ID_AddMediaFiles, ("Add Media Files"), "Add media files to a simulation");
	menuFile->Append(ID_saveLayout,("Save Layout"), "Saves Current Screen Layout");
	menuFile->Append(ID_loadLayout,("Load Layout"), "Loads Saved Layout");
	menuFile->Append(ID_Exit, ("E&xit\tAlt-X"),"Exit the application");

	/*SimulationMenuEvents--adds each of the necessary options to the simulation menu*/
	menuSimulation = new wxMenu;
	menuSimulation->Append(ID_StartSimulation, ("Start Simulation"), "Start the select simulation scenario");
	menuSimulation->Append(ID_PauseSimulation, ("Pause Simulation"), "Pause the current simulation scenario");
	menuSimulation->Append(ID_FastForward, ("Fast Forward Simulation "), "Fast Forward the current simulation scenario");
	menuSimulation->Append(ID_HaltScenario, ("Halt Simulation"), "Halt the current simulation scenario");
	menuSimulation->Append(ID_DefaultScenario, ("Default Simulation"), "Select the default simulation scenario");
	menuSimulation->Append(ID_MonitorSound, ("Monitor Sound"), "Enables or disables simulation sounds");

	/*EditMenuEvents--adds each of the necessary options to the edit menu*/
	menuEdit = new wxMenu;
	menuEdit->Append(ID_MonitorSetup, ("Patient/Monitor Setup"), "Configure Patient Monitor Setup");
	menuEdit->Append(ID_VitalSigns, ("Vital Setups"), "Change vital signs of mannequin");

	/*HelpMenuEvents--adds each of the necessary options to the help menu*/
	menuHelp = new wxMenu;
	menuHelp->Append(ID_About, ("&About...\tF1"),"About dialog");
	menuHelp->Append(ID_Help, ("&Help...\tF2"),"User Manual/Help");

	/*ScreenLayoutEvents--adds each of the necessary options to the screen menu*/
	menuScreenLayout = new wxMenu;
	menuScreenLayout->Append(ID_DefaultMode, ("Default Mode"),"Select the default screen layout");
	menuScreenLayout->Append(ID_ManualMode, ("Manual Mode"),"Select screen Layout for Manual Mode");
	menuScreenLayout->Append(ID_PreProgrammedMode, ("PreProgrammed Mode"),"Select screen Layout for PreProgrammed Mode");

    menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuSimulation, _("&Simulation"));
	menuBar->Append(menuEdit, _("&Edit"));
	menuBar->Append(menuScreenLayout,_("Preset Layouts"));
	menuBar->Append(menuHelp, _("&Help"));
     
    SetMenuBar( menuBar);

    CreateStatusBar();
    GetStatusBar()->SetStatusText(_("Ready"));

	//sets the best size of the screen to the current screen size
	SetBestSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

	//sets the max size of the screen to the current screen size
	SetMaxSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

	this->Maximize();

	//loads all of the preset layouts stored on file
	MyFrame::loadAllPresetLayouts();

	//builds the initial screen layout which will remain if the saved default screen layout isn't found
	InitialScreenLayout();//the screen needs to be setup before it can be loaded with a new perspective

	//loads the saved default screen layout if one is found otherwise the above initial screen layout is used
	if(presetLayouts[DEFAULT_SCREEN_MODE].length() > 0)
	{
		mainWindow.LoadPerspective(presetLayouts[DEFAULT_SCREEN_MODE],true);//loads the saved default layout screen

		//if the screen width and screen height for the default screen are null the current screen settings are affiliated with it
		if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL || presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] == NULL)
		{
			presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
			presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
		}
		//if the screen settings for the saved default mode exist and don't match the current screen setup
		//the layout is updated to fit the current screen size if its dimensions weren't built for this screen
		else if(presetLayoutsNativeScreenWidth[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
			presetLayoutsNativeScreenHeight[DEFAULT_SCREEN_MODE]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
		{
			updatePresetLayoutForCurrentScreenSize(DEFAULT_SCREEN_MODE);
		}
	}

	//initialises the sub screens tool bar to inidicate which screens are active
	setSubScreensOnToolBar();

}

void MyFrame::InitialScreenLayout()
{
	/*eventsList and eventsLog*/
	eventsList_ABC = new EventsList(this,"ABC");
	eventsList_Misc = new EventsList(this,"Miscellaneous");
	eventsList_Med = new EventsList(this,"Medication");

	eventsLog = new EventsLog(this);
	
	mainWindow.AddPane(eventsLog, wxAuiPaneInfo().
		Name(wxT("eventsLog")).Caption(wxT("Events Log")).CloseButton().MaximizeButton().MinimizeButton().PinButton().Left().Position(0).BestSize(wxSize( 450, 206)));

	/*scenario Controls*/
	scenarioControls = new ScenarioControls(this);
	mainWindow.AddPane(scenarioControls, wxAuiPaneInfo().
		Name(wxT("scenarioControls")).Caption(wxT("Scenario Controls")).CloseButton().MaximizeButton().MinimizeButton().PinButton()
		.Left().Position(1).BestSize(wxSize( 185, 161)));

	/*scenario Controls*/

	mainWindow.AddPane(eventsList_ABC, wxAuiPaneInfo().
		Name(wxT("eventsList_ABC")).Caption(wxT("Events List")).CloseButton().MaximizeButton().Center().MinimizeButton().PinButton().
		Row(0).BestSize(wxSize( 293, 206)));

	mainWindow.AddPane(eventsList_Misc, wxAuiPaneInfo().
		Name(wxT("eventsList_Misc")).Caption(wxT("Events List")).CloseButton().MaximizeButton().Center().MinimizeButton().PinButton().Row(1)
		.BestSize(wxSize( 293, 206)));


	mainWindow.AddPane(eventsList_Med, wxAuiPaneInfo().
		Name(wxT("eventsList_Med")).Caption(wxT("Events List")).CloseButton().MaximizeButton().MinimizeButton().Center().PinButton().Row(2)
		.BestSize(wxSize( 293, 206)));

	/*eventsList and eventsLog*/

	/*manneqin and instructor/patient control*/
	mannequin = new MyScrolledWindowSmart(this);
	instructorPatientMonitor = new MyScrolledWindowSmart(this);

	mainWindow.AddPane(mannequin, wxAuiPaneInfo().
		Name(wxT("mannequin")).Caption(wxT("Mannequin Avatar")).CloseButton().MaximizeButton().MinimizeButton().PinButton().
		BestSize(wxSize( 450, 206)).Top().Position(0));

	mainWindow.AddPane(instructorPatientMonitor, wxAuiPaneInfo().
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

    mainWindow.AddPane(subScreensBar, wxAuiPaneInfo().
                  Name(wxT("SubScreensBar")).Caption(wxT("SubScreens Toolbar")).
                  ToolbarPane().Top());

	mainWindow.AddPane(adminOverWritePresetLayoutBar, wxAuiPaneInfo().
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

	 mainWindow.Update();
}

//jma342--updates a preset layout to fit the current screen size
void MyFrame::updatePresetLayoutForCurrentScreenSize(int chosenScreenLayout)
{
	//captures an array of all of the panes on screen
	wxAuiPaneInfoArray *allPanes = &(mainWindow.GetAllPanes());
	int adjustedWidth = 0;
	int adjustedHeight = 0;

	//loops through each pane and adjusts them to fit the current screen size
	for(int pane = 0;pane < allPanes->size();pane++)
	{
		//updates all panes except the two toolbars
		if((*allPanes)[pane].name != "SubScreensBar" && (*allPanes)[pane].name != "OverwritePresetLayoutBar")
		{
			adjustedWidth = ceil((*allPanes)[pane].best_size.GetWidth() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/(double)presetLayoutsNativeScreenWidth[chosenScreenLayout]));
			adjustedHeight = ceil((*allPanes)[pane].best_size.GetHeight() *((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/(double)presetLayoutsNativeScreenHeight[chosenScreenLayout]));

			//attempting to resize each pane based on the current screen size and the
			//screen size each pane was initially built for...on searching the internet it appears
			//that the only way for this to work is to fix(prevents a pane from being moved) each pane update the screen and then
			//make each pane resizable again followed by another screen update...this seems convoluted to 
			//me...I believe there is a better way I just haven't been able to find one
			(*allPanes)[pane].BestSize(adjustedWidth,adjustedHeight);
			(*allPanes)[pane].Fixed();

			mainWindow.Update();

			(*allPanes)[pane].Resizable();

			mainWindow.Update();
		}	
	}

	//stores the current screen settings for the chosen preset layout
	presetLayoutsNativeScreenWidth[chosenScreenLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	presetLayoutsNativeScreenHeight[chosenScreenLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	//stores the current screen layout for the chosen preset layout
	presetLayouts[chosenScreenLayout] = mainWindow.SavePerspective();

}

//jma342--uupdates a custom layout to fit the current screen size
void MyFrame::updateCustomLayoutForCurrentScreenSize()
{
	//captures an array of all of the panes on screen
	wxAuiPaneInfoArray *allPanes = &(mainWindow.GetAllPanes());
	int adjustedWidth = 0;
	int adjustedHeight = 0;

	//loops through each pane and adjusts them to fit the current screen size
	for(int pane = 0;pane < allPanes->size();pane++)
	{
		//updates all panes except the two toolbars
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

			mainWindow.Update();

			(*allPanes)[pane].Resizable();

			mainWindow.Update();
		}	
	}

	//stores the current screen settings for the chosen custom layout
	customLayoutNativeScreenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	customLayoutNativeScreenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

}


//jma342--destroys the manager for the panes
MyFrame::~MyFrame()
{
    mainWindow.UnInit();
}

//jma342--updates the chosen preset layout to best fit the current screen size
void MyFrame::updateChosenPresetLayout(int mode)
{
	//loads the default layout
	mainWindow.LoadPerspective(presetLayouts[mode]);

	//updates the screens toolbar
	setSubScreensOnToolBar();

	//updates the screen to reflect the loaded layout
	mainWindow.Update();

	//if the screen width and screen height are null the current screen settings are affiliated with it
	if(presetLayoutsNativeScreenWidth[mode] == NULL || presetLayoutsNativeScreenWidth[mode] == NULL)
	{
		presetLayoutsNativeScreenWidth[mode] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		presetLayoutsNativeScreenHeight[mode] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	}

	//updates the layout to fit the current screen size if its dimensions weren't built for this screen
	else if(presetLayoutsNativeScreenWidth[mode]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || 
		presetLayoutsNativeScreenHeight[mode]!= wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
	{
		updatePresetLayoutForCurrentScreenSize(mode);
	}
}

//jma342--this function is triggered when the default mode option is chosen.
void MyFrame::onDefaultMode(wxCommandEvent& WXUNUSED(event))
{
	updateChosenPresetLayout(DEFAULT_SCREEN_MODE);
}

//jma342--this function is triggered when the manual mode option is chosen.
void MyFrame::onManualMode(wxCommandEvent& WXUNUSED(event))
{
	updateChosenPresetLayout(MANUAL_SCREEN_MODE);
}

//jma342--this function is triggered when the preProgrammed mode option is chosen.
void MyFrame::onPreProgrammedMode(wxCommandEvent& WXUNUSED(event))
{
	updateChosenPresetLayout(PREPROGRAMMED_SCREEN_MODE);

}

//jma342--sets the subscreens toolbar to indicate which panes are active
void MyFrame::setSubScreensOnToolBar()
{
	//mannequin
	if(this->mannequin->IsShown())
	{
		subScreensBar->ToggleTool(ID_mannequin,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_mannequin,false);
	}

	//instructor patient monitor
	if(this->instructorPatientMonitor->IsShown())
	{
		subScreensBar->ToggleTool(ID_instructorPatientMonitor,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_instructorPatientMonitor,false);
	}

	//eventslog
	if(this->eventsLog->IsShown())
	{
		subScreensBar->ToggleTool(ID_eventsLog,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_eventsLog,false);
	}

	//scenario controls
	if(this->scenarioControls->IsShown())
	{
		subScreensBar->ToggleTool(ID_scenarioControls,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_scenarioControls,false);
	}

	//eventslist_ABC
	if(this->eventsList_ABC->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_ABC,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_ABC,false);
	}

	//eventslist_Med
	if(this->eventsList_Med->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_Med,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_Med,false);
	}

	//eventslist_Misc
	if(this->eventsList_Misc->IsShown())
	{
		menuPopup_EventsList->Check(ID_eventsList_Misc,true);
	}
	else
	{
		menuPopup_EventsList->Check(ID_eventsList_Misc,false);
	}

	//toggles the main button for all of the events lists
	if(this->eventsList_ABC->IsShown() || this->eventsList_Med->IsShown() || this->eventsList_Misc->IsShown())
	{
		subScreensBar->ToggleTool(ID_eventsList,true);
	}
	else
	{
		subScreensBar->ToggleTool(ID_eventsList,false);
	}
	
}

//jma342--this is triggered when the mannequin toolbar button is executed
void MyFrame::onMannequin(wxCommandEvent& WXUNUSED(event))
{
	toggleMannequin();
	mainWindow.Update();
}

//jma342--displays or hides the mannequin window/pane depending on the toggle position of the 
//mannequin toolbar button
void MyFrame::toggleMannequin()
{
	//displays mannequin pane if the mannequin toolbar button is toggled on
	if(subScreensBar->GetToolToggled(ID_mannequin))
	{
		mainWindow.GetPane("mannequin").Show();
	}
	else
	{
		mainWindow.GetPane("mannequin").Hide();
	}

}

//jma342--this is triggered when the instructorPatient monitor button is executed
void MyFrame::onInstructorPatientMonitor(wxCommandEvent& WXUNUSED(event))
{
	toggleInstructorPatientMonitor();
	mainWindow.Update();
}

//jma342--displays or hides the instructor/patient monitor window/pane depending on the toggle position of the 
//instructor/patient monitor toolbar button
void MyFrame::toggleInstructorPatientMonitor()
{
	if(subScreensBar->GetToolToggled(ID_instructorPatientMonitor))
	{
		
		mainWindow.GetPane("instructorPatientMonitor").Show();
				
	}
	else
	{
		mainWindow.GetPane("instructorPatientMonitor").Hide();
	}

}

//jma342--this is triggered when the events log button is executed
void MyFrame::onEventsLog(wxCommandEvent& WXUNUSED(event))
{
	toggleEventsLog();
	mainWindow.Update();
}

//jma342--displays or hides the events log window/pane depending on the toggle position of the 
//events log toolbar button
void MyFrame::toggleEventsLog()
{
	if(subScreensBar->GetToolToggled(ID_eventsLog))
	{
		mainWindow.GetPane("eventsLog").Show();			
	}
	else
	{
		mainWindow.GetPane("eventsLog").Hide();
	}

}

//jma342--this is triggered when the eventsList_ABC button is executed
void MyFrame::onEventsList_ABC(wxCommandEvent& WXUNUSED(event))
{
	//eventsList_ABC button is checked the eventsList ABC pane is displayed
	//otherwise it is hidden
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_ABC))
	{
		mainWindow.GetPane("eventsList_ABC").Show();	
	}
	else
	{
		mainWindow.GetPane("eventsList_ABC").Hide();
	}

}

//jma342--this is triggered when the eventsList_Med button is executed
void MyFrame::onEventsList_Med(wxCommandEvent& WXUNUSED(event))
{
	//eventsList_Med button is checked the eventsList_Med pane is displayed
	//otherwise it is hidden
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Med))
	{
		mainWindow.GetPane("eventsList_Med").Show();	
	}
	else
	{
		mainWindow.GetPane("eventsList_Med").Hide();
	}
}

//jma342--this is triggered when the eventsList_Misc button is executed
void MyFrame::onEventsList_Misc(wxCommandEvent& WXUNUSED(event))
{
	//eventsList_Misc button is checked the eventsList_Misc pane is displayed
	//otherwise it is hidden
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
	{
		mainWindow.GetPane("eventsList_Misc").Show();	
	}
	else
	{
		mainWindow.GetPane("eventsList_Misc").Hide();
	}
}

//jma342--this is triggered when the eventsList ShowAll button is executed
void MyFrame::onEventsList_ShowAll(wxCommandEvent& WXUNUSED(event))
{
	//toggles on the main button for each of the events lists
	subScreensBar->ToggleTool(ID_eventsList,true);

	//displays all of the eventst lists on screen
	mainWindow.GetPane("eventsList_ABC").Show();			
	mainWindow.GetPane("eventsList_Med").Show();
	mainWindow.GetPane("eventsList_Misc").Show();

	//sets each of the events lists menu options to indicate on screen
	menuPopup_EventsList->Check(ID_eventsList_ABC,true);
	menuPopup_EventsList->Check(ID_eventsList_Med,true);
	menuPopup_EventsList->Check(ID_eventsList_Misc,true);

}

//jma342--this is triggered when the eventsList HideAll button is executed
void MyFrame::onEventsList_HideAll(wxCommandEvent& WXUNUSED(event))
{
	//toggles off the main button for each of the events lists
	subScreensBar->ToggleTool(ID_eventsList,false);

	//removes all of the eventst list from the screen
	mainWindow.GetPane("eventsList_ABC").Hide();			
	mainWindow.GetPane("eventsList_Med").Hide();
	mainWindow.GetPane("eventsList_Misc").Hide();

	//sets each of the events lists menu options to off screen
	menuPopup_EventsList->Check(ID_eventsList_ABC,false);
	menuPopup_EventsList->Check(ID_eventsList_Med,false);
	menuPopup_EventsList->Check(ID_eventsList_Misc,false);

}

//jma342--this is triggered when the expand all events list menu option is executed
//this expands all of the folders in each of the eventst lists
void MyFrame::onExpandAllEventsLists(wxCommandEvent& WXUNUSED(event))
{
	//if the expand all events lists menu option is chosen all of the folders
	//in each of the events lists are expanded
	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Expand_All_Folders))
	{

		((EventsList*)eventsList_ABC)->expandAllFolders();
		((EventsList*)eventsList_Misc)->expandAllFolders();
		((EventsList*)eventsList_Med)->expandAllFolders();
	}

	//if the expand all events lists menu option is not chosen all of the folders
	//in each of the events lists are collapsed
	else
	{
		((EventsList*)eventsList_ABC)->collapseAllFolders();
		((EventsList*)eventsList_Misc)->collapseAllFolders();
		((EventsList*)eventsList_Med)->collapseAllFolders();
	}
}

////jma342--this is triggered when the expand all events list menu option is executed
////this expands all of the folders in each of the eventst lists
//void MyFrame::toggleEventsList()
//{
//	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
//	{
//		mainWindow.GetPane("eventsList_Misc").Show();	
//	}
//	else
//	{
//		mainWindow.GetPane("eventsList_Misc").Hide();
//	}
//
//	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Med))
//	{
//		mainWindow.GetPane("eventsList_Med").Show();	
//	}
//	else
//	{
//		mainWindow.GetPane("eventsList_Med").Hide();
//	}
//
//	if(this->menuPopup_EventsList->IsChecked(ID_eventsList_Misc))
//	{
//		mainWindow.GetPane("eventsList_ABC").Show();	
//	}
//	else
//	{
//		mainWindow.GetPane("eventsList_ABC").Hide();
//	}
//}

//jma342--this is triggered when the scenario controls button is executed
void MyFrame::onScenarioControls(wxCommandEvent& WXUNUSED(event))
{
	toggleScenarioControls();
	mainWindow.Update();
}

//jma342--displays or hides the scenario controls window/pane depending on the toggle position of the 
//scenarion controls toolbar button
void MyFrame::toggleScenarioControls()
{
	if(subScreensBar->GetToolToggled(ID_scenarioControls))
	{
		mainWindow.GetPane("scenarioControls").Show();			
	}

	else
	{
		mainWindow.GetPane("scenarioControls").Hide();			
	}

}

//jma342--this is triggered when the overwrite presetlayout toolbar button is executed
void MyFrame::onOverwritePresetLayout(wxCommandEvent& WXUNUSED(event))
{
	//caputres the string representation of the layout option chosen
	wxString chosenPresetLayout = this->layoutChoice->GetString(this->layoutChoice->GetSelection());

	//executes only if an option is chosen
	if(chosenPresetLayout.Length() > 0)
	{
		 wxMessageDialog dialog(this,
			 " Do you want to overwrite " + chosenPresetLayout + "?",
							   "Overwrite Preset Layout",
							   wxCENTER |
							   wxYES_NO | wxICON_QUESTION);


		 //displays the above message dialog in modal state
		switch ( dialog.ShowModal() )
		{
			case wxID_YES:
				overWritePresetLayout(this->layoutChoice->GetSelection());
				break;

			case wxID_NO:
				wxMessageBox("Overwrite of " + chosenPresetLayout + "cancelled.");
				break;

			default:
				wxMessageBox("Unexpected option!");
		}
	
	}

	//toggles the overwrite preset layout button off after the operation is complete
	this->adminOverWritePresetLayoutBar->ToggleTool(ID_adminOverwritePresetLayouts,false);
	mainWindow.Update();
	
}

//jma342--this is triggered when the overwrite presetlayout toolbar button is executed
void MyFrame::overWritePresetLayout(int chosenPresetLayout)
{        
	wxString filePathLayout;
	wxString filePathNativeScreen;

	//creates preset layouts folder if it doesn't exist
	if(!wxDir::Exists("PresetLayouts"))
	{
		wxDir::Make("PresetLayouts");
	}

	int layoutID = -1;

	//sets the paths for the layouts and their corresponding screen settings
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
	out.WriteString(mainWindow.SavePerspective());
		
	outputLayout.Close();

	presetLayouts[chosenPresetLayout] = mainWindow.SavePerspective();

	//write the native screen settings for the layout to file
	wxFileOutputStream outputNativeScreen("PresetLayouts\\"+filePathNativeScreen);

		
	wxTextOutputStream outNativeScreen(outputNativeScreen);
	outNativeScreen.WriteDouble((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_X));
	outNativeScreen.WriteString("\n");
	outNativeScreen.WriteDouble((double)wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));
		
	outputNativeScreen.Close();

	//store the current screen settings for the chosen preset layout
	presetLayoutsNativeScreenWidth[chosenPresetLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	presetLayoutsNativeScreenHeight[chosenPresetLayout] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

	//if the menu option for the current layout was disabled it is enables
	//this option would have been disabled if on start up of the application
	//no layout was loaded for it due to the requisite file not being found
	if(!this->menuBar->IsEnabled(layoutID))
	{	
		this->menuBar->Enable(layoutID,true);
	}
}

//jma342--this is triggered when the application is started. It loads all of the preset layouts that are stored to file
void MyFrame::loadAllPresetLayouts()
{
	wxString presetLayoutFiles[] = {"defaultLayout.txt","manualMode.txt","preProgrammedMode.txt"};
	wxString presetLayoutNativeScreenFiles[] = {"defaultLayoutNativeScreen.txt","manualModeNativeScreen.txt","preProgrammedModeNativeScreen.txt"};
	wxString failedLayouts = "";
	int layoutsFailed = 1;

	//loops through each of the preset layout files
	for(int count = 0; count < 3;count++)
	{
		//builds the file paths for each of the preset layouts
		wxString layoutFilePath = "PresetLayouts\\"+presetLayoutFiles[count];
		wxString nativeScreenfilePath = "PresetLayouts\\"+presetLayoutNativeScreenFiles[count];

		wxString layoutsFailed_str;

		layoutsFailed_str.Printf("%d",layoutsFailed);

		//executes if the build layout file path doesn't exist
		if (!wxFile::Exists(layoutFilePath))
		{
			//if any of the layout file paths built for a layout mode
			//don't exist their corresponding menu option is disabled

			//default mode
			if(count == 0)
			{
				this->menuScreenLayout->Enable(ID_DefaultMode,false);
				failedLayouts = "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}

			//manual mode
			else if (count == 1)
			{
				this->menuScreenLayout->Enable(ID_ManualMode,false);
				failedLayouts += "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}

			//preprogrammede mode
			else if(count == 2)
			{
				this->menuScreenLayout->Enable(ID_PreProgrammedMode,false);
				failedLayouts += "\n " + layoutsFailed_str + ". " + presetLayoutFiles[count];
			}

			layoutsFailed++;

		}

		else
		{
			//intialises the file input stream
			wxFileInputStream layoutInputStream(layoutFilePath);
			wxTextInputStream inLayout(layoutInputStream);

			//reads in the layout from the current file paht
			presetLayouts[count] = inLayout.ReadLine();

			
			//if there are no native screen metrics file the metrics
			//for the current screen are used instead
			if(!wxFile::Exists(nativeScreenfilePath))
			{
				presetLayoutsNativeScreenWidth[count] = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
				presetLayoutsNativeScreenHeight[count] = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
			}
			else
			{
				wxFileInputStream nativeScreenInputStream(nativeScreenfilePath);
				wxTextInputStream inLayoutNativeScreen(nativeScreenInputStream);

				//reads the metrics from the metric file for the current layout
				presetLayoutsNativeScreenWidth[count] = (int)inLayoutNativeScreen.ReadDouble();
				presetLayoutsNativeScreenHeight[count] = (int)inLayoutNativeScreen.ReadDouble();
			}
		}
	}

	//if any of the layout files failed to load they would be displayed in the following
	//message box
	if(failedLayouts.length() > 0)
	{
		wxMessageBox("The following preset layouts failed to load: \n " + failedLayouts + 
			"\n\nPlease place the necessary files in the PresetLayouts folder at this location:\n\n" + wxGetCwd() + 
			" \n\nOR\n\nCreate the missing layouts by using the Overwrite Preset Layouts toolbar");
	}
}

//jma342--this function is triggered when the save custom layout is executed
void MyFrame::onSaveCustomLayout(wxCommandEvent& WXUNUSED(event))
{
	//executes if the applicatin is set to save custom layouts with screen 
	//settings..this is set at the beggining of this .cpp file 
	if(SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS)
	{
		saveCustomLayoutWithScreenSettings();
	}
	else
	{
		saveCustomLayoutWithOUTScreenSettings();
	}	
}

//jma342--this function saves custom layouts with no regard for the
//screen settings under which the custom layout is saved
void MyFrame::saveCustomLayoutWithOUTScreenSettings()
{
		wxFileDialog 
            saveFileDialog(this, _("Save Screen Layout"), "", "",
                           "Screen Layout files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	//executes if the user chooses to cancel file dialog
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
        
	
    //intialise the output streem
	wxFileOutputStream output_stream(saveFileDialog.GetPath());

	//if the output stream isn't successfully initialised
    if (!output_stream.IsOk())
    {
        wxMessageBox("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }

	//wraps the output stream in a text output stream
	wxTextOutputStream out(output_stream);

	//saves the current screen layout to file
	out.WriteString(mainWindow.SavePerspective());
		
	//closes the output stream
	output_stream.Close();

}

//jma342--this function saves custom layouts with regard for the
//screen settings under which the custom layout is saved
void MyFrame::saveCustomLayoutWithScreenSettings()
{
	wxFileDialog 
            saveFileDialog(this, _("Save Screen Layout"), "", "",
                           "Screen Layout files (*.txt)|*.txt", wxFD_SAVE);

	//executes if the user chooses to cancel file dialog
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
	//prevents users from entering underscores into filenames that are new
	//the system uses underscores to extract the screen settings under which the layout was created
	if(saveFileDialog.GetFilename().Find('_') >= 0 && !wxFile::Exists(saveFileDialog.GetPath()))
	{
		wxMessageBox("Please create filenames without underscores(_)...Save Operation Cancelled");
		return;
	}

	//creates and compiles regular expression to extract
	//the screen settings from the filename that the layout is being saved
	//to granted the filename already exists(is not new)
	wxRegEx extractDimensions("_[0-9]+_[0-9]+");

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

		//splits the file into the path, name and extension
		wxFileName::SplitPath(saveFileDialog.GetPath(),&path,&name,&ext);

		//inserts the screen settings into the insertResolution string
		insertResolution.Printf("_%d_%d",wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));

		//rebuilds the user chosen filename with the inserted screen settings
		fileName += path + "\\" + name + insertResolution + "." + ext;

	}

	//if the chosen file path consists of screen settings
	else if(extractDimensions.Matches(saveFileDialog.GetFilename()))
	{
		//retreives the string matching the regular expression
		match = extractDimensions.GetMatch(saveFileDialog.GetPath());

		//gets rid of first underscore to leave #_# which leaves 
		//each of the screen settings(width/height) on either side of the remaining underscore
		wxString extractEachDimension = match.substr(1,match.Length());

		//extracts the width of the screen setting which precedes the remaining underscore
		int width = atoi(extractEachDimension.substr(0,extractEachDimension.Find('_')));

		//extracts the height of the screen setting which succeeds the remaining underscore
		int height = atoi(extractEachDimension.substr(extractEachDimension.Find('_') + 1,extractEachDimension.Find('_') + 1 - extractEachDimension.Length()));	
		
		//executes if the screen settings of the chosen file don't match the current screen settings
		if(width != wxSystemSettings::GetMetric(wxSYS_SCREEN_X) || height != wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
		{
			wxString insertResolution;
			wxString path;
			wxString name;
			wxString ext;

			//splits the file into the path, name and extension
			wxFileName::SplitPath(saveFileDialog.GetPath(),&path,&name,&ext);

			//removes the screen settings from the current file name as they are being replaced 
			//with the current screen settings
			wxString newName = name.substr(0,name.Find('_'));

			//inserts the screen settings into the insertResolution string
			insertResolution.Printf("_%d_%d",wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y));

			//rebuilds the user chosen filename with the updated screen settings
			fileName += path + "\\" + newName + insertResolution + "." + ext;
		}

		//executes if the chosen file name consists of screen settings that match
		//the current screen settings
		else
		{
			fileName = saveFileDialog.GetPath();
		}
	}
	/*else
	{
		fileName = saveFileDialog.GetPath();
	}*/

	long choice = -1;

	//executes if the user chosen path being written to file already exists and
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

	//executes if the user path chosen exists but the system adjusted filename doesn't
	else if(wxFile::Exists(saveFileDialog.GetPath()) && !wxFile::Exists(fileName))
	{
		wxString path;
		wxString name;
		wxString ext;

		//simply used to extract the name for display in the following dialog
		wxFileName::SplitPath(fileName,&path,&name,&ext);

		//message dialog displays the options that are available to the user
		//based on the scenario
		wxMessageDialog dialog(this,
			 saveFileDialog.GetFilename() + " already exists.\n\n"
			"Option 1 - Overwrite " + saveFileDialog.GetFilename() +"\n\n" + 
			"Option 2 - Keep " + saveFileDialog.GetFilename() + " and new file(" + name + ")  being created with the current layout.",
						"Overwrite Custom Layout",
						wxCENTER |
						wxYES_NO | wxCANCEL | wxICON_QUESTION);

		//changes the labels for the Yes, No and Cancel buttons
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
		//will be created which is done further down in the code

		//option 3
		else if(choice == wxID_CANCEL)
		{
			wxMessageBox("Save operation cancelled.");
			return;
		}
	}

	//executes if the user chosen path being written to already exists but
	//it didn't consist of the current screen dimensions therefore a system adjusted filename
	//is created. However, this system adjusted filename also exists.
	else if(wxFile::Exists(saveFileDialog.GetPath()) && wxFile::Exists(fileName))
	{
		wxString path;
		wxString name;
		wxString ext;

		//simply used to extract the name for display in the following dialog
		wxFileName::SplitPath(fileName,&path,&name,&ext);
		
		//it is possible that the filename chosen to be overwritten and the
		//system built filename both exist...the user is given the options
		//of 
		//1. removing the chosen file and overwriting the system created file
		//2. keeping the chosen file and overwriting the file that conflicts with the system created file
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

    //initialise the output stream
	wxFileOutputStream output_stream(fileName);

	//executes if the output stream isn't initialised ok
    if (!output_stream.IsOk())
    {
        wxMessageBox("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }

	//wraps the output stream in a text output stream
	wxTextOutputStream out(output_stream);

	//saves the custom layout to file
	out.WriteString(mainWindow.SavePerspective());
		
	//closes the output stream
	output_stream.Close();
}

//jma342--this function loads custom layouts with no regard for the
//screen settings under which the custom layout was saved
void MyFrame::onLoadCustomLayout(wxCommandEvent& WXUNUSED(event))
{ 
	//executes if the applicatin is set to save custom layouts with screen 
	//settings..this is set at the beggining of this .cpp file. If the application
	//was set to save custom layouts with screen settings the application will
	//load custom layouts with the expectation of screen settings being in 
	//the filename
	if(SAVE_CUSTOM_LAYOUTS_WITH_SCREEN_SETTINGS)
	{
		loadCustomLayoutWithScreenSettings();
	}
	else
	{
		loadCustomLayoutWithOUTScreenSettings();
	}	
}

//jma342--this function is triggered when the load custom layout is executed
void MyFrame::loadCustomLayoutWithOUTScreenSettings()
{
	    wxFileDialog 
        openFileDialog(this, _("Load Screen Layout"), "", "",
                        "Screen Layouts (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	//executes if the user chooses to cancel file dialog
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
        
   //intialise the input stream
    wxFileInputStream input_stream(openFileDialog.GetPath());

	//executes if the input stream wasn't inialised ok
    if (!input_stream.IsOk())
    {
        wxMessageBox("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

	wxString perspective = "";

	//wraps input stream in text input stream
	wxTextInputStream in(input_stream);

	//reads layout from file
	perspective = in.ReadLine();

	//loads layout onto the screen
	mainWindow.LoadPerspective(perspective);

	//updates the subscreens toolbar to reflect
	//the panes on screen
	setSubScreensOnToolBar();

	//update the screen to display the loaded layout
	mainWindow.Update();
}

//jma342--this function loads custom layouts with regard for the
//screen settings under which the custom layout is saved
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

	mainWindow.LoadPerspective(perspective);
	setSubScreensOnToolBar();

	mainWindow.Update();

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

		mainWindow.Update();

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
		mainWindow.LoadPerspective(currentPerspective,true);

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
	currentPerspective = mainWindow.SavePerspective();

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

	mainWindow.Update();
}

void MyFrame::onRestorePane(wxAuiManagerEvent& evt)
{
	if(evt.GetPane()->IsMaximized())
	{
		mainWindow.LoadPerspective(currentPerspective,true);

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


