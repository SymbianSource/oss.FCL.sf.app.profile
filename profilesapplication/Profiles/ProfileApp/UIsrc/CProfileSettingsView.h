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
* Description:  Settings view class.
*
*/



#ifndef CPROFILESETTINGSVIEW_H
#define CPROFILESETTINGSVIEW_H

// INCLUDES
#include <aknview.h>
#include "MProfileViewHandler.h"

#include "MProfileExternalSettingsHandler.h"



// CONSTANTS
const TUid KProfileAppSettingViewId	= { 2 }; // UID of view

// FORWARD DECLARATIONS
class CProfileEngineHandler;
class CProfileIndexHandler;
class CProfileSettingsContainer;
class CProfileTabHandler;
class CProfileTopControl;
class CProfileDummyHandler;


// CLASS DECLARATION


/**
* This interceptor control sits on top of the component stack. Its only purpose
* is to route focus events to CProfileSettingsContaier. Key events are not
* handled at all, they just fall through this component.
*/
NONSHARABLE_CLASS( CProfileTopControl ) : public CCoeControl
	{
	public:
		static CProfileTopControl* NewL( CCoeControl* aFocusControl );
		void FocusChanged( TDrawNow aDrawNow ); // Overrides CCoeControl::FocusChanged

	private:
        CCoeControl* iFocusControl;
	};


// This class is a dummy implementation for external settings interface needed
// by CProfileSettingsContainer.
NONSHARABLE_CLASS( CProfileDummyHandler ) : public CBase,
                                            public MProfileExternalSettingsHandler
	{
	public:
        CProfileDummyHandler();
        virtual ~CProfileDummyHandler();
        
    public:
        // This should never be called in Profiles App
		CAknSettingItem* CreateSettingItemL( TInt aIdentifier );
		void SetObserver(MProfileExternalSettingsObserver& aObserver );
        TBool AllowedToChange( TInt aSettingItemId ) const;
	};


/**
*  Settings view class.
*  @lib ProfileApp.app
*  @since 1.2
*/
NONSHARABLE_CLASS( CProfileSettingsView ) : public CAknView,
							 public MProfileViewHandler
    {
    public:		// Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
        * @return Returns an instance of CProfileSettingsView.
        */
		static CProfileSettingsView* NewLC(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler);

		/**
		* Destructor.
		*/
		~CProfileSettingsView();

	public:		// Functions from base classes

        /**
        * From CAknView
        */
	    TUid Id() const;

        /**
        * From CAknView
        */
		void HandleCommandL( TInt aCommand );

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
        * From MProfileViewHandler
        */
        void UpdateClientRect();

		/**
        * From MProfileViewHandler
        */
        CAknView& View();

	private:	// Constructors and destructors

		/**
		* C++ constructor.
        * @param aEngineHandler Engine Handler.
        * @param aIndexHandler Index Handler.
		*/
		CProfileSettingsView(
            CProfileEngineHandler& aEngineHandler,
            CProfileIndexHandler& aIndexHandler);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

	private:	// Data

        // Engine handler
        CProfileEngineHandler& iEngineHandler;

        // Index handler
        CProfileIndexHandler& iIndexHandler;

		// Own: Settings container.
        CProfileSettingsContainer* iContainer;

        // Own: Tab handler.
        CProfileTabHandler* iTabHandler;

        // Own: Control to route focus event to settings container
        CProfileTopControl* iTopControl;

        // Own: Dummy external settings handler for settings container
        CProfileDummyHandler* iDummyHandler;
    };

#endif // CPROFILESETTINGSVIEW_H

// End of File
