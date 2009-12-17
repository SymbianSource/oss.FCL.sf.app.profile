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
* Description:  Settings handler for the settings outside of Profiles.
*
*/



// CLASS HEADER
#include "CProfilesGSTonesPluginSettingsHandler.h"

#include <DRMHelper.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::RemoveAutomatedContent
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::RemoveAutomatedContent(
        const TDesC& aOldTone )
    {
    iDrmHelper->RemoveAutomatedPassive( aOldTone ); // ignore return value
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::DrmConstructL
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::DrmConstructL()
    {
    iDrmHelper = CDRMHelper::NewL( *iCoeEnv );
    }

// -----------------------------------------------------------------------------
// CProfilesGSTonesPluginSettingsHandler::DrmDelete
// -----------------------------------------------------------------------------
//
void CProfilesGSTonesPluginSettingsHandler::DrmDelete()
    {
    delete iDrmHelper;
    iDrmHelper = NULL;
    }

// End Of File

