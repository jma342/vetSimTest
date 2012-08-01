#include "eventsLog.h"


//namespace vetSimulatorUI
//{
	const int ROWSIZE = 25;
	const int TIME_SIZE = 20;
	const int MESSAGE_SENDER_SIZE = 50;
	const int MESSAGE_SIZE = 550;

	const int TIME = 0;
	const int MESSAGE_SENDER = 1;
	const int MESSAGE = 2;


	EventsLog::EventsLog(wxWindow *parent):MyScrolledWindowSmart(parent)
	{
		this->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

		eventsLogGrid = new wxGrid( this,
                       wxID_ANY,
                       wxPoint( 0, 0 ),
                       wxSize( 450, 206) );

		eventsLogGrid->CreateGrid(0,0);

		eventsLogGrid->AppendRows(1);
		eventsLogGrid->AppendCols(3);

		eventsLogGrid->SetCellValue(0,TIME,"TIME");
		eventsLogGrid->SetCellValue(0,MESSAGE_SENDER,"MESSAGE SENDER");
		eventsLogGrid->SetCellValue(0,MESSAGE,"MESSAGE");

		eventsLogGrid->SetRowSize(0,ROWSIZE);
		eventsLogGrid->SetColSize(TIME,TIME_SIZE);
		eventsLogGrid->SetColSize(MESSAGE_SENDER,MESSAGE_SENDER_SIZE);
		eventsLogGrid->SetColSize(MESSAGE,MESSAGE_SIZE);

		eventsLogGrid->AutoSizeColumns(true);
		eventsLogGrid->AutoSizeRows(true);

		eventsLogGrid->SetRowLabelSize(0);
		eventsLogGrid->SetColLabelSize(0);

		eventsLogGrid->SetEditable(false);

		//testVar = value;
	}

	bool EventsLog::updateLog(int msg_sndr,wxString message,long milliSecondsElapsed)
	{
		wxString msg_sndr_str = "";

		if(msg_sndr != SCENRAIO_FRAME_MSG && msg_sndr != AVATAR_MSG && msg_sndr != MANNEQUIN_MSG && msg_sndr != EVENT_LIST_MSG)
		{
			return false;
		}
		else if(msg_sndr == SCENRAIO_FRAME_MSG)
		{
			msg_sndr_str = "SCENRAIO_FRAME";
		}
		else if(msg_sndr == AVATAR_MSG)
		{
			msg_sndr_str = "AVATAR";
		}
		else if(msg_sndr == MANNEQUIN_MSG)
		{
			msg_sndr_str = "MANNEQUNIN";
		}
		else if(msg_sndr == EVENT_LIST_MSG)
		{
			msg_sndr_str = "EVENT LIST";
		}


		int nMinutes = (int)(milliSecondsElapsed/ 60000);
		int nSeconds = (int)((milliSecondsElapsed% 60000)/1000);

		wxString sTimePosition;

		sTimePosition.Printf(wxT("%2i:%02i"), nMinutes, nSeconds);

		eventsLogGrid->AppendRows(1);

		eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,TIME,sTimePosition.c_str());
		eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,MESSAGE_SENDER,msg_sndr_str);
		eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,MESSAGE,message);
		
		eventsLogGrid->AutoSizeColumns(true);
		eventsLogGrid->AutoSizeRow(eventsLogGrid->GetNumberRows()-1,true);

		return true;
		
	}

	/*void EventsLog::OnDraw(wxDC &dc)
	{}*/


//}