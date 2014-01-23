/********************************************************
* Copyright (c) 2011, REDBILLED All Rights Reserved
*
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF REDBILLED
* The copyright notice above does not evidence any actual
* or intended publication of such source code.
* =======================================================
* JNI for the using of PCSC
* =======================================================
* File : PCSC.h
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

/* Header for class fr_redbilled_security_pcscforjava_PCSC */

#include "PCSC_Defines.h"

#ifndef _Included_fr_redbilled_security_pcscforjava_PCSC
#define _Included_fr_redbilled_security_pcscforjava_PCSC
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardEstablishContext
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardEstablishContext
  (JNIEnv *, jclass, jint);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardReleaseContext
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardReleaseContext
 (JNIEnv * env, jclass object, jlong lContextId);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardCancel
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardCancel
 (JNIEnv * env, jclass object, jlong lContextId);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardIsValidContext
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardIsValidContext
 (JNIEnv * env, jclass object, jlong lContextId);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardListReaders
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardListReaders
  (JNIEnv *, jclass, jlong);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardConnect
 * Signature: (JLjava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardConnect
  (JNIEnv *, jclass, jlong, jstring, jint, jint);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardTransmit
 * Signature: (JI[BII)[B
 */
JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardTransmit
  (JNIEnv *, jclass, jlong, jint, jbyteArray, jint, jint);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardStatus
 * Signature: (J[B[Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardStatus
  (JNIEnv *, jclass, jlong, jbyteArray, jobjectArray);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardDisconnect
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardDisconnect
  (JNIEnv *, jclass, jlong, jint);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardGetStatusChange
 * Signature: (JJ[I[Ljava/lang/String;)[I
 */
JNIEXPORT jintArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetStatusChange
  (JNIEnv *, jclass, jlong, jlong, jintArray, jobjectArray);

JNIEXPORT jboolean JNICALL Java_fr_redbilled_security_pcscforjava_PCSCPnPThread_SCardPlugAndPlay(JNIEnv *env, jclass object, jlong lContextId, jlong lTimeout);

JNIEXPORT jboolean JNICALL Java_fr_redbilled_security_pcscforjava_PCSCTerminals_SCardIsPlugAndPlaySupported(JNIEnv *env, jclass object, jlong lContextId);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardGetStatusChange
 * Signature: (JJ[I[Ljava/lang/String;)[I
 */
/*JNIEXPORT jintArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardLocateCards
  (JNIEnv *, jclass, jlong, jlong, jintArray, jobjectArray);
SCardLocateCardsByATR
SCardGetCardTypeProviderName
SCardGetProviderId
SCardGetTransmitCount
SCardListCards
SCardListInterfaces
SCardListReaderGroups
SCardSetCardTypeProviderName

SCardAccessStartedEvent
SCardAddReaderToGroup
SCardForgetCardType
SCardForgetReader
SCardForgetReaderGroup
SCardIntroduceCardType
SCardIntroduceReader
SCardIntroduceReaderGroup
SCardReadCache
SCardReleaseStartedEvent
SCardRemoveReaderFromGroup
SCardWriteCache
SCardUIDlgSelectCard
  */

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardBeginTransaction
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardBeginTransaction
  (JNIEnv *, jclass, jlong);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardEndTransaction
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardEndTransaction
  (JNIEnv *, jclass, jlong, jint);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardControl
 * Signature: (JI[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardControl
  (JNIEnv *, jclass, jlong, jint, jbyteArray);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardReconnect
 * Signature: (JIII)[B
 */
JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardReconnect
  (JNIEnv *env, jclass object, jlong lCardHandle, jint iShareMode, jint iPreferredProtocols, jint iInitialization);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardGetAttrib
 * Signature: (JI)[B
 */
JNIEXPORT jbyteArray JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardGetAttrib
  (JNIEnv *env, jclass object, jlong lCardHandle, jint iAttribute);

/*
 * Class:     fr_redbilled_security_pcscforjava_PCSC
 * Method:    SCardSetAttrib
 * Signature: (JI[B)V
 */
JNIEXPORT void JNICALL Java_fr_redbilled_security_pcscforjava_PCSC_SCardSetAttrib
  (JNIEnv *env, jclass object, jlong lCardHandle, jint iAttribute, jbyteArray pBAttributeCommand);

#ifdef __cplusplus
}
#endif
#endif
