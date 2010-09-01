/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  GSProfilesPlugin implementation.
*
*/

#ifndef GSPROFILESPLUGIN_H
#define GSPROFILESPLUGIN_H

// User includes

#include <psmpluginbase.h>  // power save management plugin base


// System includes
#include <gsplugininterface.h>
#include <ConeResLoader.h>
#include <AknServerApp.h>

// Classes referenced
class CAknNullService;
class CRepository;
class CApaDocument;
// Constants
const TUid KGSProfilesPluginUID       = { 0x10283386 };  // dll uid
const TUid KGSProfilesPluginImplUID   = { 0x10283387 };  // dll impl uid
const TUid KGsProfilesAppUid          = { 0x100058F8 };  // from ProfileApp.mmp

_LIT( KGSProfilesApp,    "z:\\sys\\bin\\ProfileApp.exe" );
_LIT( KGSProfilesPluginResourceFileName, "z:GSProfilesPluginRsc.rsc" );
_LIT( KGSProfilesPluginIconDirAndName, "z:GSProfilesPlugin.mbm"); // Use KDC_BITMAP_DIR


enum TPsmPluginKeys
    {
    EPsmPluginKeyVibra = 1,
    EPsmPluginKeyKbdVolume = 2,
    EPsmPluginTactileFeedback = 3,
    EPsmPluginAudioFeedback = 4
    };


// CLASS DECLARATION

//For embedding CC in CS
class TAppInfo
    {
    public:
        TAppInfo( TUid aUid, const TDesC& aFile )
            : iUid( aUid ), iFile( aFile )
            {}
        TUid iUid;
        TFileName iFile;
    };


/**
* CGSProfilesPlugin.
*
* This class handles state and application logic of CGSProfilesPlugin.
* The plugin is a type of EGSItemTypeSettingDialog and therefore the GS FW will
* call HandleSelection() instead of DoActivate(). No CAknView functionality is
* supported even though the base class is CAknView derived via
* CGSPluginInterface.
*
*/
class CGSProfilesPlugin : public CGSPluginInterface,
                          public MAknServerAppExitObserver // Embedding
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSProfilesPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSProfilesPlugin();

    public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;

        /**
        * See base class.
        */
        TGSListboxItemTypes ItemType();

        /**
        * See base class.
        */
        void GetValue( const TGSPluginValueKeys aKey,
                       TDes& aValue );

        /**
        * See base class.
        */
        void HandleSelection( const TGSSelectionTypes aSelectionType );
        
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );

    protected: // New

        /**
        * C++ default constructor.
        */
        CGSProfilesPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    protected: // From CAknView

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView finctionality.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * This implementation is empty because this class, being just a dialog,
        * does not implement the CAknView finctionality.
        */
        void DoDeactivate();

    private:

        /**
        * Opens localized resource file.
        */
        void OpenLocalizedResourceFileL(
            const TDesC& aResourceFileName,
            RConeResourceLoader& aResourceLoader );

        /**
        * Launches provisioning application.
        */
        void LaunchProfilesAppL();
        
        /**
        * Launches application as embedded.
        */
        void EmbedAppL( const TAppInfo& aApp );
        
    protected:

        // Resource loader.
        RConeResourceLoader iResources;
        
        CAknNullService* iNullService;
        
        CApaDocument* iEmbedded;
    };




class TPsmSettingItems
    {
    public:
        TInt iVibraAlert;
        TInt iKeypadVolume;
        TInt iTactileFeedback;
        TInt iAudioFeedback;
    };


/**
* CPSMProfilesPlugin.
*
* Psm server calls this plugin is when power save mode changes. Plugin
* is needed for writing psm specific setting values into profile 
* central repository. These settings are vibrating alert, keypad volume,
* tactile feedback and audio feedback. Psm values come from
* psm server which reads them from CPSMProfilesPlugin .xml file
* (10283386.xml).
*
*/
class CPSMProfilesPlugin : public CPsmPluginBase
    {
    public:
        
        /**
        * Two-phased constructor.
		* @return The created object.
        */
        static CPSMProfilesPlugin* NewL( TPsmPluginCTorParams& initParams );
        
        /**
        * Destructor.
        */
        virtual ~CPSMProfilesPlugin();

	private:
	    CPSMProfilesPlugin( TPsmPluginCTorParams& aInitParams );
	    void ConstructL();
	    

    public: // From CPsmPluginBase
        void NotifyModeChange( const TInt aMode );

	private:
	    void NotifyModeChangeL( const TInt aMode );
        void GetPsmConfigSettingsL( TPsmSettingItems& aItems );
	    void GetActiveProfileSettingsL( TPsmSettingItems& aItems );
	    //void GetActiveProfileSettings2L( TPsmSettingItems& aItems );
	    TUint32 ResolveKey( TUint aPartialKey, TInt aProfileId );
	    void CPSMProfilesPlugin::Panic( TInt aReason );
    };




#endif // GSPROFILESPLUGIN_H
// End of File
