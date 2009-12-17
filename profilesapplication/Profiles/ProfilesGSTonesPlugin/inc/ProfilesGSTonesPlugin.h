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
* Description:    Header file for plugin.
*
*/


#ifndef PROFILESGSTONESPLUGIN_H
#define PROFILESGSTONESPLUGIN_H

// Includes
#include <GSPluginInterface.h>
#include <coneresloader.h>
#include <MProfileViewHandler.h>
#include <MProfileChangeObserver.h>


// FORWARD DECLARATIONS
class CProfileSettingsContainer;
class CProfileGSTonesTopControl;
class CProfilesGSTonesPluginSettingsHandler;
class CProfileEngineHandler;
class MProfilesLocalFeatures;
class CProfileChangeNotifyHandler;
class CProfileNaviPaneContainer;

// CLASS DECLARATION


/**
* This interceptor control sits on top of the component stack. Its only purpose
* is to route focus events to CProfileSettingsContainer. Key events are not
* handled at all, they just fall through this component.
*/
NONSHARABLE_CLASS( CProfileGSTonesTopControl ) : public CCoeControl
	{
	public:

		static CProfileGSTonesTopControl* NewL( CCoeControl* aFocusControl );
		void FocusChanged( TDrawNow aDrawNow );

	private:
        CCoeControl* iFocusControl;
	};




/**
* CProfilesGSTonesPlugin view class.
*
* @since Series60_3.1
*/
class CProfilesGSTonesPlugin : public CGSPluginInterface,
    public MProfileChangeObserver,
    public MProfileViewHandler
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor
        * @return 
        */
        static CProfilesGSTonesPlugin* NewL( TAny* aInitParams );
    
        /**
        * Destructor.
        */
        ~CProfilesGSTonesPlugin();

    public: // From CAknView
        
        /**
        * See base class.
        */
        TUid Id() const;
        
        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        /**
        * See base class.
        */
        void DoDeactivate();
        
        /**
        * See base class.
        */
        void HandleCommandL( TInt aCommand );

        /**
        * See base class.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
   
    public: // From CGSPluginInterface
        
        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;
        
        /**
        * See base class.
        */
        TGSListboxItemTypes ItemType();

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;
    
        /**
        * From MProfileChangeObserver
        */
        void HandleActiveProfileEventL(
            TProfileEvent aProfileEvent, TInt aProfileId );

        /**
        * From MProfileViewHandler
        */
		virtual void UpdateClientRect();

        /**
        * From MProfileViewHandler
        */
		virtual CAknView& View();

    protected: // New functions

        /**
         * Shows the information note with the text found from the given
         * resource.
         * @param aResourceId The ID of the resource containing the text to be
         *        shown.
         */
        void ShowInfoNoteL( TInt aResourceId );

    protected: // Constructors
        /**
        * C++ default constructor.
        */
        CProfilesGSTonesPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();
        
    private: // Data
        
        // Resource loader.
        RConeResourceLoader iResourceLoader;
        
        /// Own: Container for the profile settings
        CProfileSettingsContainer* iContainer;

        /// Own: control needed to get the focus on listbox
        CProfileGSTonesTopControl* iTopControl;

        /// Own: handler for the settings external to Profiles
        CProfilesGSTonesPluginSettingsHandler* iSettingsHandler;

        /// Own: handler for Profiles Engine
        CProfileEngineHandler* iEngineHandler;

        // Own: Profile change notify handler.
        CProfileChangeNotifyHandler* iNotifier;

        // Own: Navi pane container.
        CProfileNaviPaneContainer* iNaviPaneContainer;

        // The ID of the previously active profile
        TInt iPreviousProfile;
    };


#endif // PROFILESGSTONESPLUGIN_H

// End of File

