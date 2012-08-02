#include "eventsLog.h"
#include "eventsList.h"
#include "wx/richtext/richtextctrl.h"

#ifndef SCENARIOCONTROLS_H
#define SCENARIOCONTROLS_H


class ScenarioControls : public MyScrolledWindowSmart
{
public:
	ScenarioControls(wxWindow *parent);

	long getMilliSecondsElapsed();
	void setEventsLog(EventsLog *);

private:
		
	wxButton *play;
	wxButton *start;
	wxButton *stop;
	wxButton *ffwd;
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

	long milliSecondsElapsed;

	boolean scenarioPlaying;
	boolean scenarioPaused;
	boolean scenarioFastFwd;

	void onStartScenarioButton(wxCommandEvent& event);
	void onPlayButton(wxCommandEvent& event);
	void onPauseButton(wxCommandEvent& event);
	void onStopButton(wxCommandEvent& event);
	void onFastForwardButton(wxCommandEvent& event);
	void OnDebriefButton(wxCommandEvent& event);
	void onTimer(wxTimerEvent& event);


	DECLARE_EVENT_TABLE()
};


#endif