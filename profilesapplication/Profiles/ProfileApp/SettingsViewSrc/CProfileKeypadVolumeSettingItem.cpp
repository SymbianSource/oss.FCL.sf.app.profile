/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Setting item class for keypad volume setting.
*
*/



// INCLUDES
#include <AknUtils.h>

#include "CProfileKeypadVolumeSettingItem.h"
#include "CProfileKeypadVolumeSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingItem::CProfileKeypadVolumeSettingItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileKeypadVolumeSettingItem::CProfileKeypadVolumeSettingItem(
	TInt aIdentifier,
	TInt& aValue,
	CAknKeySoundSystem* aSoundSystem )
	:	CAknEnumeratedTextPopupSettingItem( aIdentifier, aValue ),
		iSoundSystem( aSoundSystem )
	{
	}

// -----------------------------------------------------------------------------
// CProfileKeypadVolumeSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfileKeypadVolumeSettingItem::CompleteConstructionL()
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
// CProfileKeypadVolumeSettingItem::CreateSettingPageL
// -----------------------------------------------------------------------------
//
CAknSettingPage* CProfileKeypadVolumeSettingItem::CreateSettingPageL()
	{
	return new( ELeave ) CProfileKeypadVolumeSettingPage(
		SettingPageResourceId(),
		*QueryValue(),
		iSoundSystem );
	}

// End of File
