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
* Description:  Tab handler for handling tabs in Profiles
*
*/


#ifndef CPROFILETABHANDLER_H
#define CPROFILETABHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <AknTabObserver.h>

//  FORWARD DECLARATIONS
class CProfileEngineHandler;
class CProfileIndexHandler;
class CAknNavigationControlContainer;
class CCoeAppUi;
class MCoeView;
class CAknNavigationDecorator;
class CAknTabGroup;

// CLASS DECLARATION

/**
*  Tab handler class for handling tabs in Profiles
*  When instantiated, creates a tab for each profile and puts itself visible.
*  Needs an instance of Index handler to handle indices of the profiles.
*  Inherited from MAknTabObserver in order to receive events when tabs change.
*  That however requires adding the tab group as topmost item in control stack.
*
*  @lib CProfileSettingsView.lib
*  @since 2.5
*/
class CProfileTabHandler : public CBase,
                           public MAknTabObserver
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aEngineHandler An instance of Engine Handler.
        * @param aIndexHandler An instance of Index Handler.
        * @param aNaviPane An instance of CAknNavigationControlContainer.
        * @return Returns an instance of CProfileTabHandler.
        */
        IMPORT_C static CProfileTabHandler* NewL(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler,
            CAknNavigationControlContainer& aNaviPane );

        // Destructor.
        ~CProfileTabHandler();

    public:     // New methods

        /**
        * Returns tab group instance to be added to control stack.
        * @return Returns tab group instance.
        */
        IMPORT_C CAknTabGroup* TabGroup() const;

        /**
        * Sets a tab active.
        * @param aIndex Index of the tab to be activated.
        */
        void SetActiveTab( TInt aIndex );

        /**
        * Refreshes tab texts with profile names.
        */
        void RefreshTabsL();
        
        /**
        * Sets tab observer.
        * Tab events are forwarded to the observer if set.
        * @param aTabObserver Tab observer.
        */
        void SetTabObserver( MAknTabObserver* aTabObserver );

    protected:  // New methods

        /**
        * Creates tab group and reads names of profiles to tab texts.
        * Separated to own method to enable updating tabs dynamically
        * (i.e. supports profiles being added or deleted).
        */
        void CreateTabGroupL();

    protected:  // From base classes

       /**
        * From MAknTabObserver
        */
        void TabChangedL( TInt aIndex );

    protected:

        /**
        * C++ constructor.
        * @param aEngineHandler An instance of Engine Handler.
        * @param aIndexHandler An instance of Index Handler.
        * @param aNaviPane An instance of CAknNavigationControlContainer.
        */
        CProfileTabHandler(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler,
            CAknNavigationControlContainer& aNaviPane );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Profile Engine handler.
        CProfileEngineHandler& iEngineHandler;

        // Index handler
        CProfileIndexHandler& iIndexHandler;

        // Navi pane. Needed for creating tab group.
        CAknNavigationControlContainer& iNaviPane;

        // Own: Navigation decorator.
        CAknNavigationDecorator* iNaviDecorator;

        // Ref: Tab group owned by the navigation decorator.
        CAknTabGroup* iTabGroup;

        // Ref: Tab observer. Tab events are forwarded to the observer if set.
        MAknTabObserver* iTabObserver;

    };

#endif // CPROFILETABHANDLER_H
