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
* Description:  Settings handler for the settings outside of Profiles.
*
*/



#ifndef CPROFILESGSTONESPLUGINSETTINGSHANDLER_H
#define CPROFILESGSTONESPLUGINSETTINGSHANDLER_H

// INCLUDES
#include <e32base.h>
#include <cenrepnotifyhandler.h>
#include <MProfileExternalSettingsHandler.h>
#include "MProfilesGSTonesPluginItemDataInterface.h"
#include "CProfilesGSTonesViewFeatures.h"
#include "CProfilesGSTonesViewSettings.h"

// FORWARD DECLARATIONS
class CCoeEnv;
class CRepository;
class MProfileExternalSettingsObserver;
class CDRMHelper;
class CProfilesGSTonesPluginCalendarDataInterface;
class CProfilesGSTonesPluginClockDataInterface;
class TCoeHelpContext;

// CLASS DECLARATION
/**
*  Setting item class for tone selection.
*  CProfilesGSTonesPluginSettingsHandler launches File List Dialog common component
*  from which user can select a file for ringing tone.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfilesGSTonesPluginSettingsHandler )
    : public CBase,
      public MCenRepNotifyHandlerCallback,
      public MProfileExternalSettingsHandler
	{

    friend class CProfilesGSTonesPluginCalendarDataInterface; // CSI: 36 # This friend is defined in this very same file
    friend class CProfilesGSTonesPluginClockDataInterface; // CSI: 36 # This friend is defined in this very same file

	public:		// Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfilesGSTonesPluginSettingsHandler* NewL( CCoeEnv* aCoeEnv );

        /**
        * Destructor.
        */
		~CProfilesGSTonesPluginSettingsHandler();

        /**
        * Variated destructor.
        */
        void DrmDelete();

	private:	// Functions from base classes

        /**
        * From MCenRepNotifyHandlerCallback
        */
        void HandleNotifyString( TUint32 aId, const TDesC16& aNewValue );
        
        /**
        * From MProfilesExternalSettingsHandler
        */
		CAknSettingItem* CreateSettingItemL( TInt aIdentifier );

        /**
        * From MProfilesExternalSettingsHandler
        */
		void SetObserver(
                MProfileExternalSettingsObserver& aObserver );

        /**
        * From MProfilesExternalSettingsHandler
        */
        TBool AllowedToChange( TInt aSettingItemId ) const;

        /**
        * From MProfilesExternalSettingsHandler
        */
        TBool GetHelpContext( TCoeHelpContext& aContext ) const;

        /**
        * From MProfilesExternalSettingsHandler
        */
        void SetHiddenItems( CAknSettingItemArray& aSettingItemArray ) const;
             
        /**
        * From MProfilesExternalSettingsHandler
        */
        void StoreIfChangedL( TInt aSettingId );

    protected:

        /**
        * C++ constructor.
        */
		CProfilesGSTonesPluginSettingsHandler( CCoeEnv* aCoeEnv );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Variated constructor.
        */
        void DrmConstructL();

	private:	// New functions 

        /**
        * Stores the new Calendar alarm tone in the Central Repository.
        * @param aTone the file to be used as the new Calendar alarm tone.
        */
        void StoreCalendarToneL( const TDesC& aTone );

        /**
        * Stores the new Clock alarm tone in the Central Repository.
        * @param aTone the file to be used as the new Clock alarm tone.
        */
        void StoreClockToneL( const TDesC& aTone );

        /**
        * Removes the given tone from the list of automated content.
        * @param aOldTone tone to be removed from the automated content list.
        */
        void RemoveAutomatedContent( const TDesC& aOldTone );

	private:	// Data 

        /// Ref: Observer for the settings controlled by this handler
        MProfileExternalSettingsObserver* iObserver;

		/// Own: Pointer to Central Repository of Clock App
        CRepository* iClockSession;

		/// Own: Pointer to Central Repository of Calendar App
        CRepository* iCalendarSession;

		/// Own: Pointer to clock alert tone notify handler
		CCenRepNotifyHandler* iClockNotifyHandler;

		/// Own: Pointer to calendar alert tone notify handler
		CCenRepNotifyHandler* iCalendarNotifyHandler;

        /// Current Clock alert tone
        TFileName iClockTone;

        /// Current Calendar alert tone
        TFileName iCalendarTone;

        /// Ref: Pointer to the control environment
        CCoeEnv* iCoeEnv;

        /// Own: Interface for File list setting item for calendar tone
        CProfilesGSTonesPluginCalendarDataInterface* iCalendarInterface;
        
        /// Own: Interface for File list setting item for clock tone
        CProfilesGSTonesPluginClockDataInterface* iClockInterface;

        /// Own: Drm helper for removing automated content when needed:
        CDRMHelper* iDrmHelper;

        /// Own: The variable features of Tones View
        CProfilesGSTonesViewFeatures* iFeatures;

        /// Own: The settings only in Tones View
        CProfilesGSTonesViewSettings* iTonesViewSettings;
	};

// This friend class of CProfilesGSTonesPluginSettingsHandler provides the
// interface for the setting item to actually load and store the Calendar
// alert tone name to the handler above.
NONSHARABLE_CLASS( CProfilesGSTonesPluginCalendarDataInterface )
    : public CBase,
      public MProfilesGSTonesPluginItemDataInterface
	{
	public:		// Constructors and destructor

        /**
		* C++ constructor.
        */
        CProfilesGSTonesPluginCalendarDataInterface(
                CProfilesGSTonesPluginSettingsHandler& aSettingsHandler );

        /**
        * Destructor.
        */
		~CProfilesGSTonesPluginCalendarDataInterface() {};

	private:	// Functions from base classes

        /**
        * From MProfilesGSTonesPluginItemDataInterface
        */
		virtual const TDesC& ItemData();

        /**
        * From MProfilesGSTonesPluginItemDataInterface
        */
		virtual void SetItemDataL( const TDesC& aDataBuf );

	private:	// Data

        /// Settings handler containing the setting to be controlled
        CProfilesGSTonesPluginSettingsHandler& iSettingsHandler;
	};

// This friend class of CProfilesGSTonesPluginSettingsHandler provides the
// interface for the setting item to actually load and store the Clock
// alert tone name to the handler above.
NONSHARABLE_CLASS( CProfilesGSTonesPluginClockDataInterface )
    : public CBase,
      public MProfilesGSTonesPluginItemDataInterface
	{
	public:		// Constructors and destructor

        /**
		* C++ constructor.
        */
        CProfilesGSTonesPluginClockDataInterface(
                CProfilesGSTonesPluginSettingsHandler& aSettingsHandler );

        /**
        * Destructor.
        */
		~CProfilesGSTonesPluginClockDataInterface() {};

	private:	// Functions from base classes

        /**
        * From MProfilesGSTonesPluginItemDataInterface
        */
		virtual const TDesC& ItemData();

        /**
        * From MProfilesGSTonesPluginItemDataInterface
        */
		virtual void SetItemDataL( const TDesC& aDataBuf );

	private:	// Data

        /// Settings handler containing the setting to be controlled
        CProfilesGSTonesPluginSettingsHandler& iSettingsHandler;
	};

#endif // CPROFILESGSTONESPLUGINSETTINGSHANDLER_H

// End of File

