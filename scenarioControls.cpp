#include "scenarioControls.h"

//namespace vetSimulatorUI
//{
	const wxWindowIDRef ID_PLAY_BUTTON   = wxWindow::NewControlId();
	const wxWindowIDRef ID_START_SCENARIO_BUTTON   = wxWindow::NewControlId();
	const wxWindowIDRef ID_PAUSE_BUTTON  = wxWindow::NewControlId();
	const wxWindowIDRef ID_STOP_BUTTON   = wxWindow::NewControlId();
	const wxWindowIDRef ID_FAST_FORWARD_BUTTON    = wxWindow::NewControlId();
	const wxWindowIDRef ID_DEBRIEF_BUTTON   = wxWindow::NewControlId();
	const wxWindowIDRef ID_SCENARIO_TIMER   = wxWindow::NewControlId();

	
	BEGIN_EVENT_TABLE(ScenarioControls, wxScrolled<wxWindow>)
		EVT_BUTTON( ID_PLAY_BUTTON,    ScenarioControls::onPlayButton)
		EVT_BUTTON( ID_START_SCENARIO_BUTTON,  ScenarioControls::onStartScenarioButton)
		EVT_BUTTON( ID_PAUSE_BUTTON,   ScenarioControls::onPauseButton)
		EVT_BUTTON( ID_STOP_BUTTON, ScenarioControls::onStopButton)
		EVT_BUTTON( ID_FAST_FORWARD_BUTTON,   ScenarioControls::onFastForwardButton)
		EVT_BUTTON( ID_DEBRIEF_BUTTON,   ScenarioControls::OnDebriefButton)
		EVT_TIMER (ID_SCENARIO_TIMER , ScenarioControls::onTimer)
	END_EVENT_TABLE()

	ScenarioControls::ScenarioControls(wxWindow *parent): MyScrolledWindowSmart(parent)
	{
		milliSecondsElapsed = 1000;

		scenarioPlaying = false;
		scenarioPaused = false;
		scenarioFastFwd = false;

		this->ShowScrollbars(wxSHOW_SB_DEFAULT,wxSHOW_SB_DEFAULT);

		//Scenario Controls
		
	
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

		/*patientTime_label = new wxRichTextCtrl(this, -1,"Frame:", wxPoint(315,106),wxSize(120,30));
		patientTime_label->SetEditable(false);
		patientTime_label->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

		patientTime = new wxRichTextCtrl(this, -1, wxEmptyString, wxPoint(435,106),wxSize(185,30));
		patientTime->SetEditable(false);
		patientTime->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);*/
		
	}

	/*void ScenarioControls::OnDraw(wxDC& dc)
	{

	}*/

	void ScenarioControls::onTimer(wxTimerEvent& event)
	{
		milliSecondsElapsed+=1000;

		int nMinutes = (int)(milliSecondsElapsed/ 60000);
		int nSeconds = (int)((milliSecondsElapsed% 60000)/1000);

		wxString sTimePosition;
		sTimePosition.Printf(wxT("%2i:%02i"), nMinutes, nSeconds);

		time->SetValue(sTimePosition.c_str());
		timeInframe->SetValue(sTimePosition.c_str());
		
	}

	void ScenarioControls::onPlayButton(wxCommandEvent& event)
	{
		if(!scenarioPlaying)
		{
			scenarioTimer->Start(1000);
			scenarioPlaying = true;
		}
		else if(scenarioPaused || scenarioFastFwd)
		{
			scenarioTimer->Start(1000);
			scenarioPaused = false;
			scenarioFastFwd = false;
		}
		
	}

	void ScenarioControls::onStartScenarioButton(wxCommandEvent& event)
	{
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

	void ScenarioControls::onPauseButton(wxCommandEvent& event)
	{
		if(scenarioPlaying && !scenarioPaused)
		{
			scenarioTimer->Stop();
			scenarioPaused = true;
			scenarioFastFwd = false;
		}
	}

	void ScenarioControls::onStopButton(wxCommandEvent& event)
	{
		if(scenarioPlaying)
		{	
			wxString sTimePosition;
			sTimePosition.Printf(wxT("%2i:%02i"), 0, 0);

			time->SetValue(sTimePosition.c_str());
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

	void ScenarioControls::setEventsLog(EventsLog *eventsLog)
	{
		this->eventsLog = (EventsLog*)eventsLog;
	}

	long ScenarioControls::getMilliSecondsElapsed()
	{
		return milliSecondsElapsed;
	}

//}
