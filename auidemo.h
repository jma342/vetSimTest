// For compilers that support precompilation, includes "wx/wx.h".
#ifndef AUIDEMO_H
#define AUIDEMO_H

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


//class wxSizeReportCtrl;
// -- wxSizeReportCtrl --
// (a utility control that always reports it's client size)

class wxSizeReportCtrl : public wxControl
{
public:

    wxSizeReportCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     wxAuiManager* mgr = NULL)
                     : wxControl(parent, id, pos, size, wxNO_BORDER)
    {
        m_mgr = mgr;
    }

private:

    void OnPaint(wxPaintEvent& WXUNUSED(evt));

    void OnEraseBackground(wxEraseEvent& WXUNUSED(evt));

    void OnSize(wxSizeEvent& WXUNUSED(evt));

private:

    wxAuiManager* m_mgr;

    DECLARE_EVENT_TABLE()
};



// -- frame --

class MyFrame : public wxFrame
{
    enum
    {
        ID_CreateTree = wxID_HIGHEST+1,
        ID_CreateGrid,
        ID_CreateText,
        ID_CreateHTML,
        ID_CreateNotebook,
        ID_CreateSizeReport,
        ID_GridContent,
        ID_TextContent,
        ID_TreeContent,
        ID_HTMLContent,
        ID_NotebookContent,
        ID_SizeReportContent,
        ID_CreatePerspective,
        ID_CopyPerspectiveCode,
        ID_AllowFloating,
        ID_AllowActivePane,
        ID_TransparentHint,
        ID_VenetianBlindsHint,
        ID_RectangleHint,
        ID_NoHint,
        ID_HintFade,
        ID_NoVenetianFade,
        ID_TransparentDrag,
        ID_NoGradient,
        ID_VerticalGradient,
        ID_HorizontalGradient,
        ID_LiveUpdate,
        ID_AllowToolbarResizing,
        ID_Settings,
        ID_CustomizeToolbar,
        ID_DropDownToolbarItem,
        ID_NotebookNoCloseButton,
        ID_NotebookCloseButton,
        ID_NotebookCloseButtonAll,
        ID_NotebookCloseButtonActive,
        ID_NotebookAllowTabMove,
        ID_NotebookAllowTabExternalMove,
        ID_NotebookAllowTabSplit,
        ID_NotebookWindowList,
        ID_NotebookScrollButtons,
        ID_NotebookTabFixedWidth,
        ID_NotebookArtGloss,
        ID_NotebookArtSimple,
        ID_NotebookAlignTop,
        ID_NotebookAlignBottom,

        ID_SampleItem,

		/*FileMenuEvents*/
		ID_StartScenario /*= 1*/,
		ID_MonitorSound,
		ID_ImportVocalSounds,
		ID_ImportMediaFiles,
		ID_AddMediaFiles,
		ID_Exit,

		/*ViewMenuEvents*/
		ID_PatientInstructorMonitor,

		/*SimulationMenuEvents*/
		ID_StartSimulation,
		ID_PauseSimulation,
		ID_FastForward,
		ID_HaltScenario,
		ID_DefaultScenario,

		/*EditMenuEvents*/
		ID_MonitorSetup,
		ID_VitalSigns,

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
		ID_adminOverwritePresetLayouts_choice,
        ID_FirstPerspective = ID_CreatePerspective+1000
    };

public:
    MyFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MyFrame();

    wxAuiDockArt* GetDockArt();
    void DoUpdate();

private:
    wxTextCtrl* CreateTextCtrl(const wxString& text = wxEmptyString);
    wxGrid* CreateGrid();
    wxTreeCtrl* CreateTreeCtrl();
    wxSizeReportCtrl* CreateSizeReportCtrl(int width = 80, int height = 80);
    wxPoint GetStartPosition();
    wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL);
    wxAuiNotebook* CreateNotebook();

    wxString GetIntroText();

private:

    void OnEraseBackground(wxEraseEvent& evt);
    void OnSize(wxSizeEvent& evt);

    void OnCreateTree(wxCommandEvent& evt);
    void OnCreateGrid(wxCommandEvent& evt);
    void OnCreateHTML(wxCommandEvent& evt);
    void OnCreateNotebook(wxCommandEvent& evt);
    void OnCreateText(wxCommandEvent& evt);
    void OnCreateSizeReport(wxCommandEvent& evt);
    void OnChangeContentPane(wxCommandEvent& evt);
    
    void OnCreatePerspective(wxCommandEvent& evt);
    void OnCopyPerspectiveCode(wxCommandEvent& evt);
    void OnRestorePerspective(wxCommandEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnCustomizeToolbar(wxCommandEvent& evt);
    void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnTabAlignment(wxCommandEvent &evt);

    void OnGradient(wxCommandEvent& evt);
    void OnToolbarResizing(wxCommandEvent& evt);
    void OnManagerFlag(wxCommandEvent& evt);
    void OnNotebookFlag(wxCommandEvent& evt);
    void OnUpdateUI(wxUpdateUIEvent& evt);

   

private:

    wxAuiManager m_mgr;
    wxArrayString m_perspectives;
    wxMenu* m_perspectives_menu;
    long m_notebook_style;
    long m_notebook_theme;

private:

		/*FileMenuEvents*/
		void onStartScenario(wxCommandEvent& event);
		void onMonitorSound(wxCommandEvent& event);
		void onImportVocalSounds(wxCommandEvent& event);
		void onImportMediaFiles(wxCommandEvent& event);
		void onAddMediaFiles(wxCommandEvent& event);
		void onExit(wxCommandEvent& event);

		/*ViewMenuEvents*/
		void onPatientInstructorMonitor(wxCommandEvent& event);

		/*SimulationMenuEvents*/
		void onStartSimulation(wxCommandEvent& event);
		void onPauseSimulation(wxCommandEvent& event);
		void onFastForward(wxCommandEvent& event);
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

		void DefaultScreenLayout();
		//void setScreenLayout(int mode);
		void setSubScreensOnToolBar();

		void onSaveCustomLayout(wxCommandEvent& event);
		void onLoadCustomLayout(wxCommandEvent& event);

		void toggleMannequin();
		void toggleInstructorPatientMonitor();
		void toggleEventsLog();
		void toggleEventsList();
		void toggleScenarioControls();

		void OnPaneClose(wxAuiManagerEvent& evt);
		void onPaneMaximize(wxAuiManagerEvent& evt);
		void onRestorePane(wxAuiManagerEvent& evt);

		void onOverwritePresetLayout(wxCommandEvent& event);

		void overWritePresetLayout(int chosenPresetLayout);

		void loadAllPresetLayouts();

		void updateLayoutForCurrentScreenSize(int chosenScreenLayout);
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
		wxMenu *menuSimulation;
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

class SettingsPanel : public wxPanel
{
    enum
    {
        ID_PaneBorderSize = wxID_HIGHEST+1,
        ID_SashSize,
        ID_CaptionSize,
        ID_BackgroundColor,
        ID_SashColor,
        ID_InactiveCaptionColor,
        ID_InactiveCaptionGradientColor,
        ID_InactiveCaptionTextColor,
        ID_ActiveCaptionColor,
        ID_ActiveCaptionGradientColor,
        ID_ActiveCaptionTextColor,
        ID_BorderColor,
        ID_GripperColor
    };

public:
	SettingsPanel(wxWindow* parent, MyFrame* frame);

private:

    wxBitmap CreateColorBitmap(const wxColour& c);
	void UpdateColors();
	void OnPaneBorderSize(wxSpinEvent& event);
	void OnSashSize(wxSpinEvent& event);
	void OnCaptionSize(wxSpinEvent& event);
    void OnSetColor(wxCommandEvent& event);

    MyFrame* m_frame;
    wxSpinCtrl* m_border_size;
    wxSpinCtrl* m_sash_size;
    wxSpinCtrl* m_caption_size;
    wxBitmapButton* m_inactive_caption_text_color;
    wxBitmapButton* m_inactive_caption_gradient_color;
    wxBitmapButton* m_inactive_caption_color;
    wxBitmapButton* m_active_caption_text_color;
    wxBitmapButton* m_active_caption_gradient_color;
    wxBitmapButton* m_active_caption_color;
    wxBitmapButton* m_sash_color;
    wxBitmapButton* m_background_color;
    wxBitmapButton* m_border_color;
    wxBitmapButton* m_gripper_color;

    DECLARE_EVENT_TABLE()
};

#endif