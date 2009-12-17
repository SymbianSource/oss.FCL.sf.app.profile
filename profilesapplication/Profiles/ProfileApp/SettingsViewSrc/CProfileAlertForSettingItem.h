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
* Description:  Setting item class for "Alert for" setting.
*
*/



#ifndef CPROFILEALERTFORSETTINGITEM_H
#define CPROFILEALERTFORSETTINGITEM_H

// INCLUDES
#include <aknsettingitemlist.h>
#include <MPbkContactDbObserver.h>
#include <akncheckboxsettingpage.h> // For CSelectionItemList member data

// FORWARD DECLARATIONS
class CCoeEnv;
class CPbkContactChangeNotifier;

// CONSTANTS
const TInt KMaxPbkContactGroupNameLength( 64 );

// CLASS DECLARATION
/**
*  Setting item class for "Alert for" setting.
*  CProfileAlertForSettingItem reads user defined groups from
*  Phonebook contact database and shows a list of the groups
*  in a check box setting page (CAknCheckBoxSettingPage).
*  CProfileAlertForSettingItem is inherited from MPbkContactDbObserver
*  in order to catch Phonebook contact group change events and
*  dynamically update its contents.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileAlertForSettingItem ) : public CAknSettingItem
								  ,public MPbkContactDbObserver
	{

	public:		// Constructors and destructor

        /**
        * C++ constructor.
        * @param aIdentifier Resource identifier.
        * @aContactList Array where selected contact groups are stored.
        * @aCoeEnv A pointer to Control Environment.
        */
        CProfileAlertForSettingItem(
			TInt aIdentifier,
			RArray<TContactItemId>& aContactList,
			CCoeEnv* aCoeEnv );

		/**
		* Destructor
		*/
		~CProfileAlertForSettingItem();

	private:	// Enumerations

		enum TAlertForStatus
			{
			EAlertForAllCallers,
			EAlertForSingleGroup,
			EAlertForSeveralGroups
			};

	private:	// New functions

		/**
		* Resolves the alert for status of iPopupArray.
		* @return Returns the alert for status of iPopupArray.
		*/
		TAlertForStatus AlertForStatus() const;

		/**
		* Sets selection status to iPopupArray at specified index.
		* @param aIndex Index in iPopupArray.
		* @param aStatus Selection status of an item.
		*/
		void SetSelectionStatus( TInt aIndex, TBool aStatus );

		/**
		* Returns selection status of an item in iPopupArray.
		* @param aIndex Index in iPopupArray.
		* @return Returns selection status of an item.
		*/
		TBool SelectionStatus( TInt aIndex ) const;

		/**
		* Populates iPopupArray. Reads contact group names from Phonebook engine.
		*/
		void PopulatePopupArrayL();

		/**
		* Sets other groups unselected if "All callers" has been selected.
		*/
		void CheckAllCallersSetting();

		/**
		* Updates iPopupArray and if setting page is opened,
		* updates items of the setting page too.
		*/
		void UpdateSettingPageL();

		/**
		* Stores currently selected groups to given array.
		* @param aArray An array to store currently selected groups to.
		*/
		void StoreL( RArray<TContactItemId>& aArray ) const;

		/**
		* Loads selected groups from given array and sets them to iPopupArray.
		* @param aArray An array to load currently selected groups from.
		*/
		void LoadL( const RArray<TContactItemId>& aArray );

	private:	// Functions from base classes

        /**
        * From CAknSettingItem
        */
        void CompleteConstructionL();

        /**
        * From CAknSettingItem
        */
		void StoreL();

        /**
        * From CAknSettingItem
        */
		void LoadL();

        /**
        * From CAknSettingItem
        */
		void EditItemL( TBool aCalledFromMenu );

        /**
        * From CAknSettingItem
        */
		const TDesC& SettingTextL();

        /**
        * From MPbkContactDbObserver
        */
		void HandleDatabaseEventL( TContactDbObserverEvent aEvent );

        /**
        * From MAknSettingPageObserver
        */
		void HandleSettingPageEventL(
			CAknSettingPage* aSettingPage,
			TAknSettingPageEvent aEventType );

	private:	// Data

		// Contains all items in check box setting list
		// (item name string and selection status boolean value)
		CSelectionItemList iPopupArray;

		// Own: Localised text "All callers"
		HBufC* iTextAllCallers;

		// Own: Localised text "Several groups"
		HBufC* iTextSeveralGroups;

		// Text to return with SettingTextL()
		TBuf<KMaxPbkContactGroupNameLength> iSettingText;

		// An array which contains contact item id:s of all contact groups
		RArray<TContactItemId> iFullContactArray;

		// Array of selected contact groups
		RArray<TContactItemId>& iExternalContactArray;

		// Own: PhoneBook contact change notifier
		CPbkContactChangeNotifier* iChangeNotifier;

		// Own: PhoneBook contact engine
		CPbkContactEngine* iPbkContactEngine;

		// Ref: Pointer to Control Environment
		CCoeEnv* iCoeEnv;

	};

#endif // CPROFILEALERTFORSETTINGITEM_H

// End of File
