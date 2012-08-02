#include "scenarioControls.h"

//associates ids with each of the scenario controls
const wxWindowIDRef ID_PLAY_BUTTON   = wxWindow::NewControlId();
const wxWindowIDRef ID_START_SCENARIO_BUTTON   = wxWindow::NewControlId();
const wxWindowIDRef ID_PAUSE_BUTTON  = wxWindow::NewControlId();
const wxWindowIDRef ID_STOP_BUTTON   = wxWindow::NewControlId();
const wxWindowIDRef ID_FAST_FORWARD_BUTTON    = wxWindow::NewControlId();
const wxWindowIDRef ID_DEBRIEF_BUTTON   = wxWindow::NewControlId();
const wxWindowIDRef ID_SCENARIO_TIMER   = wxWindow::NewControlId();

//captures the events emitted by the events list 
//associates each id with a given function
BEGIN_EVENT_TABLE(ScenarioControls, wxScrolled<wxWindow>)
	EVT_BUTTON( ID_PLAY_BUTTON,    ScenarioControls::onPlayButton)
	EVT_BUTTON( ID_START_SCENARIO_BUTTON,  ScenarioControls::onStartScenarioButton)
	EVT_BUTTON( ID_PAUSE_BUTTON,   ScenarioControls::onPauseButton)
	EVT_BUTTON( ID_STOP_BUTTON, ScenarioControls::onStopButton)
	EVT_BUTTON( ID_FAST_FORWARD_BUTTON,   ScenarioControls::onFastForwardButton)
	EVT_BUTTON( ID_DEBRIEF_BUTTON,   ScenarioControls::OnDebriefButton)
	EVT_TIMER (ID_SCENARIO_TIMER , ScenarioControls::onTimer)
END_EVENT_TABLE()

//jma342--initialises the scneario controls object. This object inherits from the MyScrolledWindowSmart class
ScenarioControls::ScenarioControls(wxWindow *parent): MyScrolledWindowSmart(parent)
{
	//used to increment the milliseconds elapsed and keep track of the overall run time of a scenario
	milliSecondsElapsed = 1000;

	scenarioPlaying = false;
	scenarioPaused = false;
	scenarioFastFwd = false;

	//sets up the scrollbars of the scenario controls to be displayed
	//only if necessary
	this->ShowScrollbars(wxSHOW_SB_DEFAULT,wxSHOW_SB_DEFAULT);

	//Scenario Controls
	//intialises each of the scenario control buttons with their respective IDs, size and positions on screen
	start = new wxButton(this, ID_START_SCENARIO_BUTTON,  "Start", wxPoint(5,10),wxSize(180,30));
	start->SetHelpText("Start Scenario");

	play = new wxButton( this, ID_PLAY_BUTTON,  ">", wxPoint(5,42),wxSize(45,25));
	play->SetHelpText("Play");

	pause = new wxButton( this, ID_PAUSE_BUTTON, "||", wxPoint(51,42),wxSize(45,25));
	pause->SetHelpText("Pause");

	stop = new wxButton( this, ID_STOP_BUTTON,  "[]", wxPoint(97,42),wxSize(45,25));
	stop->SetHelpText("Stop");

	ffwd = new wxButton( this, ID_FAST_FORWARD_BUTTON,  ">>|", wxPoint(143,42),wxSize(45,25));
	ffwd->SetHelpText("Next Frame");

		
	//dbrief =  new wxButton(this, ID_DEBRIEF_BUTTON,"Debrief", wxPoint(5,101),wxSize(304,30));
		
	//Scenario Summary
	scenarioTitle_label = new wxRichTextCtrl(this, -1,"Scnr:", wxPoint(5,68),wxSize(50,30));
	scenarioTitle_label->SetEditable(false);
	scenarioTitle_label->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	scenarioTitle = new wxRichTextCtrl(this, -1, wxEmptyString, wxPoint(55,68),wxSize(130,30));
	scenarioTitle->SetEditable(false);
	scenarioTitle->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	time_label = new wxRichTextCtrl(this, -1,"Time:", wxPoint(5,99),wxSize(50,30));
	time_label->SetEditable(false);
	time_label->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	time = new wxRichTextCtrl(this, -1, wxEmptyString, wxPoint(55,99),wxSize(130,30));
	time->SetEditable(false);
	time->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	scenarioTimer = new wxTimer(this,ID_SCENARIO_TIMER);

	frameTitle_label = new wxRichTextCtrl(this, -1,"Frame:", wxPoint(5,130),wxSize(50,30));
	frameTitle_label->SetEditable(false);
	frameTitle_label->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	frameTitle = new wxRichTextCtrl(this, -1, wxEmptyString, wxPoint(55,130),wxSize(130,30));
	frameTitle->SetEditable(false);
	frameTitle->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	timeInframe_label = new wxRichTextCtrl(this, -1,"Time:", wxPoint(5,161),wxSize(50,30));
	timeInframe_label->SetEditable(false);
	timeInframe_label->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	timeInframe = new wxRichTextCtrl(this, -1, wxEmptyString, wxPoint(55,161),wxSize(130,30));
	timeInframe->SetEditable(false);
	timeInframe->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);	
}

//jma342--this timer is used to update the time elapsed for the scenario
//this timer is called every second by default. This is currently determined by
//the Start function in the onPlayButton function.
void ScenarioControls::onTimer(wxTimerEvent& event)
{
	milliSecondsElapsed+=1000;

	int nMinutes = (int)(milliSecondsElapsed/ 60000);
	int nSeconds = (int)((milliSecondsElapsed% 60000)/1000);

	wxString sTimePosition;
	sTimePosition.Printf(wxT("%2i:%02i"), nMinutes, nSeconds);

	//sets the elapsed time for the scenario
	time->SetValue(sTimePosition.c_str());

	//sets the elapsed time for the current frame
	timeInframe->SetValue(sTimePosition.c_str());
		
}

//jma342--triggered when the play button is executed
void ScenarioControls::onPlayButton(wxCommandEvent& event)
{
	//starts timer by intialising it to update the timer
	//every 1000 milliseconds
	if(!scenarioPlaying)
	{
		scenarioTimer->Start(1000);
		scenarioPlaying = true;
	}
	//if the play button is executed after the scenario was paused
	else if(scenarioPaused || scenarioFastFwd)
	{
		scenarioTimer->Start(1000);
		scenarioPaused = false;
		scenarioFastFwd = false;
	}
		
}

//jma342--triggered when the scenario button is executed
void ScenarioControls::onStartScenarioButton(wxCommandEvent& event)
{
	//if the scenario is not playing a file dialog box is used
	//capture the scenario file to be loaded
	if(!scenarioPlaying)
	{
		wxFileDialog 
				openFileDialog(this, _("Load Scenario"), "", "",
								"Screen Layouts (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

		if (openFileDialog.ShowModal() == wxID_CANCEL)
			return;     // the user changed idea...

		//TODO: when the scenario facility is built implement loading of the scenario contents here

		scenarioTimer->Start(1000);
		scenarioPlaying = true;

		eventsLog->updateLog(eventsLog->SCENRAIO_FRAME_MSG,"Scenario Started",milliSecondsElapsed);
	}

}

//jma342--triggered when the pause button is executed
void ScenarioControls::onPauseButton(wxCommandEvent& event)
{
	//only executes if the pause button is not active
	if(scenarioPlaying && !scenarioPaused)
	{
		scenarioTimer->Stop();
		scenarioPaused = true;
		scenarioFastFwd = false;
	}
}

//jma342--triggered when the stop button executed
void ScenarioControls::onStopButton(wxCommandEvent& event)
{
	//only executes if the scenario play button is active...this also
	//includes if the scneario is paused
	if(scenarioPlaying)
	{	
		wxString sTimePosition;
		sTimePosition.Printf(wxT("%2i:%02i"), 0, 0);

		//sets the elapsed time for the scenario
		time->SetValue(sTimePosition.c_str());

		//sets the elapsed time for the current frame
		timeInframe->SetValue(sTimePosition.c_str());

		milliSecondsElapsed = 1000;

		scenarioTimer->Stop();
		scenarioPaused = false;
		scenarioPlaying = false;
		scenarioFastFwd = false;
	}

}

void ScenarioControls::onFastForwardButton(wxCommandEvent& event)
{
	if(scenarioPlaying && !scenarioFastFwd)
	{
		scenarioPaused = false;
		scenarioFastFwd = true;
		scenarioTimer->Start(1);
	}
}

void ScenarioControls::OnDebriefButton(wxCommandEvent& event)
{}

//jma342--initialises the events log pointer
void ScenarioControls::setEventsLog(EventsLog *eventsLog)
{
	this->eventsLog = (EventsLog*)eventsLog;
}

//jma342--returns the milliseconds elapsed
long ScenarioControls::getMilliSecondsElapsed()
{
	return milliSecondsElapsed;
}

