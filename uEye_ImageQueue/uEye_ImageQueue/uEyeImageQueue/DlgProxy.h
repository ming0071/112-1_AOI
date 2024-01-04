
// DlgProxy.h: header file
//

#pragma once

class CuEyeImageQueueDlg;


// CuEyeImageQueueDlgAutoProxy command target

class CuEyeImageQueueDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CuEyeImageQueueDlgAutoProxy)

	CuEyeImageQueueDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CuEyeImageQueueDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CuEyeImageQueueDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CuEyeImageQueueDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

