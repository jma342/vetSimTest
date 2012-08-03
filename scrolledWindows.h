#ifndef SCROLLEDWINDOWS_H
#define	SCROLLEDWINDOWS_H

#include "mainApplicationWindow.h"


class MyScrolledWindowBase : public wxScrolled<wxWindow>
{
public:
	MyScrolledWindowBase(wxWindow *parent)
		: wxScrolled<wxWindow>(parent, wxID_ANY,
								wxDefaultPosition, wxDefaultSize,
								wxBORDER_SUNKEN)
	{
		m_nLines = 50;

		wxClientDC dc(this);

		//used to set the height which is used to set the scroll rate in the following class
		dc.GetTextExtent("Line 17", NULL, &m_hLine);
	}

	virtual void ScrollWindow(int dx, int dy, const wxRect *rect = NULL)
	{
		wxScrolled<wxWindow>::ScrollWindow(dx, dy, rect);

	}

protected:
	// the height of one line on screen
	int m_hLine;

	// the number of lines we draw
	size_t m_nLines;
};

// only redraws the lines which must be
// redrawn and sets the scroll rate and virtual size to affect the
// scrollbars.
class MyScrolledWindowSmart : public MyScrolledWindowBase
{
public:
	MyScrolledWindowSmart(wxWindow *parent) : MyScrolledWindowBase(parent)
	{
		// no horz scrolling
		SetScrollRate( 0, m_hLine );
		SetVirtualSize( wxDefaultCoord, ( m_nLines + 1 ) * m_hLine );
	}

};

#endif