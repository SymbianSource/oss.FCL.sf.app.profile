/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A helper interface to be used by S60 applications for
*                setting ringing and alert tones for different profiles.
*                Registers DRM protected tones as automated content
*                and removes old content from automated content list.
*                Shows a content activation query if necessary.
*
*/



// INCLUDE FILES
#include    "CProfileToneHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileToneHandler::DrmConstructL
// 2nd phase construct for DRM objects. Empty implementation.
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::DrmConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::ReleaseDrm
// Destructor for DRM objects. Empty implementation.
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::ReleaseDrm()
    {
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CheckProtectedFileL
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::CheckProtectedFileL( const TDesC& /*aFileName*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::CanSetAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::CanSetAutomated( const TDesC& /*aFileName*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::AskAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::AskAutomated( const TDesC& /*aFileName*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::SetAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
TInt CProfileToneHandler::SetAutomated( TProfileTones /* aToneSetting */,
                                        const TDesC& /*aFileName*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::RemoveAutomated
// Empty implementation.
// -----------------------------------------------------------------------------
//
void CProfileToneHandler::RemoveAutomated( const TDesC& /*aFileName*/ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileToneHandler::IsProtected
// Empty implementation.
// -----------------------------------------------------------------------------
//
TBool CProfileToneHandler::IsProtected( const TDesC& /*aFileName*/ ) const
    {
    return EFalse;
    }

//  End of File
