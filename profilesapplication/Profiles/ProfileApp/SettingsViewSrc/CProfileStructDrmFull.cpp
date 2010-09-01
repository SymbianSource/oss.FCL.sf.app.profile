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
* Description:  Data structure class for profile settings.
*
*/



// CLASS HEADER
#include "CProfileStruct.h"

// EXTERNAL INCLUDES
#include <MProfileExtended.h>
#include <MProfileTones.h>
#include <MProfileExtraTones.h>
#include <DRMHelper.h>
#include <coemain.h>

// ============================ LOCAL FUNCTIONS ================================

void DoCompare( const TDesC& aOldFile, const TDesC& aNewFile, CDRMHelper& aDRMHelper )
    {
    if( aOldFile.CompareF( aNewFile ) != 0 )
        {
        // The file has been changed.
        // Remove the old file from the automated content list.
        // Ignore any error value. Also non-drm files are tried to be removed 
        // so the error might be KErrNotFound.
        //
        // Note:
        //   RemoveAutomatedPassive may leave even if it is not an L-method!
        TRAP_IGNORE( aDRMHelper.RemoveAutomatedPassive( aOldFile ) ); // CSI: 59 # See the note above.
        }
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileStruct::RemoveAutomatedContentL
// -----------------------------------------------------------------------------
//
void CProfileStruct::RemoveAutomatedContentL( MProfileExtended* aProfile ) const
    {
    CDRMHelper* drmHelper = CDRMHelper::NewLC( *CCoeEnv::Static() );
    const MProfileTones& tones = aProfile->ProfileTones();
    const MProfileExtraTones& extra = aProfile->ProfileExtraTones();
    DoCompare( tones.RingingTone1(), *iRingingTone, *drmHelper );
    DoCompare( tones.RingingTone2(), *iRingingToneLine2, *drmHelper );
    DoCompare( extra.VideoCallRingingTone(), *iVideoCallTone, *drmHelper );
    DoCompare( tones.MessageAlertTone(), *iMessageAlertTone, *drmHelper );
    DoCompare( extra.EmailAlertTone(), *iEmailAlertTone, *drmHelper );
    CleanupStack::PopAndDestroy( drmHelper );
    }

// End of File
