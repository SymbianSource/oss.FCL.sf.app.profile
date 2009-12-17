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
* Description:  Setting page class for Feedback settings.
*
*/



// INCLUDES
#include <ProfileSettingsView.rsg>
#include <StringLoader.h>	// For StringLoader
#include <aknnotewrappers.h> // For CAknInformationNote

// for power save mode handling
#include <psmsettings.h>
#include <psmsrvdomaincrkeys.h>
#include <centralrepository.h>

#include "CProfileFeedbackSettingPage.h"
#include "ProfileSettingsView.hrh"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingPage::CProfileFeedbackSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileFeedbackSettingPage::CProfileFeedbackSettingPage(
    TInt aResourceID,
    MAknQueryValue& aQueryValue,
    TInt aIdentifier )
    :   CAknPopupSettingPage( aResourceID, aQueryValue ),
        iIdentifier( aIdentifier )
    {
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingPage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingPage::ConstructL()
    {
    CAknPopupSettingPage::ConstructL();
    }

// Destructor.
CProfileFeedbackSettingPage::~CProfileFeedbackSettingPage()
    {
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingPage::OkToExitL
// -----------------------------------------------------------------------------
//
TBool CProfileFeedbackSettingPage::OkToExitL( TBool aAccept )
    {
    if( aAccept )
        {
        TInt psm = EPsmsrvModeNormal;
        CRepository* cenrep = CRepository::NewLC( KCRUidPowerSaveMode );
        cenrep->Get( KPsmCurrentMode, psm );
        CleanupStack::PopAndDestroy( cenrep );
        if( psm == EPsmsrvModePowerSave )
            {
            HBufC* buf = StringLoader::LoadLC( R_PROFILE_TEXT_PSM_INFONOTE );
            CAknInformationNote* queryDialog = new (ELeave) CAknInformationNote( EFalse );
         	queryDialog->ExecuteLD( *buf );
            CleanupStack::PopAndDestroy( buf );
            }
        }
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CProfileFeedbackSettingPage::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CProfileFeedbackSettingPage::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    return CAknPopupSettingPage::OfferKeyEventL( aKeyEvent, aType );
    }


// -----------------------------------------------------------------------------
// CProfileFeedbackSettingPage::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingPage::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    CAknPopupSettingPage::HandlePointerEventL( aPointerEvent );
    }


// End of File
