/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ECOM prxy table for plugin.
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// Internal includes
#include "ProfilesGSTonesPlugin.h"
#include "ProfilesUID.h"

// Constants
const TImplementationProxy KProfilesGSTonesPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( IMPLEMENTATION_UID_PROFILES_GS_TONES_PLUGIN,
                                CProfilesGSTonesPlugin::NewL )
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
	{
	aTableCount = sizeof( KProfilesGSTonesPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
	return KProfilesGSTonesPluginImplementationTable;
	}


// End of File
