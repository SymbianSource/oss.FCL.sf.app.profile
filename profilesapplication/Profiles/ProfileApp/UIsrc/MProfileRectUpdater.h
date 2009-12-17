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
* Description:  Interface for getting client rect area.
*
*/



#ifndef MPROFILERECTUPDATER_H
#define MPROFILERECTUPDATER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for updating the active area.
*
*  @lib ProfileApp.app
*  @since 2.8
*/
class MProfileRectUpdater
    {

    public: // New functions

        /**
        * Updates the client drawing area.
        * @since 2.8
        */
		virtual void UpdateClientRect() = 0;

    };

#endif      // MProfileRectUpdater_H

// End of File
