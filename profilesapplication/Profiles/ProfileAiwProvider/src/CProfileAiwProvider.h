/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile Aiw service provider.
*
*/


#ifndef __CPROFILEAIWPROVIDER_H__
#define __CPROFILEAIWPROVIDER_H__

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <AiwServiceIfMenu.h>

// FORWARD DECLARATIONS
class CProfileToneHandler;
class CProfileEngineHandler;
class CAknPopupList;

// CLASS DECLARATION

/**
*  Profiles AIW Provider
*  AIW Provider for seting files as ringingtone
*
*  @lib ProfileSettingsView.lib
*  @since 3.1
*/

NONSHARABLE_CLASS( CProfileAiwProvider )
 : public CAiwServiceIfMenu
    {
    public: // Constructors and destructors
    
    	/**
         * Two-phase static constructor
         * @return A pointer to a fully constructed CProfileAiwProvider instance
         */
        static CProfileAiwProvider* NewL();
        
        /**
         * Destructor
         */
        virtual ~CProfileAiwProvider();

    private: // From CAiwServiceIfMenu

        void InitialiseL( MAiwNotifyCallback& aFrameworkCallback,
                          const RCriteriaArray& aInterest );

        void HandleServiceCmdL( const TInt& aCmdId,
                                const CAiwGenericParamList& aInParamList,
                                CAiwGenericParamList& aOutParamList,
                                TUint aCmdOptions = 0,
                                const MAiwNotifyCallback* aCallback = NULL );

        void InitializeMenuPaneL( CAiwMenuPane& aMenuPane,
                                  TInt aIndex,
                                  TInt aCascadeId,
                                  const CAiwGenericParamList& aInParamList );

        void HandleMenuCmdL( TInt aMenuCmdId,
                             const CAiwGenericParamList& aInParamList,
                             CAiwGenericParamList& aOutParamList,
                             TUint aCmdOptions = 0,
                             const MAiwNotifyCallback* aCallback = NULL );

    private: // Constructors and destructors
        
        /**
         * Constructor
         */
        CProfileAiwProvider();
      	
      	/**
         * Second phase constructor
         */
        void ConstructL();

	private:	// New functions
		
       /*
       * Reads AIW parameters
       * @param aParamList Generic parameterlist
       * @param aParamType Generic parameterID
       * @return Parameter in text form
       */
        TPtrC GetAiwParamAsDescriptor(
        	const CAiwGenericParamList& aParamList,
        	 TGenericParamId aParamType);
		/*
       * Checks MIME type 
       * @param aMimeTypeString MIME type to be checked
       * @return Returns ETrue if MIME type is valid ( audio or video )
       */
       TBool CheckMimeTypeL(const TDesC& aMimeTypeString);
       
       /*
        * Launches multiselection popup listbox containing profile names
        * @param aPopupTitle Listbox title
        * @param aFilename Filename to be assigned as ringing tone
       	*/
		void LaunchL( const TDesC& aPopupTitle, const TDesC& aFileName );   
		/*
		* Populates listbox
		* @param aItems Pointer to constructed listbox item array
       	*/
		void PopulateListBoxL( CDesCArrayFlat* aItems );
		/*
		* Performs the setting of ringing tone through ProfileSettingsView API
		* @param aFileName Filename to be assigned as ringing tone
		* @param aChangeAllProfiles Boolean value defines whether all 
		* profiles are changed
       	*/
		void PerformProfileChangeL(
		 const TDesC& aFileName, const TBool& aChangeAllProfiles ) const;

        /**
         * Checks that the size of the tone file does not exceed the set limit
         * (if one has been set).
         * @param aFileName the name of the file to be assigned as ringing tone
         * @return ETrue if the size of the file does not rule the file out to
         * be set as ringing tone
         */
        TBool IsToneFileSizeOkL( const TDesC& aFile );

		/**
         * Displays an error note.
         * @param aResourceId Resource ID to a TBUF resource.
         */
        void ShowErrorNoteL( const TDesC& aErrorText ) const;

		/**
         * Checks if file is WMDRM protected.
         * @param aFileName Name of the file to be checked.
         * @return ETrue if file is WMDRM protected, EFalse otherwise.
         */
		TBool IsFileWMDRMProtectedL( const TDesC& aFileName ) const;

		/**
         * Checks if file is located on a remote drive and copies it if so.
         * @param aFileName Name of the file to be checked/copied.
         * @param aTarget File name on device after copying.
         * @return ETrue if file is remote, EFalse otherwise.
         */
		TBool CopyIfRemoteL( const TDesC& aFileName, TFileName& aTarget ) const;

    private: // Data

		/// Own: Profile tone handler
		CProfileToneHandler* iProfileToneHandler;
		
		/// Own: Engine handler.
        CProfileEngineHandler* iEngineHandler;
		
      	/// Ref: AIWCallback
        MAiwNotifyCallback* iConstNotifyCallback;
        
        /// Own: WMA Error text
        HBufC* iWMAErrorText;
        
        /// Own: MP4 Error text
		HBufC* iMP4ErrorText;
        
        /// Own: Unprotected Error text
		HBufC* iUnprotectedErrorText;
		
		/// Own: Not supported Error text
		HBufC* iNotSupportedErrorText;
		
		/// Own: Popup
		CAknPopupList* iPopup;

		/// Own: Popup title text
		HBufC* iPopupTitle;
		
		/// Own: Changed profile information text
		HBufC* iChangedProfilesText;
		
		/// Own: File was copied and set
		HBufC* iCopiedAndChangedText;

		/// Own: Tone file size too big error text
		HBufC* iSizeErrorText;
		
		/// Own: ListBox selection array
		const CArrayFix<TInt>* iSelectionArray;
		
		/// Own: Array for mapping of original indexes in Profile name array
        /// to Profile IDs
		CArrayFixFlat<TInt>* iIndexToIdArray;
		
        /// Resource offset
        TInt iResourceOffset;
     
        /// Maximum allowed size for tone files:
        TInt iMaxSize;
        
        /// Is file video:
        TBool iIsVideo;
     
    };

#endif // __CPROFILEAIWPROVIDER_H__
