#include "eventsLog.h"
#include "eventsList.h"
#include "wx/richtext/richtextctrl.h"

#ifndef SCENARIOCONTROLS_H
#define SCENARIOCONTROLS_H


class ScenarioControls : public MyScrolledWindowSmart
{
public:
	ScenarioControls(wxWindow *parent);

	long getScenarioMilliSecondsElapsed();
	void setEventsLog(EventsLog *);
	void transitionToNextState();
	void startScenario();
	void playScenario();
	void stopScenario();
	void pauseScenario();

private:
		
	wxButton *play;
	wxButton *start;
	wxButton *stop;
	wxButton *nextState;
	wxButton *pause;
	wxButton *dbrief;
	wxRichTextCtrl *time;
	wxRichTextCtrl *time_label;
	wxRichTextCtrl *scenarioTitle;
	wxRichTextCtrl *scenarioTitle_label;
	wxRichTextCtrl *frameTitle;
	wxRichTextCtrl *frameTitle_label;
	wxRichTextCtrl *timeInScenario;
	wxRichTextCtrl *timeInScenario_label;
	wxRichTextCtrl *timeInframe;
	wxRichTextCtrl *timeInframe_label;
	wxRichTextCtrl *patientTime;
	wxRichTextCtrl *patientTime_label;

	EventsLog *eventsLog;

	wxTimer *scenarioTimer;
	wxTimer *frameTimer;
	wxTimer *patientTimer;

	long scenarioMilliSecondsElapsed;
	long frameMilliSecondsElapsed;

	boolean scenarioPlaying;
	boolean scenarioPaused;
	boolean scenarioStarted;

	void onStartScenarioButton(wxCommandEvent& event);
	void onPlayButton(wxCommandEvent& event);
	void onPauseButton(wxCommandEvent& event);
	void onStopButton(wxCommandEvent& event);
	void onNextStateButton(wxCommandEvent& event);
	void OnDebriefButton(wxCommandEvent& event);
	void onTimer(wxTimerEvent& event);

	DECLARE_EVENT_TABLE()
};


#endif