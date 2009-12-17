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
* Description:  Interface to the actual storage used for saving the data.
*
*/



#ifndef MPROFILESGSTONESPLUGINITEMDATAINTERFACE_H
#define MPROFILESGSTONESPLUGINITEMDATAINTERFACE_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface to the actual storage used for saving the data.
*
*  @lib ProfilesGSTonesPlugin.dll
*  @since 3.1
*/
class MProfilesGSTonesPluginItemDataInterface
    {

    public: // New functions

        /**
        * Returns the data currently saved in the storage.
        * @return the data currently saved in the storage.
        */
		virtual const TDesC& ItemData() = 0;

        /**
        * Sets the data in the storage.
        * @param aDataBuf contains the data to be saved.
        */
		virtual void SetItemDataL( const TDesC& aDataBuf ) = 0;

    };

#endif      // MPROFILESGSTONESPLUGINITEMDATAINTERFACE_H

// End of File


