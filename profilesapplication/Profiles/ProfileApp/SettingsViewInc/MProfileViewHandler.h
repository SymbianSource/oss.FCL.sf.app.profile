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
* Description:  Interface for getting client rect area and getting reference to
*                the view in use.
*
*/



#ifndef MPROFILEVIEWHANDLER_H
#define MPROFILEVIEWHANDLER_H

//  INCLUDES
#include <e32std.h>

//	FORWARD DECLARATIONS
class CAknView;

// CLASS DECLARATION

/**
*  Interface for updating the active area and getting reference to the view in
*  use.
*
*  @lib ProfileSettingsView.dll
*  @since 3.1
*/
class MProfileViewHandler
    {

    public: // New functions

        /**
        * Updates the client drawing area.
        * @since 3.1
        */
		virtual void UpdateClientRect() = 0;

        /**
        * Returns the view instance in use.
        * @since 3.1
        */
		virtual CAknView& View() = 0;

    };

#endif      // MProfileViewHandler_H

// End of File
