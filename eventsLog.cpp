#include "eventsLog.h"

const int ROWSIZE = 25;//sets the size of the rows for the events log grid
const int TIME_SIZE = 20;//sets the size of the column for time stamping
const int MESSAGE_SENDER_SIZE = 50;//sets the size of the coumn used to store the object that sent the message 
const int MESSAGE_SIZE = 550;

//stores the column location for each of the 3 columns
const int TIME = 0;
const int MESSAGE_SENDER = 1;
const int MESSAGE = 2;

//intialises the events log object
EventsLog::EventsLog(wxWindow *parent):MyScrolledWindowSmart(parent)
{
	//indicates that neither of the scroll bars will be displayed
	this->ShowScrollbars(wxSHOW_SB_NEVER,wxSHOW_SB_NEVER);

	//initialises the grid for the events log
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

	//ensures the the columns and the rows each adjust based on their contest
	eventsLogGrid->AutoSizeColumns(true);
	eventsLogGrid->AutoSizeRows(true);

	eventsLogGrid->SetRowLabelSize(0);
	eventsLogGrid->SetColLabelSize(0);

	//ensures that the grid is not editable from the UI as it
	//receives messages from other events triggered on the UI and by the physical 
	//mannequin it self
	eventsLogGrid->SetEditable(false);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(eventsLogGrid,wxSizerFlags(1).Expand());
    SetSizer(sizer);
}

//jma342--used to add capture events in the events log
bool EventsLog::updateLog(int msg_sndr,wxString message,long milliSecondsElapsed)
{
	wxString msg_sndr_str = "";

	//ensures messages are received from the exepcted objects
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

	//appends the next row to receive log data
	eventsLogGrid->AppendRows(1);

	//sets the values for each of the cells in the next row to be updated
	eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,TIME,sTimePosition.c_str());
	eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,MESSAGE_SENDER,msg_sndr_str);
	eventsLogGrid->SetCellValue(eventsLogGrid->GetNumberRows()-1,MESSAGE,message);

	//resizes columns and last updated row if necessary
	eventsLogGrid->AutoSizeColumns(true);
	eventsLogGrid->AutoSizeRow(eventsLogGrid->GetNumberRows()-1,true);

	//indicates successful update
	return true;
		
}
