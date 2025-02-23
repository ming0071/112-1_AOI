

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 22 15:56:42 2018
 */
/* Compiler settings for uEyeImageQueue.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __uEyeImageQueue_h_h__
#define __uEyeImageQueue_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IuEyeImageQueue_FWD_DEFINED__
#define __IuEyeImageQueue_FWD_DEFINED__
typedef interface IuEyeImageQueue IuEyeImageQueue;

#endif 	/* __IuEyeImageQueue_FWD_DEFINED__ */


#ifndef __uEyeImageQueue_FWD_DEFINED__
#define __uEyeImageQueue_FWD_DEFINED__

#ifdef __cplusplus
typedef class uEyeImageQueue uEyeImageQueue;
#else
typedef struct uEyeImageQueue uEyeImageQueue;
#endif /* __cplusplus */

#endif 	/* __uEyeImageQueue_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __uEyeImageQueue_LIBRARY_DEFINED__
#define __uEyeImageQueue_LIBRARY_DEFINED__

/* library uEyeImageQueue */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_uEyeImageQueue;

#ifndef __IuEyeImageQueue_DISPINTERFACE_DEFINED__
#define __IuEyeImageQueue_DISPINTERFACE_DEFINED__

/* dispinterface IuEyeImageQueue */
/* [uuid] */ 


EXTERN_C const IID DIID_IuEyeImageQueue;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9D65F344-D56A-46D0-9116-7BC4B3E0646F")
    IuEyeImageQueue : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IuEyeImageQueueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IuEyeImageQueue * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IuEyeImageQueue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IuEyeImageQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IuEyeImageQueue * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IuEyeImageQueue * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IuEyeImageQueue * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IuEyeImageQueue * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IuEyeImageQueueVtbl;

    interface IuEyeImageQueue
    {
        CONST_VTBL struct IuEyeImageQueueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IuEyeImageQueue_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IuEyeImageQueue_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IuEyeImageQueue_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IuEyeImageQueue_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IuEyeImageQueue_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IuEyeImageQueue_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IuEyeImageQueue_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IuEyeImageQueue_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_uEyeImageQueue;

#ifdef __cplusplus

class DECLSPEC_UUID("F935774A-DA78-4A3B-8EAF-ABFDF44ED676")
uEyeImageQueue;
#endif
#endif /* __uEyeImageQueue_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


