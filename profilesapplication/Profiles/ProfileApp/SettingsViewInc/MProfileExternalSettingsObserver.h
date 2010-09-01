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
* Description:  Interface for observing changes in the settings outside
*                Profiles itself.
*
*/



#ifndef MPROFILEEXTERNALSETTINGSOBSERVER_H
#define MPROFILEEXTERNALSETTINGSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for observing changes in the settings outside Profiles itself.
*
*  @lib ProfileSettingsView.dll
*  @since 3.1
*/
class MProfileExternalSettingsObserver
    {

    public: // New functions

        /**
        * Notifies that the external settings have changed somehow.
        * @since 3.1
        */
		virtual void HandleExternalSettingsChangedL() = 0;

    };

#endif      // MPROFILEEXTERNALSETTINGSOBSERVER_H

// End of File


