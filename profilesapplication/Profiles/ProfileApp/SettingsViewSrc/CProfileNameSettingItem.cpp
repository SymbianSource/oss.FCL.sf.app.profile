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
* Description:  Setting item class for profile name setting.
*
*/



// INCLUDES
#include "CProfileNameSettingItem.h"
#include "CProfileNameSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileNameSettingItem::CProfileNameSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileNameSettingItem::CProfileNameSettingItem(
	TInt aIdentifier,
	TDes& aText,
	CProfileEngineHandler& aEngineHandler )
	:	CAknTextSettingItem( aIdentifier, aText ),
		iEngineHandler( aEngineHandler )
	{
	}

// -----------------------------------------------------------------------------
// CProfileNameSettingItem::EditItemL
// We do not need to know if the setting page has been launched
// with selection key or from Options menu.
// -----------------------------------------------------------------------------
//
void CProfileNameSettingItem::EditItemL( TBool /* aCalledFromMenu */ )
	{
	CProfileNameSettingPage* dlg = new( ELeave ) CProfileNameSettingPage(
		SettingPageResourceId(),
		InternalTextPtr(),
		SettingPageFlags(),
        iEngineHandler );
	SetSettingPage( dlg );
	dlg->SetSettingPageObserver( this );
	dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
	SetSettingPage( NULL ); // it is deleted now
	}

// End of File
