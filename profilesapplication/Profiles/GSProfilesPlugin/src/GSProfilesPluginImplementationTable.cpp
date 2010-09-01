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
* Description:  ECOM proxy table for GSProfilesPlugin.
*
*/


// User includes
#include "GSProfilesPlugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>


// Constants
const TImplementationProxy KGSProfilesPluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x10283387, CGSProfilesPlugin::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( 0x10283388, CPSMProfilesPlugin::NewL )
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
    RDebug::Print( _L( "[GSLog] ImplementationGroupProxy()" ) );
    aTableCount = sizeof( KGSProfilesPluginImplementationTable )
        / sizeof( TImplementationProxy );
    return KGSProfilesPluginImplementationTable;
    }


// End of File
