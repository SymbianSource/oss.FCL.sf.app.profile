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
* Description:  Data structure class for profile tones view settings.
*
*/



// CLASS HEADER
#include "CProfilesGSTonesViewSettings.h"

// INTERNAL INCLUDES
#include "ProfilesGSTonesPlugin.hrh"
#include "ProfilesDebug.h"
#include "CProfilesGSTonesViewFeatures.h"

// INCLUDES FROM PROFILES SETTINGS VIEW

// SYSTEM INCLUDES
#include <centralrepository.h>
#include <HWRMDomainCRKeys.h>
#include <ProfileEngineDomainCRKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// Destructor
CProfilesGSTonesViewSettings::~CProfilesGSTonesViewSettings()
    {
    delete iHwRmSession;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewSettings::ConstructL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesViewSettings::ConstructL()
    {
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL begin" );
    if( iFeatures.IsLocalFeatureSupported( KTonesViewFeatureIdHac ) )
        {
        iHwRmSession = CRepository::NewL( KCRUidHWSettings );
        User::LeaveIfError( iHwRmSession->Get( KHWSettingsHearingAidMode, iHac ) );
        }
    PRODEBUG( "CProfilesGSTonesPluginSettingHandler::ConstructL end" );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewSettings::NewL
// -----------------------------------------------------------------------------
//
CProfilesGSTonesViewSettings* CProfilesGSTonesViewSettings::NewL(
        CProfilesGSTonesViewFeatures& aFeatures )
       
    {
    CProfilesGSTonesViewSettings* self = new ( ELeave )
        CProfilesGSTonesViewSettings( aFeatures );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesViewSettings::StoreIfChangedL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesViewSettings::StoreIfChangedL( TInt aSettingId ) const
    {
    if( ( aSettingId == EProfileGSTonesPluginSettingHacModeId ) &&
        iFeatures.IsLocalFeatureSupported( KTonesViewFeatureIdHac ) )
        {
        TInt currentValue( 0 );
        User::LeaveIfError( iHwRmSession->Get( KHWSettingsHearingAidMode,
                                             currentValue ) );
        if( currentValue != iHac )
            {
            User::LeaveIfError( iHwRmSession->Set( KHWSettingsHearingAidMode,
                                                 iHac ) );
            currentValue = iHac;
            }
        }
    }

// End Of File

