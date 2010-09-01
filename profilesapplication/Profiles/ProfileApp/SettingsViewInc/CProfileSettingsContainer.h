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
* Description:  Container class of settings view.
*
*/


#ifndef CPROFILESETTINGSCONTAINER_H
#define CPROFILESETTINGSCONTAINER_H

// INCLUDES
#include    <aknsettingitemlist.h>      // CAknSettingItemList
#include    <MSSSettingsObserver.h>
#include    <ConeResLoader.h>           // RConeResourceLoader
#include    <AknTabObserver.h>          // MAknTabObserver
#include    <MProfileChangeObserver.h>
#include    "MProfileExternalSettingsObserver.h"
#include    "MProfileExternalSettingsHandler.h"
#include    <centralrepository.h>
#include    <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS
class MProfileViewHandler;
class CAknTitlePane;
class CProfileEngineHandler;
class CProfileIndexHandler;
class CProfileTabHandler;
class MProfileExtended;
class CProfileStruct;
class CAknKeySoundSystem;
class CProfileChangeNotifyHandler;
class MProfilesLocalFeatures;
class CProfileSettingsView;
class CAknView;
class CRepository;
class CCenRepNotifyHandler;
class CProfileNaviPaneContainer;

// CLASS DECLARATION
/**
*  Container class of settings view.
*  Inherited from...
*  MSSSettingsObserver
*      to receive notifications of changes in Alternate Line Service setting.
*  MAknTabObserver
*      to receive notifications if tabs are changed.
*  MProfileChangeObserver
*      to receive notification when the active profile changes.
*
*  @lib ProfileSettingsView.lib
*  @since 1.2
*/
class CProfileSettingsContainer : public CAknSettingItemList,
                                  public MSSSettingsObserver,
                                  public MAknTabObserver,
                                  public MProfileChangeObserver,
                                  public MProfileExternalSettingsObserver,
                                  public MCenRepNotifyHandlerCallback
    {
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        * Note, this constructor has been deprecated and should not be used.
        * @return always NULL.
        */
        IMPORT_C static CProfileSettingsContainer* NewL(
            CAknView& aAknView,
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler* aIndexHandler,
            CProfileTabHandler* aTabHandler,
            CAknTitlePane* aTitlePane );

        /**
        * Two-phased constructor.
        * @param aAknView Parent view to get ClientRect and set MopParent.
        * @param aEngineHandler Engine handler to access Profile Engine.
        * @param aExternalSettingsHandler handler class for those settings that
        *        are saved outside of Profiles.
        * @param aIndexHandler Index handler for supporting several profiles
        *        in view functionality. Can be NULL.
        * @param aTabHandler Tab handler for supporting tab functionality
        *        in Settings view. Can be NULL.
        * @param aTitlePane Title pane access for setting title pane text.
        *        Can be NULL.
        */
        IMPORT_C static CProfileSettingsContainer* NewL(
            MProfileViewHandler& aProfileViewHandler,
            CProfileEngineHandler& aEngineHandler,
            MProfileExternalSettingsHandler& aExternalSettingsHandler,
            CProfileIndexHandler* aIndexHandler,
            CProfileTabHandler* aTabHandler,
            CAknTitlePane* aTitlePane );

        /**
        * Two-phased constructor.
        * @param aAknView Parent view to get ClientRect and set MopParent.
        * @param aEngineHandler Engine handler to access Profile Engine.
        * @param aIndexHandler Index handler for supporting several profiles
        *        in view functionality. Can be NULL.
        * @param aTabHandler Tab handler for supporting tab functionality
        *        in Settings view. Can be NULL.
        * @param aTitlePane Title pane access for setting title pane text.
        *        Can be NULL.
        * @param aResourceId the id of the setting item list resource.
        */
        IMPORT_C static CProfileSettingsContainer* NewL(
            MProfileViewHandler& aProfileViewHandler,
            CProfileEngineHandler& aEngineHandler,
            MProfileExternalSettingsHandler& aExternalSettingsHandler,
            CProfileIndexHandler* aIndexHandler,
            CProfileTabHandler* aTabHandler,
            CAknTitlePane* aTitlePane,
            TInt aResourceId );

        /**
        * Destructor.
        */
        ~CProfileSettingsContainer();

    public:     // New functions

        /**
        * Tells if information of a setting can be changed.
        * If EFalse is returned, EditItemL for the setting item in question
        * should not be called and "Change" should be removed from the Options menu.
        * @param aSettingItemId The setting item ID.
        * @return Returns if information of a setting can be changed.
        */
        IMPORT_C TBool AllowedToChange( TInt aSettingItemId ) const;

		/**
		* Runs CheckFileExistenceL (by calling DoFileCheckL)
        * @param TAny* aObj CProfileSettingContainer pointer
		* @return 1
        */
		static TInt StartFileCheckL( TAny* aObj );

     public:     // Functions from base classes

        /**
        * From CAknSettingItemList.
        */
        IMPORT_C void EditItemL( TInt aIndex, TBool aCalledFromMenu );
     
     public:  // from MCentRepNotifyHandlerCallback
		    /**
        * @see MCentRepNotifyHandlerCallback.
        * @param aId Id of the key that has changed.
        * @param aNewValue The new value of the key.
        */
        void HandleNotifyInt(
            TUint32 aId, TInt aNewValue );

    protected:    // New functions

        /**
        * Frees resources allocated for profile data.
        * Remember to maintain class state before calling this method!
        */
        void ReleaseProfile();

        /**
        * Reads profile data from Profile Engine.
        */
        void ReadProfileL();

        /**
        * Hides all setting items that have been set hidden for current profile
        * ands sets visible all that aren't.
        */
        void SetHiddenFlagsL();

        /**
        * Updates all setting items.
        * Called when the changed settings must be updated to the UI.
        * @param aReadProfileSettings if this is ETrue, the profile settings are
        * read from Profiles Engine.
        */
        void UpdateSettingsL( TBool aReadProfileSettings = ETrue );

        /**
        * Creates a setting item for FileListDialog.
        * @param aFileName Filename.
        * @param aIdentifier Setting item identifier.
        * @return Returns the created setting item object.
        */
        CAknSettingItem* CreateFileListSettingItemL(
            TDes& aFileName, TInt aIdentifier );

        /**
        * Returns the index of a setting item in setting item array.
        * @param aId Setting item identifier.
        * @return Returns the index of a setting item if found, otherwise
        *         KErrNotFound.
        */
        TInt IndexForId( TInt aId ) const;

        /**
        * Sets one setting item hidden.
        * @param aId Identifier of the setting item.
        * @param aArray Pointer to setting item array.
        * @param aHidden Boolean hidden value.
        */
        void SetItemHidden( TInt aId, CAknSettingItemArray* aArray, TBool aHidden );

		/**
		* Sets title pane text.
		* @param aTitle Title pane text.
		*        If descriptor has zero length, the application name is used.
		*/
		void SetTitlePaneTextL( const TDesC& aTitle );

        /**
        * Updates names of profiles in tabs and/or title pane.
        */
        void UpdateProfileNamesL();
        
        /*
        * Launches confirmation dialog
        * @param aTitleText Dialog title text
        */
        void LaunchDialogL( const TDesC& aTitleText );
        
        /*
        * Chechks that used tones assigned for settings can be found.
        * If tone is not found, user is informed by confirmation dialog
        * and tone is replaced with default tone. 
        */
        void CheckFileExistenceL();
        
        /*
        * Checks whether settingitem is hidden
        * @param aId Identifier of the setting item.
        * @param aArray Pointer to setting item array.
		* @return Returns ETrue if item is hidden.
        */
        TBool IsItemHidden( TInt aId, CAknSettingItemArray* aArray ) const;

		/**
		* Runs CheckFileExistenceL
		* @param	None
		* @return	None
        */
		void DoFileCheckL();

    protected:  // Functions from base classes

        /**
        * From CAknSettingItemList
        */
        CAknSettingItem* CreateSettingItemL( TInt aIdentifier );

        /**
        * From CCoeControl
        */
        void SizeChanged();

        /**
        * From CCoeControl
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

 		/**
        * From CCoeControl
        */
        void HandleResourceChange(TInt aType);

        /**
        * From MEikListBoxObserver
        */
        void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );

        /**
        * From MSSSettingsObserver
        */
        void PhoneSettingChanged(
            TSSSettingsSetting aSetting,
            TInt aNewValue );

        /**
        * From MAknTabObserver
        */
        void TabChangedL( TInt aIndex );

        /**
        * From MProfileChangeObserver
        */
        void HandleActiveProfileEventL(
            TProfileEvent aProfileEvent, TInt aProfileId );

        /**
        * From MProfileExternalSettingsObserver
        */
		void HandleExternalSettingsChangedL();

    protected:

        /**
        * C++ constructor.
        */
        CProfileSettingsContainer(
            MProfileViewHandler& aProfileViewHandler,
            CProfileEngineHandler& aEngineHandler,
            MProfileExternalSettingsHandler& aExternalSettingsHandler,
            CProfileIndexHandler* aIndexHandler,
            CProfileTabHandler* aTabHandler,
            CAknTitlePane* aTitlePane );

        /**
        * Symbian 2nd phase constructor.
        * @param aRect View rect.
        * @param aResourceId the id of the setting item list resource.
        */
        void ConstructL( const TRect& aRect,
                         TInt aResourceId );

    public:     // New functions

        /**
        * Is a setting item being edited?
        * @return ETrue if a setting is being edited, EFalse otherwise.
        */
        IMPORT_C TBool ItemEdited() const;
                         
    private:
        TBool PowerSaveMode();
        TBool PowerSaveModeL();
        TBool PowerSaveMode( TInt aSettingId );

    private:    // Data

        // For reading ALS setting and
        // for receiving change notifications.
        RSSSettings iSSSettings;

        // Own: CR session for KProfilesLVFlags (KProEngFeatureIdTTSRingingTone)
        CRepository* iTTSSession;
        
        /// Own: CR session for KProEngDefaultRingingTone
        CRepository* iSession;

        // Resource loader.
        RConeResourceLoader iResourceLoader;

        // View handler to get ClientRect and set MopParent.
        MProfileViewHandler& iProfileViewHandler;

        // Profile Engine handler.
        CProfileEngineHandler& iEngineHandler;

        // Ref: Handler for settings that are saved outside of Profiles:
        MProfileExternalSettingsHandler& iExternalSettingsHandler;

        // Ref: Index handler.
        CProfileIndexHandler* iIndexHandler;

        // Ref: Tab handler.
        CProfileTabHandler* iTabHandler;

        // Ref: Title pane access.
        CAknTitlePane* iTitlePane;

        // Own: One profile data
        MProfileExtended* iProfile;

        // Own: One profile data.
        CProfileStruct* iCopyProfile;

        // Alternate Line Service support.
        TBool iAlternateLineService;

        // Own: Avkon key sounds
        CAknKeySoundSystem* iKeySounds;

        // Own: Profile change notify handler.
        CProfileChangeNotifyHandler* iNotifier;

        // ETrue if an item is currently being edited.
        TBool iItemEdited;

        // Not own: locally variated features of Profiles:
        MProfilesLocalFeatures* iLocalFeatures;
        
        /// Own: fileserver session
        RFs* iFs;
        
        /// Default tone
        TFileName* iDefaultTone;

        // VoIP supported
        TBool iVoIPStatus;
        
        // A CR and notifier for dynamic VoIP
        CRepository* iVoIPVariation;
        CCenRepNotifyHandler* iVoIPVariationNotifier;

		/// Timer to run CheckFileExistenceL
		CPeriodic* iPeriodic;

        // Navi Pane Container
        CProfileNaviPaneContainer* iNaviPaneContainer;
    };

#endif // CPROFILESETTINGSCONTAINER_H

// End of File
