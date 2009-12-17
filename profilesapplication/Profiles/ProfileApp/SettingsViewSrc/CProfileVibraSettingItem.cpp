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
* Description:  Setting item class for Vibra settings.
*
*/



// INCLUDES
#include "CProfileVibraSettingItem.h"
#include "CProfileVibraSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileVibraSettingItem::CProfileVibraSettingItem
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileVibraSettingItem::CProfileVibraSettingItem(
	TInt aIdentifier, TBool& aValue )
	:	CAknBinaryPopupSettingItem( aIdentifier, aValue )
	{
	}


// -----------------------------------------------------------------------------
// CProfileVibraSettingItem::CreateSettingPageL
// -----------------------------------------------------------------------------
//
CAknSettingPage* CProfileVibraSettingItem::CreateSettingPageL()
	{
	return new( ELeave ) CProfileVibraSettingPage(
		SettingPageResourceId(),
		*QueryValue() );
	}

// End of File
