/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Wraps the information about variable features of GS Tones
*                View.
*
*/



// CLASS HEADER
#include "CProfilesGSTonesViewFeatures.h"

// INTERNAL INCLUDES
#include "ProfilesDebug.h"

// SYSTEM INCLUDES
#include <centralrepository.h>
#include <ProfileEngineDomainCRKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// Destructor
CProfilesGSTonesViewFeatures::~CProfilesGSTonesViewFeatures()
    {
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewFeatures::ConstructL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesViewFeatures::ConstructL()
    {
    PRODEBUG( "CProfilesGSTonesViewFeatures::ConstructL begin" );
    CRepository* profilesLVSession = CRepository::NewLC( KCRUidProfilesLV );
    TInt error( profilesLVSession->Get( KProEngTonesViewLVFlags,
                                        iLocalFeatures ) );
    // If the key isn't found, all the local features are disabled:
    if( error && ( error != KErrNotFound ) )
        {
        User::Leave( error );
        }

    CleanupStack::PopAndDestroy( profilesLVSession );
    PRODEBUG( "CProfilesGSTonesViewFeatures::ConstructL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewFeatures::NewL
// -----------------------------------------------------------------------------
//
CProfilesGSTonesViewFeatures*
        CProfilesGSTonesViewFeatures::NewL()
    {
    CProfilesGSTonesViewFeatures* self = new ( ELeave )
        CProfilesGSTonesViewFeatures();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewFeatures::IsLocalFeatureSupported
// -----------------------------------------------------------------------------
//
TBool CProfilesGSTonesViewFeatures::IsLocalFeatureSupported(
        TUint32 aFeatureId ) const
    {
    return ( iLocalFeatures & aFeatureId );
    }

// End Of File



