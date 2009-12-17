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
* Description:  Container for navi pane decorator.
*
*/



#ifndef CPROFILENAVIPANECONTAINER_H
#define CPROFILENAVIPANECONTAINER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CEikStatusPane;
class CProfileEngineHandler;
class CAknNavigationControlContainer;
class CAknNavigationDecorator;

// CLASS DECLARATION
/**
*  Container for navi pane decorator.
*
*  @lib Profileapp.app
*  @since 2.5
*/
NONSHARABLE_CLASS( CProfileNaviPaneContainer ) : public CBase
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aStatusPane An instance of status pane.
        * @param aEngineHandler An instance of Engine Handler.
        * @return Returns an instance of CProfileNaviPaneContainer
        */
        IMPORT_C static CProfileNaviPaneContainer* NewL(
            CEikStatusPane& aStatusPane,
            CProfileEngineHandler& aEngineHandler );

        /**
        * Destructor.
        */
        ~CProfileNaviPaneContainer();

    public:    // New functions

        /**
        * Sets navi pane text.
        * @param aText Navi pane text.
        */
        IMPORT_C void SetNaviPaneTextL( const TDesC& aText );

        /**
        * Sets active profile name as navi pane text.
        */
        IMPORT_C void SetNaviPaneTextL();

    private:

        /**
        * C++ constructor.
        * @param aEngineHandler An instance of Engine Handler.
        */
        CProfileNaviPaneContainer( CProfileEngineHandler& aEngineHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aStatusPane An instance of status pane.
        */
        void ConstructL( CEikStatusPane& aStatusPane );
        
    private:	// New functions
    	void Release();

    private:    // Data

        // Profile Engine Handler.
        CProfileEngineHandler& iEngineHandler;

        // Ref: Pointer to navi pane
        CAknNavigationControlContainer* iNaviPane;

        // Own: Navigation decorator.
        CAknNavigationDecorator* iNaviDecorator;

    };


#endif // CPROFILENAVIPANECONTAINER_H


// End of File
