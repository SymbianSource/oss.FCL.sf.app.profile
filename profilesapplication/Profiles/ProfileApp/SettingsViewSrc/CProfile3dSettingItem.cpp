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
* Description:  Setting item class for 3D settings.
*
*/



// INCLUDES
#include "CProfile3dSettingItem.h"
#include "CProfile3dSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfile3dSettingItem::CProfile3dSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfile3dSettingItem::CProfile3dSettingItem(
	TInt aIdentifier,
	TInt& aValue,
	const TDesC& aRingingTone,
	const TInt& aRingingType,
	const TBool& aVibratingAlert,
	const TInt& aVolume,
	const TInt& a3DEffectOrEcho )
	:	CAknEnumeratedTextPopupSettingItem( aIdentifier, aValue ),
		iRingingTone( aRingingTone ),
		iRingingType( aRingingType ),
		iVibratingAlert( aVibratingAlert ),
		iVolume( aVolume ),
		i3DEffectOrEcho( a3DEffectOrEcho )
	{
	iIdentifier = aIdentifier;
	}

// -----------------------------------------------------------------------------
// CProfile3dSettingItem::CompleteConstructionL
// -----------------------------------------------------------------------------
//
void CProfile3dSettingItem::CompleteConstructionL()
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
// CProfile3dSettingItem::CreateSettingPageL
// -----------------------------------------------------------------------------
//
CAknSettingPage* CProfile3dSettingItem::CreateSettingPageL()
	{
	return new( ELeave ) CProfile3dSettingPage(
		SettingPageResourceId(),
		*QueryValue(),
		iRingingTone,
		iRingingType,
		iVibratingAlert,
		iVolume,
		i3DEffectOrEcho,
		iIdentifier );
	}

// End of File
