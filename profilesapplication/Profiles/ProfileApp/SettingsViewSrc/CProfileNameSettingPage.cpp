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
* Description:  Setting page class for profile name setting.
*
*/



// INCLUDES
#include    "CProfileNameSettingPage.h"

#include    <aknnotewrappers.h>
#include    <mprofilesnamesarray.h>
#include    <ProfileSettingsView.rsg>
#include	<StringLoader.h>

#include    "CProfileEngineHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileNameSettingPage::CProfileNameSettingPage
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProfileNameSettingPage::CProfileNameSettingPage(
    TInt aResourceID,
    TDes& aText,
    TInt aTextSettingPageFlags,
    CProfileEngineHandler& aEngineHandler )
    :   CAknTextSettingPage( aResourceID, aText, aTextSettingPageFlags ),
        iEngineHandler( aEngineHandler ),
        iOriginalText( aText )
    {
    }

// -----------------------------------------------------------------------------
// CProfileNameSettingPage::OkToExitL
// -----------------------------------------------------------------------------
//
TBool CProfileNameSettingPage::OkToExitL( TBool aAccept )
    {
    if( aAccept )
        {
        CEikEdwin* edwin = TextControl();
        TBuf<KProfileMaxProfileNameLength> editedName;
        edwin->GetText( editedName );

        if( editedName.Compare( iOriginalText ) == 0 )
            {
            return ETrue;
            }
        if( iEngineHandler.IdArray()->FindByName( editedName ) != KErrNotFound )
            {
            // A profile was found that has the same name which user just entered
            // and it isn't the currently modified profile

            // Show error note
            HBufC* errorText = StringLoader::LoadLC( R_PROFILE_TEXT_RENAME_ERROR );
            CAknErrorNote* note = new( ELeave ) CAknErrorNote( ETrue );
            note->ExecuteLD( *errorText );
            CleanupStack::PopAndDestroy( errorText );

            // Set original text to editor
            edwin->SetTextL( &iOriginalText );

            // Don't let the setting page exit
            return EFalse;
            }
        }
    return ETrue;
    }

// End of File
