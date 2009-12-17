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
* Description:  Setting page class for "Alert for" setting.
*
*/



// INCLUDES
#include "CProfileAlertForSettingPage.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileAlertForSettingPage::CProfileAlertForSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileAlertForSettingPage::CProfileAlertForSettingPage(
	TInt aResourceID,
	CSelectionItemList* aItemArray )
	:	CAknCheckBoxSettingPage( aResourceID, aItemArray )
	{
	}

// -----------------------------------------------------------------------------
// CProfileAlertForSettingPage::UpdateCba
// -----------------------------------------------------------------------------
//
void CProfileAlertForSettingPage::UpdateCba()
	{
	// Cache the pointer to button group container
	CEikButtonGroupContainer* cba = Cba();
	// Left softkey should be visible if there are items selected,
	// i.e. the selection index array has items.
	TBool visible( ListBoxControl()->View()->SelectionIndexes()->Count() > 0 );
	// Resolve left softkey command id
	TInt leftId( cba->ButtonGroup()->CommandId( 0 ) );
	// Check if left softkey visibility has changed
	if( visible != cba->IsCommandVisible( leftId ) )
		{
		// Left softkey visibility has changed, update it
		cba->MakeCommandVisible( leftId, visible );
		}
		cba->DrawDeferred();
	}

//  End of File
