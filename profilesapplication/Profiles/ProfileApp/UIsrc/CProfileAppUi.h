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
* Description:  Application UI class.
*
*/



#ifndef CPROFILEAPPUI_H
#define CPROFILEAPPUI_H

// INCLUDES
#include <aknViewAppUi.h>

// FORWARD DECLARATIONS
class CProfileMainView;
class CProfileSettingsView;
class CProfileEngineHandler;
class CProfileIndexHandler;

// CLASS DECLARATION
/**
*  Application UI class.
*  CProfileAppUi instantiates two views and
*  implements view switching and command handling.
*  The views are owned by the base class.
*
*  @lib ProfileApp.app
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileAppUi ) : public CAknViewAppUi
    {

    public:		// Constructors and destructor

		/**
		* C++ default constructor.
		*/
        CProfileAppUi(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler );

        /**
        * Destructor
        */
        ~CProfileAppUi();

		/**
		* Symbian 2nd phase constructor.
		*/
        void ConstructL();

	public:		// Functions from base classes

        /**
        * From CEikAppUi
        */
		void HandleCommandL( TInt aCommand );

	private:	// Data

        // Engine handler
        CProfileEngineHandler& iEngineHandler;

        // Index handler
        CProfileIndexHandler& iIndexHandler;

		// Ref: Pointer to Main view
		CProfileMainView* iMainView;

		// Ref: Pointer to Settings view
		CProfileSettingsView* iSettingsView;
	};

#endif // CPROFILEAPPUI_H


// End of File
