/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A thin CAknPopupList extension.
*
*/



#ifndef CPAPPOPUPLIST_H
#define CPAPPOPUPLIST_H

// EXTERNAL INCLUDES
#include <aknPopup.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  A thin CAknPopupList extension.
*  Handles resource changes.
*
*  @lib ProfileSettingsView.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CPapPopupList ) : public CAknPopupList
    {

    public: // New methods

		/**
		* Two-phased constructor.
		* @param aListbox A parameter to CAknPopupList.
		* @param aCbaResource A parameter to CAknPopupList.
		* @param aType A parameter to CAknPopupList.
		*/
		static CPapPopupList* NewL( CEikListBox* aListBox, TInt aCbaResource );
		
		void ConstructL( CEikListBox* aListBox, TInt aCbaResource );

		/**
        * Destructor.
        */	
		virtual ~CPapPopupList();

		/**
		* Fills in ListBox icon array.
		*/
		void SetIconsL();

    private: // From CAknPopupList

        void HandleResourceChange( TInt aType );
        void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );
        void ProcessCommandL( TInt aCommandId );

    private:

		/**
		* C++ constructor.
		*/
		CPapPopupList();

	private: // New methods

	    /**
	     * Updates softkeys.
	     */
	    void UpdateCba();
	    
    private:    // Data

    };

#endif      // CPAPPOPUPLIST_H

// End of File
