#ifndef SCROLLEDWINDOWS_H
#define	SCROLLEDWINDOWS_H

#include "auidemo.h"


// base class for both of them
class MyScrolledWindowBase : public wxScrolled<wxWindow>
{
public:
	MyScrolledWindowBase(wxWindow *parent)
		: wxScrolled<wxWindow>(parent, wxID_ANY,
								wxDefaultPosition, wxDefaultSize,
								wxBORDER_SUNKEN)
	{
		m_nLines = 50;
		m_winSync = NULL;
		m_inDoSync = false;

		wxClientDC dc(this);
		dc.GetTextExtent("Line 17", NULL, &m_hLine);
	}

	// this scrolled window can be synchronized with another one: if this
	// function is called with a non-NULL pointer, the given window will be
	// scrolled to the same position as this one
	void SyncWith(MyScrolledWindowBase *win)
	{
		m_winSync = win;

		DoSyncIfNecessary();
	}

	virtual void ScrollWindow(int dx, int dy, const wxRect *rect = NULL)
	{
		wxScrolled<wxWindow>::ScrollWindow(dx, dy, rect);

		DoSyncIfNecessary();
	}

protected:
	// the height of one line on screen
	int m_hLine;

	// the number of lines we draw
	size_t m_nLines;


private:
	bool WasScrolledFirst() const { return m_inDoSync; }

	void DoSyncIfNecessary()
	{
		if ( m_winSync && !m_winSync->WasScrolledFirst() )
		{
			m_inDoSync = true;

			m_winSync->Scroll(GetViewStart());

			m_inDoSync = false;
		}
	}

	// the window to synchronize with this one or NULL
	MyScrolledWindowBase *m_winSync;

	// the flag preventing infinite recursion which would otherwise happen if
	// one window synchronized the other one which in turn synchronized this
	// one and so on
	bool m_inDoSync;
};

// this class does "smart" redrawing - only redraws the lines which must be
// redrawn and sets the scroll rate and virtual size to affect the
// scrollbars.
//
// Note that this class should produce identical results to the one above.

class MyScrolledWindowSmart : public MyScrolledWindowBase
{
public:
	MyScrolledWindowSmart(wxWindow *parent) : MyScrolledWindowBase(parent)
	{
		// no horz scrolling
		SetScrollRate( 0, m_hLine );
		SetVirtualSize( wxDefaultCoord, ( m_nLines + 1 ) * m_hLine );
	}

	//virtual void OnDraw(wxDC& dc);
};

#endif