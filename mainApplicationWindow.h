// For compilers that support precompilation, includes "wx/wx.h".
#ifndef MAINAPPLICATIONWINDOW_H
#define MAINAPPLICATIONWINDOW_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "scrolledWindows.h"
#include "eventsList.h"
#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "wx/dir.h"
#include "wx/dc.h"
#include "wx/math.h"
#include "wx/regex.h"
#include "wx/filefn.h"

#include "wx/aui/aui.h"
#include "wxWidgets-2.9.3\samples\sample.xpm"
// -- application --

class MyApp : public wxApp
{
public:
    bool OnInit();
};


// -- frame --

class MyFrame : public wxFrame
{
    enum
    {
        /*FileMenuEvents*/
		ID_MonitorSound,
		ID_ImportVocalSounds,
		ID_ImportMediaFiles,
		ID_AddMediaFiles,
		ID_Exit,

		/*ScenarioMenuEvents*/
		ID_StartScenario,
		ID_PlayScenario,
		ID_PauseScenario,
		ID_NextState,
		ID_HaltScenario,
		ID_DefaultScenario,

		/*HelpMenuEvents*/
		ID_About,
		ID_Help,

		/*Screen Layout Menu Events*/
		ID_DefaultMode,
		ID_ManualMode,
		ID_PreProgrammedMode,

		/*Customize Default Screen Layout Menu Events*/
		ID_mannequin,
		ID_instructorPatientMonitor,
		ID_eventsLog,
		ID_scenarioControls,
		ID_eventsList,//events list tool bar id
		//ids for options in events list tool bar drop down
			ID_eventsList_ABC,
			ID_eventsList_Med,
			ID_eventsList_Misc,
			ID_eventsList_Expand_All_Folders,
			ID_eventsList_ShowAll,
			ID_eventsList_HideAll,
			
		ID_saveLayout,
		ID_loadLayout,
		ID_adminOverwritePresetLayouts,
		ID_adminOverwritePresetLayouts_choice
        
    };

public:
    MyFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MyFrame();

private:

	 wxAuiManager mainWindow;
	/*FileMenuEvents*/
	void onMonitorSound(wxCommandEvent& event);
	void onImportVocalSounds(wxCommandEvent& event);
	void onImportMediaFiles(wxCommandEvent& event);
	void onAddMediaFiles(wxCommandEvent& event);
	void onExit(wxCommandEvent& event);

	/*ViewMenuEvents*/
	void onPatientInstructorMonitor(wxCommandEvent& event);

	/*SimulationMenuEvents*/
	void onStartScenario(wxCommandEvent& event);
	void onPlayScenario(wxCommandEvent& event);
	void onPauseScenario(wxCommandEvent& event);
	void onNextState(wxCommandEvent& event);
	void onHaltScenario(wxCommandEvent& event);
	void onDefaultScenario(wxCommandEvent& event);

	/*EditMenuEvents*/
	void onMonitorSetup(wxCommandEvent& event);
	void onVitalSigns(wxCommandEvent& event);

	/*HelpMenuEvents*/
	void onAbout(wxCommandEvent& event);
	void onHelp(wxCommandEvent& event);

	/*ScreenLayoutEvents*/
	void onDefaultMode(wxCommandEvent& event);
	void onManualMode(wxCommandEvent& event);
	void onPreProgrammedMode(wxCommandEvent& event);

	/*CustomizeDefaultScreenLayoutEvents*/
	void onMannequin(wxCommandEvent& event);
	void onInstructorPatientMonitor(wxCommandEvent& event);
	void onEventsLog(wxCommandEvent& event);
	void onEventsList_ShowAll(wxCommandEvent& event);
	void onExpandAllEventsLists(wxCommandEvent& event);
	void onEventsList_HideAll(wxCommandEvent& event);
	void onEventsList_ABC(wxCommandEvent& event);
	void onEventsList_Med(wxCommandEvent& event);
	void onEventsList_Misc(wxCommandEvent& event);
	void onScenarioControls(wxCommandEvent& event);
	void OnDropDownToolbarItem_eventsList(wxAuiToolBarEvent& evt);

	void InitialScreenLayout();
	void setSubScreensOnToolBar();

	void onSaveCustomLayout(wxCommandEvent& event);
	void onLoadCustomLayout(wxCommandEvent& event);

	void toggleMannequin();
	void toggleInstructorPatientMonitor();
	void toggleEventsLog();
	void toggleScenarioControls();
	void toggleEventsList_ABC();
	void toggleEventsList_Med();
	void toggleEventsList_Misc();

	void OnPaneClose(wxAuiManagerEvent& evt);
	void onPaneMaximize(wxAuiManagerEvent& evt);
	void onRestorePane(wxAuiManagerEvent& evt);

	void onOverwritePresetLayout(wxCommandEvent& event);

	void overWritePresetLayout(int chosenPresetLayout);

	void loadAllPresetLayouts();

	void updatePresetLayoutForCurrentScreenSize(int chosenScreenLayout);
	void updateChosenPresetLayout(int mode);
	void updateCustomLayoutForCurrentScreenSize();
	void saveCustomLayoutWithScreenSettings();
	void saveCustomLayoutWithOUTScreenSettings();
	void loadCustomLayoutWithScreenSettings();
	void loadCustomLayoutWithOUTScreenSettings();

	MyScrolledWindowBase *mannequin,
							*instructorPatientMonitor, *eventsList_ABC,*eventsList_Misc,*eventsList_Med, *eventsLog,*scenarioControls;

	wxVector<MyScrolledWindowBase *> orphanWindows;

	wxSizerItem *mannequin_SizerItem,
							*instructorPatientMonitor_SizerItem, *eventsList_ABC_SizerItem,*eventsList_Misc_SizerItem,
							*eventsList_Med_SizerItem, *eventsLog_SizerItem,*scenarioControls_SizerItem;

	wxSizer *sizerScrollWin_eventsLogList, *sizerScrollWin_Mannequin_InstructorMonitor,*sizerScrollWin_scenControls;

	wxPanel *panel;
	wxSizer *topsizer;
	wxSize initialFrameSize;

	wxMenu *menuScreenLayout;
	wxMenu *menuHelp;
	wxMenu *menuEdit;
	wxMenu *menuScenario;
	wxMenu *menuView;
	wxMenu *menuFile;
	wxMenu *menuCustomiszeDefaultScreenLayout;
	wxMenuBar *menuBar;
	wxMenu *menuPopup_EventsList;

	wxAuiToolBar* subScreensBar;
	wxAuiToolBar* adminOverWritePresetLayoutBar;

	wxChoice* layoutChoice;

	wxString currentPerspective;
	wxString presetLayouts[3];
	int presetLayoutsNativeScreenWidth[3];
	int presetLayoutsNativeScreenHeight[3];

	int customLayoutNativeScreenWidth;
	int customLayoutNativeScreenHeight;
		

	DECLARE_EVENT_TABLE()
};

#endif