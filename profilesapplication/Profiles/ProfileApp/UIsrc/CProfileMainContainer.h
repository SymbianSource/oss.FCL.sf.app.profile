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
* Description:  Container class of main view.
*
*/



#ifndef CPROFILEMAINCONTAINER_H
#define CPROFILEMAINCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <eiklbo.h>

// FORWARD DECLARATIONS
class CAknView;
class CAknSingleStyleListBox;
class CProfileEngineHandler;
class CProfileIndexHandler;

// CONSTANTS
const TInt KMainContainerSubControlCount( 1 );

// CLASS DECLARATION
/**
*  Container class of main view.
*
*  @lib ProfileApp.app
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileMainContainer ) : public CCoeControl
							, public MEikListBoxObserver
	{
	public:		// Constructors and destructors

		/**
        * Two-phased constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
        * @param aView Pointer to the main view class.
        * @return Returns an instance of CProfileMainContainer.
        */
		static CProfileMainContainer* NewL(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler,
            CAknView& aView );

		/**
		* Destructor.
		*/
		~CProfileMainContainer();

	public:		// New functions

		/**
		* Populates listbox with names of profiles.
		* Separated to own method to enable updating listbox dynamically
		* (i.e. supports profiles being added or deleted).
        * @param aTopItemIndex If given, sets list boxes top item index.
		*/
		void PopulateListBoxL( TInt aTopItemIndex = KErrNotFound );

		/**
		* Returns the top item index of the listbox.
		* Needed to set index back when returning from personalise state.
        * @return Returns the top item index of the listbox.
		*/
		TInt TopItemIndex() const;

	private:	// Functions from base classes

        /**
        * From CoeControl
        */
		TInt CountComponentControls() const;

        /**
        * From CoeControl
        */
		CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CoeControl
        */
		void SizeChanged();

        /**
        * From CoeControl
        */
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

		/**
        * From CCoeControl
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
        
        /**
        * From CCoeControl
        */
        void HandleResourceChange(TInt aType);

        /**
         * From CCoeControl
         */
        void FocusChanged( TDrawNow aDrawNow );

        /**
        * From MEikListBoxObserver
        */
        void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );

	private:

        /**
        * C++ constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
        * @param aView Pointer to the main view class.
		*/
		CProfileMainContainer(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler,
            CAknView& aView );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aRect Drawing area.
        */
 		void ConstructL( const TRect& aRect );

	private:	// Data

        // Engine handler
        CProfileEngineHandler& iEngineHandler;

        // Index handler
        CProfileIndexHandler& iIndexHandler;

		// The view class (to call HandleCommandL).
		CAknView& iView;

		// Own: Listbox class.
		CAknSingleStyleListBox* iListBox;

	};

#endif // CPROFILEMAINCONTAINER_H


// End of File
