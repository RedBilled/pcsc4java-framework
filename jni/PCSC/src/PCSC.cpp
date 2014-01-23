/********************************************************
* Copyright (c) 2011, REDBILLED All Rights Rese_lResulted
*
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF REDBILLED
* The copyright notice above does not evidence any actual
* or intended publication of such source code.
* =======================================================
* JNI for the using of PCSC
* =======================================================
* File : PCSC.cpp
*
* List of functions
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardEstablishContext
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardReleaseContext
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardCancel
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardIsValidContext
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardLocateCards
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardListReaders
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardConnect
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardTransmit
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardStatus
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardDisconnect
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetStatusChange
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardBeginTransaction
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardEndTransaction
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardControl
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardReconnect
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetAttrib
*	Java_fr_redbilled_security_pcscforjava_PCSC_SCardSetAttrib
*
* UPDATE HISTORY
*--------------------------------------------------------
*Version | Author     | Date     |
*--------------------------------------------------------
*  1.0	   ML	       22/06/2011
**********************************************************/

#include "../inc/PCSC.h"

#if(defined WIN32) || (defined WIN64)

	BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
		{
			switch (ul_reason_for_call)
			{
			case DLL_PROCESS_ATTACH:
			case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:
				break;
			}
			return TRUE;
		}
		
#elif(defined LINUX32) || (defined LINUX64) || (defined __APPLE__)  // #if(defined WIN32) || (defined WIN64)

	__attribute__((constructor))
	static bool initializer(void)
	{
		return true;
	}

	__attribute__((destructor))
	static bool finalizer(void)
	{
		return true;
	}		

#endif // #elif(defined LINUX32) || (defined LINUX64) || (defined __APPLE__)

DWORD	g_dwOldReaders = 0;

DWORD Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(DWORD dwScardValue, UCHAR ucProtocol, UCHAR ucEntry)
{
	#if(defined WIN32) || (defined WIN64)

		// The high level is based on this value nothing to do
		return dwScardValue;

	#else
		
		// Entry is Windows values and we must return MUSCLE values			
		if(ucEntry)
		{
			if(ucProtocol)
			{
				if(dwScardValue == 0x10000)
					return SCARD_PROTOCOL_RAW;
				else
					return dwScardValue;
			}
			
			switch(dwScardValue)
			{
				case 0:
					return SCARD_UNKNOWN;

				case 1:
					return SCARD_ABSENT;

				case 2:      
					return SCARD_PRESENT;

				case 3:
					return SCARD_SWALLOWED;

				case 4:
					return SCARD_POWERED;

				case 5:
					return SCARD_NEGOTIABLE;

				case 6:
					return SCARD_SPECIFIC;

				default:
					return dwScardValue;	
			}
		}	
		// Ouput value is MUSCLE values and we must return the Windows values
		else
		{
			if(ucProtocol)
			{
				if(dwScardValue == SCARD_PROTOCOL_RAW)
					return 0x10000;
				else
					return dwScardValue;
			}

			switch(dwScardValue)
			{
				case SCARD_UNKNOWN:
					return 0;

				case SCARD_ABSENT:
					return 0x0001;

				case SCARD_PRESENT:
					return 0x0002;

				case SCARD_SWALLOWED:      
					return 0x0003;

				case SCARD_POWERED:
					return 0x0004;
	    			
				case SCARD_NEGOTIABLE:
					return 0x0005;
		
				case SCARD_SPECIFIC:
					return 0x0006;
		
				default:
					return dwScardValue;	
			}
		}
		
	#endif		
}

JNIEXPORT jlong JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardEstablishContext(JNIEnv * env, jclass object, jint iScope)
{
	long			_lResult;
	SCARDCONTEXT 	_contextId;
	DWORD			_iScope = iScope;

	printInDebug("--> SCardEstablishContext\n");
	
	switch(_iScope)
	{
		case SCARD_SCOPE_USER:
			printInDebug("SCardEstablishContext scope: SCARD_SCOPE_USER\n");
			break;
			
		case SCARD_SCOPE_SYSTEM:
			printInDebug("SCardEstablishContext scope: SCARD_SCOPE_SYSTEM\n");
			break;
		
		default:
			printInDebug("SCardEstablishContext scope error!\n");
			printInDebug("<-- SCardTransmit Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return 0;
	}

	_lResult = SCardEstablishContext(_iScope, NULL, NULL, (LPSCARDCONTEXT)&_contextId);
	
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("ScardEstablishContext failed!\n");
		printInDebug("<-- SCardEstablishContext Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return 0;
	}
	else
	{
		printInDebug("ScardEstablishContext success %lld\n", _contextId);
	}

	printInDebug("<-- SCardEstablishContext\n");
	
	return _contextId;
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardReleaseContext(JNIEnv * env, jclass object, jlong lContextId)
{
	long	_lResult;
	
	printInDebug("--> SCardReleaseContext\n");
	
	_lResult = SCardReleaseContext((SCARDCONTEXT)lContextId);
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardReleaseContext failed!\n");
		printInDebug("<-- SCardReleaseContext Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
	}
	else
	{	
		printInDebug("SCardReleaseContext success\n");
	}
		
	printInDebug("<-- SCardReleaseContext\n");
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardCancel(JNIEnv * env, jclass object, jlong lContextId)
{
	long	_lResult;
	
	printInDebug("--> SCardCancel\n");
	
	_lResult = SCardCancel((SCARDCONTEXT)lContextId);
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardCancel failed!\n");
		printInDebug("<-- SCardCancel Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
	}
	else
	{	
		printInDebug("SCardCancel success\n");
	}
		
	printInDebug("<-- SCardCancel\n");
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardIsValidContext(JNIEnv * env, jclass object, jlong lContextId)
{
	long _lResult;

	printInDebug("--> SCardIsValidContext\n");
	
	_lResult = SCardIsValidContext((SCARDCONTEXT)lContextId);

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardIsValidContext failed!\n");
		printInDebug("<-- SCardIsValidContext Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
	}
	else
	{	
		printInDebug("SCardIsValidContext success\n");
	}
	

	printInDebug("<-- SCardIsValidContext\n");
}

JNIEXPORT jobjectArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardListReaders(JNIEnv * env, jclass object, jlong lContextId)
{
	long			_lResult;
	LPTSTR			_psReadersList	= NULL;
	DWORD			_lenReadersList;
	DWORD			_i,_j,_z;
	BOOL			_bIsReaderOk = false;
	CHAR			_szReader[NB_MAX_READERS_SUPPORTED][READER_NAME_SZ];
	jobjectArray	_objReturn = NULL;
	
	printInDebug("--> SCardListReaders\n");
	
	do
	{
		free(_psReadersList);
	
		_lResult = SCardListReaders((SCARDCONTEXT)lContextId, NULL, NULL, &_lenReadersList);
		
		if(_lResult != SCARD_S_SUCCESS)
		{
			printInDebug("ScardListReaders failed!\n");
			printInDebug("<-- SCardListReaders Exception: %ld\n", _lResult);
			launchException(env, _lResult);
			return NULL;
		}

		g_dwOldReaders = _lenReadersList;
		
		printInDebug("ScardListReaders Success with length: %ld!\n", _lenReadersList);

		_psReadersList = (LPTSTR) malloc(_lenReadersList);

		_lResult = SCardListReaders((SCARDCONTEXT)lContextId, NULL, (LPTSTR)_psReadersList, &_lenReadersList);
	}
	while(_lResult == SCARD_E_INSUFFICIENT_BUFFER);

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("ScardListReaders failed!\n");
		printInDebug("<-- SCardListReaders Exception: %ld\n", _lResult);
		free(_psReadersList);
		launchException(env, _lResult);
		return NULL;
	}
	else
	{
		g_dwOldReaders = _lenReadersList;

		for (_i=0; _i < NB_MAX_READERS_SUPPORTED; _i++)
			memset(&_szReader[_i][0],0, sizeof(&_szReader[_i][0]));

		if (_lenReadersList > 2) // \0\0
		{
			_z = 0;
			_j = 0;
			
			for (_i = 0; _i < _lenReadersList; _i =_i + 1)
            {
				memcpy(&_szReader[_j][_z], &_psReadersList[_i], 1);

				if (_psReadersList[_i] == '\0')
				{
					_z = 0;
					_j++;

					if(_i < _lenReadersList-1)
						printInDebug("SCardListReaders New reader found %s\n", _szReader[_j-1]); 
				}
				else
				{
					_z++;
				}
			}
		}
		else
		{
			printInDebug("Scard List Readers failed!\n");
			printInDebug("<-- SCardListReaders Exception: %ld\n", SCARD_E_NO_READERS_AVAILABLE);
			free(_psReadersList);
			launchException(env, SCARD_E_NO_READERS_AVAILABLE);
			return NULL;
		}

		// The last index is just the end of the string not a complete reader name
		_j--;

		printInDebug("SCardListReaders Retrieving Readers Names Success: %d reader(s) found!\n", _j);

		_objReturn = (jobjectArray)env->NewObjectArray(_j, env->FindClass("java/lang/String"), env->NewStringUTF(""));

		for(_i = 0; _i < _j; _i++) 
			env->SetObjectArrayElement(_objReturn, _i, env->NewStringUTF(_szReader[_i]));
	}

	free(_psReadersList);

	printInDebug("<-- SCardListReaders\n");

	return _objReturn;
}

JNIEXPORT jlong JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardConnect(JNIEnv *env, jclass object, jlong lContextId, jstring sReaderName, 
																				 jint iShareMode, jint iPreferredProtocols)
{
	long		_lResult;
	SCARDHANDLE _cardHandle;
	DWORD		_activeProtocol;
	const CHAR	*_szReaderName = NULL;
	
	printInDebug("--> SCardConnect\n");
	
	switch(iShareMode)
    {
        case SCARD_SHARE_SHARED:
            printInDebug("SCardConnect SCARD_SHARE_SHARED\n");
            break;

        case SCARD_SHARE_EXCLUSIVE:
            printInDebug("SCardConnect SCARD_SHARE_EXCLUSIVE\n");
            break;

		case SCARD_SHARE_DIRECT:
			printInDebug("SCardConnect SCARD_SHARE_DIRECT\n");
			break;

        default:
            printInDebug("SCardConnect incorrect share mode\n");
            printInDebug("<-- SCardConnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
            return 0;
    }

    switch(iPreferredProtocols)
    {
		case SCARD_PROTOCOL_UNDEFINED:
			printInDebug("SCardConnect SCARD_PROTOCOL_UNDEFINED\n");
			break;
	
		case SCARD_PROTOCOL_T0:
            printInDebug("SCardConnect SCARD_PROTOCOL_T0\n");
            break;

        case SCARD_PROTOCOL_T1:
            printInDebug("SCardConnect SCARD_PROTOCOL_T1\n");
            break;

        case SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1:
            printInDebug("SCardConnect SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1\n");
            break;

        default:
            printInDebug("SCardConnect incorrect preferred protocols\n");
            printInDebug("<-- SCardConnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return 0;
    }
	
	_szReaderName = env->GetStringUTFChars((jstring)sReaderName, 0);
	
	printInDebug("SCardConnect %s\n", _szReaderName);
	
	_lResult = SCardConnect((SCARDCONTEXT)lContextId, _szReaderName, iShareMode, iPreferredProtocols, &_cardHandle, &_activeProtocol);
	
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardConnect failed!\n");
		printInDebug("<-- SCardConnect Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return 0;
	}
	else
	{	
		printInDebug("SCardConnect success!\n");
		
		// Display the active protocol.
		switch (_activeProtocol)
		{
			case SCARD_PROTOCOL_T0:
				printInDebug("SCardConnect Active protocol T0\n"); 
				break;

			case SCARD_PROTOCOL_T1:
				printInDebug("SCardConnect Active protocol T1\n"); 
				break;

			case SCARD_PROTOCOL_RAW:
				_activeProtocol = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(_activeProtocol, 1, 0);
				printInDebug("SCardConnect Active protocol RAW\n"); 
				break;

			case SCARD_PROTOCOL_UNDEFINED:
			default:
				printInDebug("SCardConnect Active protocol unnegotiated or unknown\n"); 
				break;
		}
	}

	printInDebug("<-- SCardConnect %ld\n", _cardHandle);

	return _cardHandle;
}

JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardTransmit(JNIEnv *env, jclass object, jlong lCardHandle, jint iProtocol, 
																					   jbyteArray pBCommand, jint iOffset, jint iLength)

{
	long					_lResult;
	BYTE					_pBResponse[BUFFER_MAX_SZ];
	LPSCARD_IO_REQUEST		_protocolResponse = NULL;
	LPCSCARD_IO_REQUEST		_protocolCurrent;
	DWORD					_dwLength = BUFFER_MAX_SZ;
	jbyteArray				_objReturn = NULL;
	BYTE					*_pBCommand;
	
	printInDebug("--> SCardTransmit\n");

	_pBCommand = (BYTE *) (env)->GetByteArrayElements(pBCommand, NULL);

	iProtocol = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(iProtocol, 1, 1);

	switch(iProtocol)
	{
		case SCARD_PROTOCOL_T0:
			_protocolCurrent = SCARD_PCI_T0;
			printInDebug("SCardTransmit Protocol current: SCARD_PCI_T0\n");
			break;

		case SCARD_PROTOCOL_T1:
			_protocolCurrent = SCARD_PCI_T1;
			printInDebug("SCardTransmit Protocol current: SCARD_PCI_T1\n");
			break;

		case SCARD_PROTOCOL_RAW:
			_protocolCurrent = SCARD_PCI_RAW;
			printInDebug("SCardTransmit Protocol current: SCARD_PCI_RAW\n");
			break;

		default:
			printInDebug("SCardTransmit Protocol error!\n");
			printInDebug("<-- SCardTransmit Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
	}

	printInDebug("SCardTransmit length: %d command: %#x %#x ... %#x %#x\n", iLength, _pBCommand[0], _pBCommand[1], _pBCommand[iLength - 2], _pBCommand[iLength - 1]);

	_lResult = SCardTransmit((SCARDHANDLE)lCardHandle, _protocolCurrent, _pBCommand, iLength, _protocolResponse, _pBResponse, &_dwLength);
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardTransmit failed!\n");
		printInDebug("<-- SCardTransmit Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
	}
	else
	{
		printInDebug("SCardTransmit success %d data retrieved\n", _dwLength);

		if(_dwLength > 0)
		{
			_objReturn = (jbyteArray)env->NewByteArray(_dwLength);
			env->SetByteArrayRegion(_objReturn, 0, _dwLength, (jbyte*)_pBResponse);

			if(_dwLength >=2)
			{
				printInDebug("SCardTransmit success card response %d bytes: %#x %#x ... %#x %#x\n", _dwLength, _pBResponse[0], _pBResponse[1], 
					_pBResponse[_dwLength-2], _pBResponse[_dwLength-1]);
			}
			else
			{
				printInDebug("SCardTransmit success card response %d byte: %#x\n", _dwLength, _pBResponse[0]);
			}
		}
		else
		{
			printInDebug("<-- SCardTransmit Invalid card response\n");
			launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
		}
	}

	(env)->ReleaseByteArrayElements(pBCommand, (jbyte *)_pBCommand, JNI_ABORT);

	printInDebug("<-- SCardTransmit\n");

	return _objReturn;
}

JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardStatus(JNIEnv *env, jclass object, jlong lCardHandle, 
																					 jbyteArray pBStatus, jobjectArray psReaderNames)
{
	long		_lResult;
	jbyteArray	_objReturn = NULL;
	CHAR		_szReaderName[1][READER_NAME_SZ];
	DWORD		_dwReaderLength = READER_NAME_SZ;
	BYTE		*_pBStatus = (BYTE*)env->GetByteArrayElements(pBStatus, NULL);
	DWORD		_pdwState;
	DWORD		_pdwProtocol;
	BYTE		_pBAtr[ATR_BUFFER_MAX_SZ];
	DWORD		_dwAtrLength = ATR_BUFFER_MAX_SZ;
		
	printInDebug("--> SCardStatus %ld\n", lCardHandle);

	if(env->GetArrayLength(pBStatus) != 2)
	{
		printInDebug("SCardStatus parameter error\n");
		printInDebug("<-- SCardStatus Exception: %ld\n", SCARD_E_INVALID_VALUE);
		launchException(env, SCARD_E_INVALID_VALUE);
		return NULL;
	}
	
	_pdwState = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(_pBStatus[0], 0, 1);
	_pdwProtocol = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(_pBStatus[1], 1, 1);

	printInDebug("SCardStatus State:%d Protocol:%d\n", _pdwState, _pdwProtocol);

	_lResult = SCardStatus((SCARDHANDLE)lCardHandle, _szReaderName[0], &_dwReaderLength, &_pdwState, &_pdwProtocol, _pBAtr, &_dwAtrLength);
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("SCardStatus failed!\n");
		printInDebug("<-- SCardStatus Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
	}
	else
	{
		printInDebug("SCardStatus success for reader: %s\n", _szReaderName[0]);

		switch(_pdwState)
		{
			case SCARD_ABSENT:
				printInDebug("SCardStatus state SCARD_ABSENT\n");
				break;

			case SCARD_PRESENT:
				printInDebug("SCardStatus state SCARD_PRESENT\n");
				break;

			case SCARD_SWALLOWED:
				printInDebug("SCardStatus state SCARD_SWALLOWED\n");
				break;

			case SCARD_POWERED:
				printInDebug("SCardStatus state SCARD_POWERED\n");
				break;

			case SCARD_NEGOTIABLE:
				printInDebug("SCardStatus state SCARD_NEGOTIABLE\n");
				break;

			case SCARD_SPECIFIC:
				printInDebug("SCardStatus state SCARD_SPECIFIC\n");
				break;

			case SCARD_UNKNOWN:
				printInDebug("SCardStatus state SCARD_UNKNOWN\n");
				break;
			
			default:
				break;
				/*printInDebug("SCardStatus state error %d\n", _pdwState);
				printInDebug("<-- SCardStatus Exception: %ld\n", SCARD_E_INVALID_VALUE);
				launchException(env, SCARD_E_INVALID_VALUE);
				return NULL;*/
		}

		_pdwState = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(_pdwState, 0, 0);

		printInDebug("SCardStatus real state %d\n", _pdwState);

		// Display the active protocol.
		switch (_pdwProtocol)
		{
			case SCARD_PROTOCOL_T0:
				printInDebug("SCardStatus Active protocol T0\n"); 
				break;

			case SCARD_PROTOCOL_T1:
				printInDebug("SCardStatus Active protocol T1\n"); 
				break;

			case SCARD_PROTOCOL_RAW:
				_pdwProtocol = Java_fr_redbilled_security_pcscforjava_PCSC_GetCorrectSCardValue(_pdwProtocol, 1, 0);
				printInDebug("SCardStatus Active protocol RAW\n"); 
				break;

			case SCARD_PROTOCOL_UNDEFINED:
			default:
				printInDebug("SCardStatus Active protocol unnegotiated or unknown\n"); 
				break;
		}
		
		if(_dwAtrLength > 0)
		{
			jstring _strFinal;

			printInDebug("SCardStatus ATR %d bytes %#x %#x ... %#x %#x\n", _dwAtrLength, _pBAtr[0], _pBAtr[1], _pBAtr[_dwAtrLength-2], _pBAtr[_dwAtrLength-1]);
			
			_objReturn = (jbyteArray)env->NewByteArray(_dwAtrLength);
			env->SetByteArrayRegion(_objReturn, 0, _dwAtrLength, (jbyte*)_pBAtr);
			
			_pBStatus[0] = (BYTE)_pdwState;
			_pBStatus[1] = (BYTE)_pdwProtocol;
			
			env->SetByteArrayRegion(pBStatus, 0, 1, (jbyte*)_pBStatus);
			env->SetByteArrayRegion(pBStatus, 1, 1, (jbyte*)&_pBStatus[1]);

			printInDebug("SCardStatus %s\n", _szReaderName[0]);
			_strFinal = env->NewStringUTF(_szReaderName[0]);
			env->SetObjectArrayElement(psReaderNames, 0, _strFinal);
		}
		else
		{
			printInDebug("SCardStatus ATR error\n");
			printInDebug("<-- SCardStatus Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
		}
	}

	printInDebug("<-- SCardStatus\n");

	return _objReturn;
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardDisconnect(JNIEnv *env, jclass object, jlong lCardHandle, jint iDisposition)
{
	long _lResult;

	printInDebug("--> SCardDisconnect\n");

	switch(iDisposition)
	{
		case SCARD_LEAVE_CARD:
			printInDebug("SCardDisconnect SCARD_LEAVE_CARD\n");
			break;
		
		case SCARD_RESET_CARD:
			printInDebug("SCardDisconnect SCARD_RESET_CARD\n");
			break;

		case SCARD_UNPOWER_CARD:
			printInDebug("SCardDisconnect SCARD_UNPOWER_CARD\n");
			break;

		case SCARD_EJECT_CARD:
			printInDebug("SCardDisconnect PCSC_SCARD_EJECT_CARD\n");
			break;

		default:
			printInDebug("<-- SCardDisconnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return;
	}

	_lResult = SCardDisconnect((SCARDHANDLE)lCardHandle, iDisposition);
	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("<-- SCardDisconnect Exception: %ld\n", _lResult);
		launchException(env, _lResult);
	}

	printInDebug("<-- SCardDisconnect\n");
}

JNIEXPORT jintArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetStatusChange(JNIEnv *env, jclass object, jlong lContextId, 
																							 jlong lTimeout, jintArray piCurrentState, 
																							 jobjectArray psReaderNames)
{

	long				_lResult;
	SCARDCONTEXT		_contextId = (SCARDCONTEXT)lContextId;
	DWORD				_dwNbElements = env->GetArrayLength(piCurrentState);
	SCARD_READERSTATE	*_readerState = (SCARD_READERSTATE *)malloc(_dwNbElements * sizeof(SCARD_READERSTATE));
	jintArray			_objReturn = (jintArray)env->NewIntArray(_dwNbElements);
	DWORD				_i = 0;
	int					*_piCurrentState = (int*)env->GetIntArrayElements(piCurrentState, NULL);

	printInDebug("--> SCardGetStatusChange context: %lld\n", lContextId);

	memset(_readerState, 0, _dwNbElements * sizeof(SCARD_READERSTATE));
	
	if(_dwNbElements != env->GetArrayLength(piCurrentState))
	{
		printInDebug("<-- SCardGetStatusChange %d - %d Exception: %ld\n", _dwNbElements, env->GetArrayLength(piCurrentState), SCARD_E_INVALID_VALUE);
		launchException(env, SCARD_E_INVALID_VALUE);
		return NULL;
	}

	for(_i = 0; _i < _dwNbElements; _i++)
	{
		printInDebug("SCardGetStatusChange current state:\n");

		if(_piCurrentState[_i] == SCARD_STATE_UNAWARE)
			printInDebug("\tSCARD_STATE_UNAWARE\n");
		if((_piCurrentState[_i] & SCARD_STATE_IGNORE) == SCARD_STATE_IGNORE)
			printInDebug("\tSCARD_STATE_IGNORE\n");
		if((_piCurrentState[_i] & SCARD_STATE_UNAVAILABLE) == SCARD_STATE_UNAVAILABLE)
			printInDebug("\tSCARD_STATE_UNAVAILABLE\n");
		if((_piCurrentState[_i] & SCARD_STATE_EMPTY) == SCARD_STATE_EMPTY)
			printInDebug("\tSCARD_STATE_EMPTY\n");
		if((_piCurrentState[_i] & SCARD_STATE_PRESENT) == SCARD_STATE_PRESENT)
			printInDebug("\tSCARD_STATE_PRESENT\n");
		if((_piCurrentState[_i] & SCARD_STATE_ATRMATCH) == SCARD_STATE_ATRMATCH)
			printInDebug("\tSCARD_STATE_ATRMATCH\n");
		if((_piCurrentState[_i] & SCARD_STATE_EXCLUSIVE) == SCARD_STATE_EXCLUSIVE)
			printInDebug("\tSCARD_STATE_EXCLUSIVE\n");
		if((_piCurrentState[_i] & SCARD_STATE_INUSE) == SCARD_STATE_INUSE)
			printInDebug("\tSCARD_STATE_INUSE\n");
		if((_piCurrentState[_i] & SCARD_STATE_MUTE) == SCARD_STATE_MUTE)
			printInDebug("\tSCARD_STATE_MUTE\n");
		
		
		jobject _jReaderName = env->GetObjectArrayElement(psReaderNames, _i);
		_readerState[_i].szReader = env->GetStringUTFChars((jstring)_jReaderName, 0);
		_readerState[_i].pvUserData = NULL;
		_readerState[_i].dwCurrentState = _piCurrentState[_i];
		_readerState[_i].dwEventState = SCARD_STATE_UNAWARE;
		_readerState[_i].cbAtr = 0;

		printInDebug("SCardGetStatusChange %d reader: %s - current %d - event %d \n", _i, _readerState[_i].szReader, _readerState[_i].dwCurrentState, _readerState[_i].dwEventState);
	}

	env->ReleaseIntArrayElements(piCurrentState, (jint*)_piCurrentState, JNI_ABORT);

	printInDebug("SCardGetStatusChange %d elements ready to test\n", _dwNbElements);

	printInDebug("SCardGetStatusChange timeout: %d \n", lTimeout);

	_lResult = SCardGetStatusChange((SCARDCONTEXT)_contextId, (long)lTimeout, _readerState, _dwNbElements);

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("<-- SCardGetStatusChange Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
	}
	else
	{
		printInDebug("SCardGetStatusChange %ld new states retrieved\n", _dwNbElements);
		
		for(_i = 0; _i < _dwNbElements; _i++)
		{
			jint _eventStateTmp;

			_eventStateTmp = ((jint) _readerState[_i].dwEventState);

			printInDebug("SCardGetStatusChange nb bytes atr %d \n", _readerState[_i].cbAtr);
			
			printInDebug("SCardGetStatusChange event state %d: \n", _eventStateTmp);

			if(_eventStateTmp == SCARD_STATE_UNAWARE)
				printInDebug("\tSCARD_STATE_UNAWARE\n");
			if((_eventStateTmp & SCARD_STATE_IGNORE) == SCARD_STATE_IGNORE)
				printInDebug("\tSCARD_STATE_IGNORE\n");
			if((_eventStateTmp & SCARD_STATE_UNAVAILABLE) == SCARD_STATE_UNAVAILABLE)
				printInDebug("\tSCARD_STATE_UNAVAILABLE\n");
			if((_eventStateTmp & SCARD_STATE_EMPTY) == SCARD_STATE_EMPTY)
				printInDebug("\tSCARD_STATE_EMPTY\n");
			if((_eventStateTmp & SCARD_STATE_PRESENT) == SCARD_STATE_PRESENT)
				printInDebug("\tSCARD_STATE_PRESENT\n");
			if((_eventStateTmp & SCARD_STATE_ATRMATCH) == SCARD_STATE_ATRMATCH)
				printInDebug("\tSCARD_STATE_ATRMATCH\n");
			if((_eventStateTmp & SCARD_STATE_EXCLUSIVE) == SCARD_STATE_EXCLUSIVE)
				printInDebug("\tSCARD_STATE_EXCLUSIVE\n");
			if((_eventStateTmp & SCARD_STATE_INUSE) == SCARD_STATE_INUSE)
				printInDebug("\tSCARD_STATE_INUSE\n");
			if((_eventStateTmp & SCARD_STATE_MUTE) == SCARD_STATE_MUTE)
				printInDebug("\tSCARD_STATE_MUTE\n");
			
			env->SetIntArrayRegion(_objReturn, _i, 1, &_eventStateTmp);
		}
	}

	free(_readerState);

	printInDebug("<-- SCardGetStatusChange\n");

	return _objReturn;
}

JNIEXPORT jboolean JNICALL Java_fr_redbilled_security_pcscforjava_PCSCTerminals_SCardIsPlugAndPlaySupported(JNIEnv *env, jclass object, jlong lContextId)
{
	long				_lResult;
	SCARDCONTEXT		_contextId = (SCARDCONTEXT)lContextId;
	SCARD_READERSTATE	_readerState[1];
		
	printInDebug("--> SCardIsPlugAndPlaySupported context: %lld\n", lContextId);

	_readerState[0].szReader = "\\\\?PnP?\\Notification";
	_readerState[0].pvUserData = NULL;
	_readerState[0].dwCurrentState = SCARD_STATE_UNAWARE;
	_readerState[0].dwEventState = SCARD_STATE_UNAWARE;
	_readerState[0].cbAtr = 0;

	printInDebug("SCardIsPlugAndPlaySupported timeout: %d \n", 1);

	_lResult = SCardGetStatusChange((SCARDCONTEXT)_contextId, (long)1, _readerState, 1);

	printInDebug("<-- SCardIsPlugAndPlaySupported %d\n", _lResult);

	if((_lResult != SCARD_S_SUCCESS) || ((_readerState[0].dwEventState & SCARD_STATE_UNKNOWN) == SCARD_STATE_UNKNOWN))
		return false;
	else
		return true;	
}

JNIEXPORT jboolean JNICALL Java_fr_redbilled_security_pcscforjava_PCSCPnPThread_SCardPlugAndPlay(JNIEnv *env, jclass object, jlong lContextId, jlong lTimeout)
{

	long				_lResult;
	SCARDCONTEXT		_contextId = (SCARDCONTEXT)lContextId;
	SCARD_READERSTATE	_readerState[1];
	DWORD 				_dwReaders = 0;
	BOOL			_bIsWaitingConfirmation = false;
	
	
	printInDebug("--> SCardPlugAndPlay context: %lld\n", lContextId);

	if(g_lContextIdForPnPManagement != lContextId)
	{
		_readerState[0].szReader = "\\\\?PnP?\\Notification";
		_readerState[0].pvUserData = NULL;
		_readerState[0].dwCurrentState = SCARD_STATE_UNAWARE;
		_readerState[0].dwEventState = SCARD_STATE_UNAWARE;
		_readerState[0].cbAtr = 0;

		printInDebug("SCardPlugAndPlay timeout: %d \n", lTimeout);

		_lResult = SCardGetStatusChange((SCARDCONTEXT)_contextId, (long)lTimeout, _readerState, 1);
		
		if(_lResult == SCARD_E_TIMEOUT)
		{
			printInDebug("SCardPlugAndPlay timeout \n");
			printInDebug("SCardPlugAndPlay native plug and play is supported or not ...\n");
			_bIsWaitingConfirmation = true;
				
			#if(defined WIN32) || (defined WIN64)
				g_bPnP = true;
			#else
				g_bPnP = false;
			#endif
		}
		else if((_lResult != SCARD_S_SUCCESS) || ((_readerState[0].dwEventState & SCARD_STATE_UNKNOWN) == SCARD_STATE_UNKNOWN))
		{
			printInDebug("SCardPlugAndPlay native plug and play not supported result: %d & event %d \n", _lResult, _readerState[0].dwEventState);
			g_bPnP = false;
		}
		else
		{
			printInDebug("SCardPlugAndPlay native plug and play is supported \n");
			g_bPnP = true;
		}

		g_lContextIdForPnPManagement = (SCARDCONTEXT)lContextId;
		g_lEventOccured = _readerState[0].dwEventState;
	}

	if(g_bPnP)
	{
		_readerState[0].szReader = "\\\\?PnP?\\Notification";
		_readerState[0].pvUserData = NULL;
		_readerState[0].dwCurrentState = g_lEventOccured;
		_readerState[0].dwEventState = SCARD_STATE_UNAWARE;
		_readerState[0].cbAtr = 0;
		
		_lResult = SCardGetStatusChange((SCARDCONTEXT)_contextId, (long)lTimeout, _readerState, 1);

		if(_lResult == SCARD_E_TIMEOUT)
		{
			if(_bIsWaitingConfirmation)
			{
				g_bPnP = false;
				printInDebug("SCardPlugAndPlay  native plug and play not supported\n");
 			}
			printInDebug("<-- SCardPlugAndPlay SCARD_E_TIMEOUT\n");	
			return false;
		}
		else if(_lResult == SCARD_S_SUCCESS)
		{
			printInDebug("<-- SCardPlugAndPlay SCARD_S_SUCCESS\n");		
			g_lEventOccured = _readerState[0].dwEventState;
			return true;
		}
		else
		{
			if(_bIsWaitingConfirmation)
			{
				g_bPnP = false;
				printInDebug("SCardPlugAndPlay  native plug and play not supported\n");
 			}
			printInDebug("<-- SCardPlugAndPlay Exception: %ld\n", _lResult);
			launchException(env, _lResult);
			return false;
		}
	}
	else
	{
		_dwReaders = g_dwOldReaders;
		_lResult = SCARD_S_SUCCESS;

 		while((SCARD_S_SUCCESS == _lResult) && (_dwReaders == g_dwOldReaders))
		{
		 	_lResult = SCardListReaders((SCARDCONTEXT)_contextId, NULL, NULL, &_dwReaders);
		 	if (SCARD_E_NO_READERS_AVAILABLE == _lResult)
			{
				_lResult = SCARD_S_SUCCESS;
				_dwReaders = 0;
			}

			#if(defined WIN32) || (defined WIN64)
				Sleep(1);
			#else 
				sleep(1);
			#endif

			if(lTimeout != 0xffffffff)
				lTimeout--;
			
			if(lTimeout == 0)
				break;

			printInDebug("SCardPlugAndPlay %d %d\n", _dwReaders, g_dwOldReaders);	
 		}

		if((_lResult == SCARD_S_SUCCESS) && (lTimeout > 0))
		{
			g_dwOldReaders = _dwReaders;
			printInDebug("<-- SCardPlugAndPlay EVENT\n");
			return true;
		}
		else if(lTimeout == 0)
		{
			printInDebug("<-- SCardPlugAndPlay SCARD_E_TIMEOUT\n");	
			return false;
		}
		else
		{
			printInDebug("<-- SCardPlugAndPlay Exception: %ld\n", _lResult);
			launchException(env, _lResult);
			return false;
		}
	}
	
	return false;
}

JNIEXPORT jintArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardLocateCards(JNIEnv *env, jclass object, jlong lContextId, 
																						 jobjectArray psCardNames, jintArray piCurrentState, 
																						 jobjectArray psReaderNames)
{
	/*long _lResult;

	printInDebug("--> SCardLocateCards\n");

	_lResult = SCardLocateCards((long)lContextId, 

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("<-- SCardLocateCards Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
	}
	else
	{
		printInDebug("SCardLocateCards success\n");
	}

	printInDebug("<-- SCardLocateCards\n");*/

	return NULL;
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardBeginTransaction(JNIEnv *env, jclass object, jlong lCardHandle)
{
	long _lResult;

	printInDebug("--> SCardBeginTransaction\n");

	_lResult = SCardBeginTransaction((SCARDHANDLE)lCardHandle);

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("<-- SCardBeginTransaction Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
	}
	else
	{
		printInDebug("SCardBeginTransaction success\n");
	}
	
	printInDebug("<-- SCardBeginTransaction\n");
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardEndTransaction(JNIEnv *env, jclass object, jlong lCardHandle, 
																					   jint iDisposition)
{
	long _lResult;

	printInDebug("--> SCardEndTransaction\n");

	switch(iDisposition)
	{
		case SCARD_EJECT_CARD:
			printInDebug("SCardEndTransaction SCARD_EJECT_CARD\n");
			break;

		case SCARD_LEAVE_CARD:
			printInDebug("SCardEndTransaction SCARD_LEAVE_CARD\n");
			break;

		case SCARD_RESET_CARD:
			printInDebug("SCardEndTransaction SCARD_RESET_CARD\n");
			break;

		case SCARD_UNPOWER_CARD:
			printInDebug("SCardEndTransaction SCARD_UNPOWER_CARD\n");
			break;

		default:
			printInDebug("<-- SCardEndTransaction Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return;
	}

	_lResult = SCardEndTransaction((SCARDHANDLE)lCardHandle, iDisposition);

	if(_lResult != SCARD_S_SUCCESS)
	{
		printInDebug("<-- SCardEndTransaction Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
	}
	else
	{
		printInDebug("SCardEndTransaction success\n");
	}
	
	printInDebug("<-- SCardEndTransaction\n");
}


JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardControl(JNIEnv *env, jclass object, jlong lCardHandle, 
																					  jint iControlCode, jbyteArray pBCommand)
{
	SCARDHANDLE     _lCardHandle = (SCARDHANDLE)lCardHandle;
	long            _lResult;
	jbyte*         	_pBCommand = (env)->GetByteArrayElements(pBCommand, NULL);
	jint            _iLength = (env)->GetArrayLength(pBCommand);
	jbyte         	_pBReceptionBuffer[BUFFER_MAX_SZ];
	jint            _iReceptionLength = BUFFER_MAX_SZ;
	ULONG         	_ulReturnedLength = 0;
	jbyteArray     	_objReturn = NULL;

    printInDebug("--> SCardControl\n");

    printInDebug("SCardControl controlCode: %d\n", iControlCode);

	if(_iLength >= 2)
		printInDebug("SCardControl Length %d Command %#x %#x ... %#x %#x\n", _iLength, _pBCommand[0], _pBCommand[1], _pBCommand[_iLength - 2], _pBCommand[_iLength - 1]);
	else
		printInDebug("SCardControl Length %d\n", _iLength);

	_lResult = SCardControl((SCARDHANDLE)_lCardHandle, iControlCode, _pBCommand, _iLength, _pBReceptionBuffer, _iReceptionLength, &_ulReturnedLength);

    (env)->ReleaseByteArrayElements(pBCommand, _pBCommand, JNI_ABORT);

    if(_lResult != SCARD_S_SUCCESS)
    {
        printInDebug("SCardControl failed\n");
        printInDebug("<-- SCardControl Exception: %ld\n", _lResult);
        launchException(env, _lResult);
		return NULL;
    }
    else
    {
        printInDebug("SCardControl success\n");

        if(_ulReturnedLength > 0)
        {
            printInDebug("SCardControl Length %d Response %#x %#x ... %#x %#x\n", _ulReturnedLength, _pBReceptionBuffer[0], _pBReceptionBuffer[1], _pBReceptionBuffer[_ulReturnedLength - 2], _pBReceptionBuffer[_ulReturnedLength - 1]);
        
            _objReturn = (env)->NewByteArray(_ulReturnedLength);
            (env)->SetByteArrayRegion(_objReturn, 0, _ulReturnedLength, _pBReceptionBuffer);    
        }
        else
        {
			printInDebug("SCardControl response empty\n");
        
            /*printInDebug("SCardControl invalid reception length\n");
            printInDebug("<-- SCardControl Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;*/
        }
    }

	printInDebug("<-- SCardControl\n");

	return _objReturn;
}

JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardReconnect(JNIEnv *env, jclass object, jlong lCardHandle, jint iShareMode, jint iPreferredProtocols, jint iInitialization)
{
    long            _lResult;
    DWORD           _dwActiveProtocol;
    jbyteArray     	_objReturn = NULL;

    printInDebug("--> SCardReconnect\n");
	
    switch(iShareMode)
    {
        case SCARD_SHARE_SHARED:
            printInDebug("SCardReconnect SCARD_SHARE_SHARED\n");
            break;

        case SCARD_SHARE_EXCLUSIVE:
            printInDebug("SCardReconnect SCARD_SHARE_EXCLUSIVE\n");
            break;

        default:
            printInDebug("SCardReconnect incorrect share mode\n");
            printInDebug("<-- SCardReconnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
            launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
    }

    switch(iPreferredProtocols)
    {
        case SCARD_PROTOCOL_T0:
            printInDebug("SCardReconnect SCARD_PROTOCOL_T0\n");
            break;

        case SCARD_PROTOCOL_T1:
            printInDebug("SCardReconnect SCARD_PROTOCOL_T1\n");
            break;

        case SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1:
            printInDebug("SCardReconnect SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1\n");
            break;

        default:
            printInDebug("SCardReconnect incorrect preferred protocols\n");
            printInDebug("<-- SCardReconnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
            launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
    }

    switch(iInitialization)
    {
        case SCARD_LEAVE_CARD:
            printInDebug("SCardReconnect SCARD_LEAVE_CARD nothing to do\n");
            break;

        case SCARD_RESET_CARD:
            printInDebug("SCardReconnect SCARD_RESET_CARD => WARM RESET\n");
            break;

        case SCARD_UNPOWER_CARD:
            printInDebug("SCardReconnect SCARD_UNPOWER_CARD => COLD RESET\n");
            break;

        default:
            printInDebug("SCardReconnect incorrect initialization type\n");
            printInDebug("<-- SCardReconnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
            launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
    }

    _lResult = SCardReconnect((SCARDHANDLE)lCardHandle, iShareMode, iPreferredProtocols, iInitialization, &_dwActiveProtocol);

    if(_lResult != SCARD_S_SUCCESS)
    {
        printInDebug("SCardReconnect failed\n");
        printInDebug("<-- SCardReconnect Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
    }
    else
    {
        printInDebug("SCardReconnect success\n");

        switch(iPreferredProtocols)
        {
            case SCARD_PROTOCOL_T0:
                printInDebug("SCardReconnect active protocol: SCARD_PROTOCOL_T0\n");
                break;

            case SCARD_PROTOCOL_T1:
                printInDebug("SCardReconnect active protocol: SCARD_PROTOCOL_T1\n");
                break;

            default:
                printInDebug("SCardReconnect incorrect active protocol\n");
                printInDebug("<-- SCardReconnect Exception: %ld\n", SCARD_E_INVALID_VALUE);
				launchException(env, SCARD_E_INVALID_VALUE);
				return NULL;
        }

        #ifdef SCARD_ATTR_ATR_STRING
        
            // Get the new ATR of the card
            _objReturn = Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetAttrib(env, object, lCardHandle, SCARD_ATTR_ATR_STRING);
        
        #else
        
            jclass          _stringClass = env->FindClass("Ljava/lang/String;");
            jobjectArray    _psReaderNames;
            jbyteArray      _pBStatus = (jbyteArray)env->NewByteArray(2);
        
            if(_stringClass == NULL)
                return NULL;
        
            _psReaderNames = env->NewObjectArray(1, _stringClass, NULL);
            
            _objReturn = Java_fr_redbilled_security_pcscforjava_PCSC_SCardStatus(env, object, lCardHandle, _pBStatus, _psReaderNames);
        
        #endif
    }

    printInDebug("<-- SCardReconnect\n");

    return _objReturn;
}

JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetAttrib(JNIEnv *env, jclass object, jlong lCardHandle, 
																						jint iAttribute)
{
	long		_lResult;
	LPBYTE		_pBResponse = NULL;
	DWORD		_dwAttributeLen = 0;
	jbyteArray  _objReturn = NULL;
	
	printInDebug("--> SCardGetAttrib\n");

	switch(iAttribute)
	{
        #ifdef SCARD_ATTR_ATR_STRING    
            case SCARD_ATTR_ATR_STRING:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_ATR_STRING\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CHANNEL_ID    
            case SCARD_ATTR_CHANNEL_ID:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CHANNEL_ID\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CHARACTERISTICS    
            case SCARD_ATTR_CHARACTERISTICS:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CHARACTERISTICS\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CURRENT_BWT    
            case SCARD_ATTR_CURRENT_BWT:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_BWT\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CURRENT_CLK    
            case SCARD_ATTR_CURRENT_CLK:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_CLK\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CURRENT_CWT    
            case SCARD_ATTR_CURRENT_CWT:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_CWT\n");
                break;
        #endif

        #ifdef SCARD_ATTR_CURRENT_D    
            case SCARD_ATTR_CURRENT_D:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_D\n");
                break;
        #endif

		#ifdef SCARD_ATTR_CURRENT_EBC_ENCODING    	
		case SCARD_ATTR_CURRENT_EBC_ENCODING:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_EBC_ENCODING\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_F    
		case SCARD_ATTR_CURRENT_F:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_F\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_IFSC    
		case SCARD_ATTR_CURRENT_IFSC:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_IFSC\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_IFSD    
		case SCARD_ATTR_CURRENT_IFSD:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_IFSD\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_N    
		case SCARD_ATTR_CURRENT_N:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_N\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_PROTOCOL_TYPE    
		case SCARD_ATTR_CURRENT_PROTOCOL_TYPE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_PROTOCOL_TYPE\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_CURRENT_W    
		case SCARD_ATTR_CURRENT_W:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_CURRENT_W\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_DEFAULT_CLK    
		case SCARD_ATTR_DEFAULT_CLK:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEFAULT_CLK\n");
			break;
		#endif

		#ifdef SCARD_ATTR_DEFAULT_DATA_RATE    
		case SCARD_ATTR_DEFAULT_DATA_RATE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEFAULT_DATA_RATE\n");
			break;
		#endif

		#ifdef SCARD_ATTR_DEVICE_FRIENDLY_NAME    
		case SCARD_ATTR_DEVICE_FRIENDLY_NAME:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEVICE_FRIENDLY_NAME\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_DEVICE_IN_USE    
		case SCARD_ATTR_DEVICE_IN_USE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEVICE_IN_USE\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_DEVICE_SYSTEM_NAME    
		case SCARD_ATTR_DEVICE_SYSTEM_NAME:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEVICE_SYSTEM_NAME\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_DEVICE_UNIT    
		case SCARD_ATTR_DEVICE_UNIT:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_DEVICE_UNIT\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_ICC_INTERFACE_STATUS    
		case SCARD_ATTR_ICC_INTERFACE_STATUS:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_ICC_INTERFACE_STATUS\n");
			break;
		#endif
			
		#ifdef SCARD_ATTR_ICC_PRESENCE    
		case SCARD_ATTR_ICC_PRESENCE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_ICC_PRESENCE\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_ICC_TYPE_PER_ATR    
		case SCARD_ATTR_ICC_TYPE_PER_ATR:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_ICC_TYPE_PER_ATR\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_MAX_CLK    
		case SCARD_ATTR_MAX_CLK:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_MAX_CLK\n");
			break;
		#endif
		
		#ifdef SCARD_ATTR_MAX_DATA_RATE    
		case SCARD_ATTR_MAX_DATA_RATE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_MAX_DATA_RATE\n");
			break;
		#endif

		#ifdef SCARD_ATTR_MAX_IFSD    
		case SCARD_ATTR_MAX_IFSD:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_MAX_IFSD\n");
			break;
		#endif
		
        #ifdef SCARD_ATTR_POWER_MGMT_SUPPORT
            case SCARD_ATTR_POWER_MGMT_SUPPORT:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_POWER_MGMT_SUPPORT\n");
                break;
        #endif

		#if(!defined LINUX32) && (!defined LINUX64) && (!defined __APPLE__)
		case SCARD_ATTR_PROTOCOL_TYPES:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_PROTOCOL_TYPES\n");
			break;
		#endif

        #ifdef SCARD_ATTR_VENDOR_IFD_SERIAL_NO
            case SCARD_ATTR_VENDOR_IFD_SERIAL_NO:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_VENDOR_IFD_SERIAL_NO\n");
                break;
        #endif

        #ifdef SCARD_ATTR_VENDOR_IFD_TYPE
		case SCARD_ATTR_VENDOR_IFD_TYPE:
			printInDebug("SCardGetAttrib attribute: SCARD_ATTR_VENDOR_IFD_TYPE\n");
			break;
        #endif
		
        #ifdef SCARD_ATTR_VENDOR_IFD_VERSION
            case SCARD_ATTR_VENDOR_IFD_VERSION:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_VENDOR_IFD_VERSION\n");
                break;
        #endif
        
        #ifdef SCARD_ATTR_VENDOR_NAME
            case SCARD_ATTR_VENDOR_NAME:
                printInDebug("SCardGetAttrib attribute: SCARD_ATTR_VENDOR_NAME\n");
                break;
        #endif

		default:
			printInDebug("<-- SCardGetAttrib attribute not supported Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			return NULL;
	}

	_lResult = SCardGetAttrib((SCARDHANDLE)lCardHandle, iAttribute, NULL, &_dwAttributeLen);

	if(_lResult == SCARD_S_SUCCESS)
	{
			_pBResponse = (LPBYTE)malloc(_dwAttributeLen);
			_lResult = SCardGetAttrib((SCARDHANDLE)lCardHandle, iAttribute, _pBResponse, &_dwAttributeLen);
	}

	if(_lResult != SCARD_S_SUCCESS)
    {
        printInDebug("SCardGetAttrib failed\n");
        printInDebug("<-- SCardGetAttrib Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return NULL;
    }
    else
    {
		printInDebug("SCardGetAttrib success\n");
		
		if(_dwAttributeLen > 1)
			printInDebug("SCardGetAttrib length %d - %#x %#x ... %#x %#x\n", _dwAttributeLen, _pBResponse[0], _pBResponse[1], _pBResponse[_dwAttributeLen - 2], _pBResponse[_dwAttributeLen - 1]);
		else
			printInDebug("SCardGetAttrib length %d\n", _dwAttributeLen);

		_objReturn = (env)->NewByteArray(_dwAttributeLen);
		(env)->SetByteArrayRegion(_objReturn, 0, _dwAttributeLen, (jbyte*)_pBResponse);
	}

	printInDebug("<-- SCardGetAttrib\n");

	free(_pBResponse);
	
	return _objReturn;
}

JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardSetAttrib(JNIEnv *env, jclass object, jlong lCardHandle, 
																						jint iAttribute, jbyteArray pBAttributeCommand)
{
	long	_lResult;
	BYTE*  _pBCommand = (BYTE*)(env)->GetByteArrayElements(pBAttributeCommand, NULL);
	DWORD	_dwAttributeLength = (env)->GetArrayLength(pBAttributeCommand);

	printInDebug("--> SCardSetAttrib\n");

	switch(iAttribute)
	{
        #ifdef SCARD_ATTR_SUPRESS_T1_IFS_REQUEST
            case(SCARD_ATTR_SUPRESS_T1_IFS_REQUEST):
                break;
        #endif
            
		default:
			printInDebug("<-- SCardSetAttrib attribute not supported Exception: %ld\n", SCARD_E_INVALID_VALUE);
			launchException(env, SCARD_E_INVALID_VALUE);
			break;
	}

	if(_dwAttributeLength > 1)
		printInDebug("SCardSetAttrib length %d - %#x %#x ... %#x %#x\n", _dwAttributeLength, _pBCommand[0], _pBCommand[1], _pBCommand[_dwAttributeLength - 2], _pBCommand[_dwAttributeLength - 1]);
	else
		printInDebug("SCardSetAttrib length %d\n", _dwAttributeLength);

	_lResult = SCardSetAttrib((SCARDHANDLE)lCardHandle, iAttribute, _pBCommand, _dwAttributeLength);

	(env)->ReleaseByteArrayElements(pBAttributeCommand, (jbyte*)_pBCommand, JNI_ABORT);

	if(_lResult != SCARD_S_SUCCESS)
    {
        printInDebug("SCardSetAttrib failed\n");
        printInDebug("<-- SCardSetAttrib Exception: %ld\n", _lResult);
		launchException(env, _lResult);
		return;
    }
    else
    {
		printInDebug("SCardSetAttrib success\n");
	}

	printInDebug("<-- SCardSetAttrib\n");
}

#if(defined _DEBUG)
	FILE * _file;
#endif

void printInDebug(const char * format, ...)
{
	#if(defined _DEBUG)
		static unsigned char _ucFirstTime = 1;
		va_list	_arguments;
		time_t	_time;
		#if(defined WIN32) || (defined WIN64)
		char _hour[32];
		#else
			char * _hour;
		#endif
		int _idRead = 0, _idWrite = 0;
	   	
		if(_ucFirstTime)
		{
			// open the debug file
			#if(defined WIN32) || (defined WIN64)	
				fopen_s(&_file, "c:\\debugPCSC4JavaJNI.log", "a+");
			#else
				_file = fopen("/tmp/debugPCSC4JavaJNI.log", "a+");
			#endif
		}
		
		va_start(_arguments, format);
		time(&_time);
		#if(defined WIN32) || (defined WIN64)
		ctime_s(_hour, sizeof(_hour) ,&_time);
		#else
			_hour = ctime(&_time);
		#endif
		
 		while (_hour[_idRead] != '\0')
 		{
			if (_hour[_idRead] != '\n')
			{
				_hour[_idWrite] = _hour[_idRead];
				_idWrite++;
			}
			_idRead++;
		}
		_hour[_idWrite] = '\0';

		fputs(_hour, _file);
		fputs(" :: ", _file);
		vfprintf(_file, format, _arguments);

		_ucFirstTime = 0;
		
		// close the debug file
		//fclose(_file);		
	#endif
}

void launchException(JNIEnv * env, long lException)
{
	jclass		_pcscClass;
    jmethodID	_constructor;
    jthrowable	_pcscException;

    _pcscClass = env->FindClass(PCSC_EXCEPTION_CLASS);
    
	_constructor = env->GetMethodID(_pcscClass, "<init>", "(I)V");
    _pcscException = (jthrowable) env->NewObject(_pcscClass, _constructor, lException);
	env->Throw(_pcscException);
}


