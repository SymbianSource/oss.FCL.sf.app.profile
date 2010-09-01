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
* Description:  Main view class.
*
*/



#ifndef CPROFILEMAINVIEW_H
#define CPROFILEMAINVIEW_H

// INCLUDES
#include <aknview.h>
#include "MProfileChangeObserver.h"
#include "MProfileViewHandler.h"

// CONSTANTS
const TUid KProfileAppMainViewId	= { 1 }; // UID of view

// FORWARD DECLARATIONS
class CProfileEngineHandler;
class CProfileIndexHandler;
class CProfileMainContainer;
class CProfileChangeNotifyHandler;
class CProfileNaviPaneContainer;
class MProfilesLocalFeatures;

// CLASS DECLARATION
/**
*  Main view class.
*  Derived from MProfileChangeObserver to get an event when active profile is
*  changed (needed for updating the navi pane).
*
*  @lib ProfileApp.app
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileMainView ) : public CAknView,
                         public MProfileChangeObserver,
                         public MProfileViewHandler
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
        * @return Returns an instance of CProfileMainView.
        */
        static CProfileMainView* NewLC(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler );

        /**
        * Destructor.
        */
        ~CProfileMainView();

	public:     // Functions from base classes

        /**
        * From CAknView
        */
        TUid Id() const;

        /**
        * From CAknView
        */
        void HandleCommandL( TInt aCommand );
        
        /**
        * From MProfileViewHandler
        */
        void UpdateClientRect();

        /**
        * From MProfileViewHandler
        */
		CAknView& View();

    private:    // New functions

		/**
		* Launches a CAknQueryDialog.
		* @param aResourceId Resource id to DIALOG resource.
		* @return Returns ETrue if user accepted the query.
		*/
		TBool DisplayConfirmationQueryL( TInt aResourceId ) const;

		/**
		* HandleCommandL switch separated to own method.
		*/
		void DoCmdActivateL();

		/**
		* HandleCommandL switch separated to own method.
		*/
		void DoCmdCreateNewL();

		/**
		* HandleCommandL switch separated to own method.
		*/
		void DoCmdDeleteL();

		/**
		* HandleCommandL switch separated to own method.
		*/
		void DoCmdContextSpecificOptionsMenuL();
		
		/**
		* HandleCommandL switch separated to own method.
		*/
		void DoCmdTimedL();
		
		/**
		* Displays information note containing time left
		* until the profile expires
		*/
		void DisplayTimedInfoNoteL( TInt aMinutes, TInt aHours ) const;
		/**
		* Displays confirmation note containing time left
		* until the profile expires
		*/
		void DisplayTimedQueryNoteL( TInt aMinutes, TInt aHours ) const;
		
		/**
		* Update cba.
		*/
		void UpdateCbaL(TInt aResourceId);


    private:    // Functions from base classes.

        /**
        * From CAknView
        */
        void DoActivateL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,
            const TDesC8& aCustomMessage );

		/**
        * From CAknView
        */
        void DoDeactivate();

        /**
        * From CAknView
        */
		void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
        * From MProfileChangeObserver
        */
        void HandleActiveProfileEventL(
            TProfileEvent aProfileEvent, TInt aProfileId );

    private:

        /**
        * C++ constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
        */
        CProfileMainView(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler );

	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Engine handler
        CProfileEngineHandler& iEngineHandler;

        // Index handler
        CProfileIndexHandler& iIndexHandler;

        // Top item index of the listbox.
		TInt iTopItemIndex;

        // Own: Main Container.
        CProfileMainContainer* iContainer;

        // Own: Profile change notify handler.
        CProfileChangeNotifyHandler* iNotifier;

        // Own: Navi pane container.
        CProfileNaviPaneContainer* iNaviPaneContainer;
        
        // Not own: locally variated features of Profiles:
        MProfilesLocalFeatures* iLocalFeatures;

    };


#endif // CPROFILEMAINVIEW_H


// End of File
