/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Setting item class for Feedback settings.
*
*/



// INCLUDES
#include "CProfileFeedbackSettingItem.h"
#include "CProfileFeedbackSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingItem::CProfileFeedbackSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileFeedbackSettingItem::CProfileFeedbackSettingItem(
	TInt aIdentifier, TInt& aValue )
	:	CAknEnumeratedTextPopupSettingItem( aIdentifier, aValue )
	{
	iIdentifier = aIdentifier;
	}

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingItem::CompleteConstructionL()
	{
	CAknEnumeratedTextPopupSettingItem::CompleteConstructionL();
	CArrayPtr<CAknEnumeratedText>* array = EnumeratedTextArray();
	TInt count( array->Count() );
	HBufC* text = NULL;
	while( count-- )
		{
		text = array->At( count )->Value();
		TPtr modifiable( text->Des() );
		AknTextUtils::LanguageSpecificNumberConversion( modifiable );
		}
	}

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingItem::CreateSettingPageL
// -----------------------------------------------------------------------------
//
CAknSettingPage* CProfileFeedbackSettingItem::CreateSettingPageL()
	{
	return new( ELeave ) CProfileFeedbackSettingPage(
		SettingPageResourceId(),
		*QueryValue(),
		iIdentifier );
	}

// End of File
