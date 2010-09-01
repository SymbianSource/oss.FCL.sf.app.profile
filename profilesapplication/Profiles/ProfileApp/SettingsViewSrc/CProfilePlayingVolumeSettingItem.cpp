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
* Description:  Setting item class for ringing volume setting.
*
*/



// INCLUDES
#include "CProfilePlayingVolumeSettingItem.h"
#include "CProfilePlayingVolumeSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingItem::CProfilePlayingVolumeSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfilePlayingVolumeSettingItem::CProfilePlayingVolumeSettingItem(
	TInt aIdentifier,
	TInt& aRingingVolume,
	const TDesC& aRingingTone,
	const TInt& aRingingType,
	const TBool& aVibratingAlert,
	const TBool aDisplayQuery )
	:	CAknVolumeSettingItem( aIdentifier, aRingingVolume ),
		iRingingTone( aRingingTone ),
		iRingingType( aRingingType ),
		iVibratingAlert( aVibratingAlert ),
		iDisplayQuery( aDisplayQuery )
	{
	}

// -----------------------------------------------------------------------------
// CProfilePlayingVolumeSettingItem::EditItemL
// -----------------------------------------------------------------------------
//
void CProfilePlayingVolumeSettingItem::EditItemL( TBool /* aCalledFromMenu */ )
	{
	CProfilePlayingVolumeSettingPage* dlg = new( ELeave ) CProfilePlayingVolumeSettingPage(
		SettingPageResourceId(),
		InternalVolumeValue(),
		iRingingTone,
		iRingingType,
		iVibratingAlert,
		iDisplayQuery );
	SetSettingPage( dlg );
	SettingPage()->SetSettingPageObserver( this );
    SettingPage()->ExecuteLD();
	SetSettingPage( NULL );
	}

// End of File
