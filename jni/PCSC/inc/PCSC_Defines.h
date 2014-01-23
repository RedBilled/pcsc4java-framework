/********************************************************
* Copyright (c) 2011, REDBILLED All Rights Reserved
*
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF REDBILLED
* The copyright notice above does not evidence any actual
* or intended publication of such source code.
* =======================================================
* JNI for the using of PCSC
* =======================================================
* File : PCSC_Defines.h
*
*
* UPDATE HISTORY
*--------------------------------------------------------
*Version | Author     | Date     |
*--------------------------------------------------------
*  1.0	   ML	       22/06/2011
**********************************************************/

#ifndef __PCSC_Defines__H__
#define __PCSC_Defines__H__

#if(defined WIN32) || (defined WIN64)

	#pragma once

	// Modifiez les d�finitions suivantes si vous devez cibler une plate-forme avant celles sp�cifi�es ci-dessous.
	// Reportez-vous � MSDN pour obtenir les derni�res informations sur les valeurs correspondantes pour les diff�rentes plates-formes.
	#ifndef WINVER				// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.
	#define WINVER 0x0501		// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
	#endif

	#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.                   
	#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
	#endif						

	#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�98 ou version ult�rieure.
	#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�Me ou version ult�rieure.
	#endif

	#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Internet Explorer�6.0 ou version ult�rieure.
	#define _WIN32_IE 0x0600	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions d'Internet Explorer.
	#endif

	#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s
	// Fichiers d'en-t�te Windows�:
	#include <windows.h>

	#include <winscard.h>
	
	#include "jni.h"

#elif(defined LINUX32) || (defined LINUX64)

	#include <unistd.h>
	#include <string.h>

	#include <wintypes.h>
	#include <winscard.h>
	#include <ifdhandler.h>
	#include <pcsclite.h>
	#include <reader.h>
	
	#include "../linux/jni.h"

	#define CHAR	char

#elif(defined __APPLE__)

    #include <unistd.h>
    #include <string.h>

    #include <PCSC/wintypes.h>
    #include <PCSC/winscard.h>
    #include <PCSC/mscdefines.h>
    #include <PCSC/pcsclite.h>
    #include <PCSC/musclecard.h>

    #include "../mac_os_x/jni.h"

    #define CHAR	char
	
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_MAX_SZ				1024
#define NB_MAX_READERS_SUPPORTED	255
#define READER_NAME_SZ				200
#define ATR_LENGTH					32
#define ATR_BUFFER_MAX_SZ			(ATR_LENGTH * 4)	
#define PCSC_EXCEPTION_CLASS		"fr/redbilled/security/pcscforjava/PCSCException"

BOOL			g_bPnP = true;
SCARDCONTEXT	g_lContextIdForPnPManagement = 0;
long			g_lEventOccured = 0;

void printInDebug(const char * format, ...);
void launchException(JNIEnv * env, long lException);

#endif
