
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeImageQueue.h"
#include "DlgProxy.h"
#include "uEyeImageQueueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuEyeImageQueueDlgAutoProxy

IMPLEMENT_DYNCREATE(CuEyeImageQueueDlgAutoProxy, CCmdTarget)

CuEyeImageQueueDlgAutoProxy::CuEyeImageQueueDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CuEyeImageQueueDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CuEyeImageQueueDlg)))
		{
			m_pDialog = reinterpret_cast<CuEyeImageQueueDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CuEyeImageQueueDlgAutoProxy::~CuEyeImageQueueDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CuEyeImageQueueDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CuEyeImageQueueDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CuEyeImageQueueDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IuEyeImageQueue to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {9D65F344-D56A-46D0-9116-7BC4B3E0646F}
static const IID IID_IuEyeImageQueue =
{ 0x9D65F344, 0xD56A, 0x46D0, { 0x91, 0x16, 0x7B, 0xC4, 0xB3, 0xE0, 0x64, 0x6F } };

BEGIN_INTERFACE_MAP(CuEyeImageQueueDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CuEyeImageQueueDlgAutoProxy, IID_IuEyeImageQueue, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {F935774A-DA78-4A3B-8EAF-ABFDF44ED676}
IMPLEMENT_OLECREATE2(CuEyeImageQueueDlgAutoProxy, "uEyeImageQueue.Application", 0xf935774a, 0xda78, 0x4a3b, 0x8e, 0xaf, 0xab, 0xfd, 0xf4, 0x4e, 0xd6, 0x76)


// CuEyeImageQueueDlgAutoProxy message handlers
