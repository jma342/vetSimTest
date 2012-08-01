#include "auidemo.h"
#include "wx/grid.h"

#ifndef EVENTSLOG_H
#define EVENTSLOG_H


//namespace vetSimulatorUI
//{
	class EventsLog : public MyScrolledWindowSmart
	{
	public:
		EventsLog(wxWindow *parent);
		
		//virtual void OnDraw(wxDC& dc);

		bool updateLog(int msg_sndr,wxString message,long milliSecondsElapsed);
		int testVar;

		enum
		{
			SCENRAIO_FRAME_MSG = 1,
			AVATAR_MSG,
			MANNEQUIN_MSG,
			EVENT_LIST_MSG,

		};
		
	private:
		
		wxGrid *eventsLogGrid;

	};
//}

#endif