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
* Description:  The document class for Profiles application.
*
*/



#ifndef CPROFILEDOCUMENT_H
#define CPROFILEDOCUMENT_H

// INCLUDES
#include <AknDoc.h>

// FORWARD DECLARATIONS
class CEikAppUi;
class CProfileEngineHandler;
class CProfileIndexHandler;

// CLASS DECLARATION
/**
*  The document class for Profiles application.
*  CProfileDocument owns Engine handler and Index handler.
*  An instance of both are needed during application lifetime.
*  CProfileDocument initializes the Feature Manager.
*
*  @lib ProfileApp.app
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileDocument ) : public CAknDocument
    {
    public:		// Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileDocument* NewL( CEikApplication& aApp );

        /**
        * Destructor.
        */
        ~CProfileDocument();

    private:	// Functions from base classes

       /**
       * From CEikDocument
       */
       CEikAppUi* CreateAppUiL();

	private:

		/**
        * C++ constructor.
        */
        CProfileDocument( CEikApplication& aApp );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

	private:	// Data

        // Own: Engine handler.
        CProfileEngineHandler* iEngineHandler;

        // Own: Index handler.
        CProfileIndexHandler* iIndexHandler;

	};

#endif // CPROFILEDOCUMENT_H


// End of File
