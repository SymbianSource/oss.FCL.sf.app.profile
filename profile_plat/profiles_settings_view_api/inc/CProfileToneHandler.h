/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A helper interface to be used by S60 applications for
*                setting ringing and alert tones for different profiles.
*                Registers DRM protected tones as automated content
*                and removes old content from automated content list.
*                Shows a content activation query if necessary.
*
*/



#ifndef CPROFILETONEHANDLER_H
#define CPROFILETONEHANDLER_H

//  INCLUDES
#include <f32file.h> // CBase, RFs
#include <msssettingsobserver.h> // MSSSettingsObserver, RSSSettings
#include <apmstd.h> // KMaxDataTypeLength
#include <data_caging_path_literals.hrh>

// DATA TYPES

/**
*  Enumerations for different tone settings in Profiles.
*  Should there become a need e.g. for separately place line 1 and line 2
*  ringing tones, we just add them to these enumerations.
*/
enum TProfileTones
    {
    EProfileRingingToneSetting              = 1,
    EProfileVideoCallToneSetting            = 2,
    EProfileMessageAlertToneSetting         = 3,
    EProfileInstantMessageAlertToneSetting  = 4,
    EProfileEmailAlertToneSetting           = 5
    };

// FORWARD DECLARATIONS
class DRMCommon;
class CDRMHelper;
class MProfileExtended;
class MProfileEngineExtended;
class MProfilesLocalFeatures;

// CLASS DECLARATION

/**
*  A helper interface to be used by S60 applications for
*  setting ringing and alert tones for different profiles.
*  Registers DRM protected tones as automated content
*  and removes old content from automated content list.
*  Shows a content activation query and other DRM related
*  notes if necessary.

*
*  @lib ProfileSettingsView.lib
*  @since Series 60 2.6
*/
class CProfileToneHandler : public CBase, public MSSSettingsObserver
    {
    public:  // Constructors and destructor

        // Two-phased constructors.
        IMPORT_C static CProfileToneHandler* NewL();
        IMPORT_C static CProfileToneHandler* NewLC();

        // Destructor.
        ~CProfileToneHandler();

    public: // New functions

        /**
         * Sets the ringing tone of the current active profile.
         * If the file does not exist, leaves with KErrNotFound.
         * If the file is of an unsupported type, leaves with KErrNotSupported.
         * If the file is an unprotected MP3/AAC in user data area
         * and if those are not allowed as ringing or alert tones,
         * leaves with KErrPermissionDenied.
         * If the file is a WMA file in user data area and those are not
         * allowed, leaves with KErrArgument.
         * If the active profile is not allowed to be changed
         * (e.g. Drive-profile) leaves with KErrAccessDenied.
         * If Alternate Line Service is enabled, finds out the line in use
         * and sets the ringing tone for that line respectively.
         * @since 2.6
         * @param aFileName Full path and filename of the tone to be used
         *        as the new ringing tone. An empty descriptor
         *        will be replaced with "No_Sound.wav".
         * @return KErrNone if successful,
         *         KErrCancel if user answers No to a DRM activation query.
         *         (See DRMHelper::SetAutomatedPassive)
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
         */
        IMPORT_C TInt SetActiveProfileRingingToneL( const TDesC& aFileName );

        /**
         * Sets a ringing or alert tone for a profile.
         * If the file does not exist, leaves with KErrNotFound.
         * If the file is of an unsupported type, leaves with KErrNotSupported.
         * If the file is an unprotected MP3/AAC in user data area
         * and if those are not allowed as ringing or alert tones,
         * leaves with KErrPermissionDenied.
         * If the file is a WMA file in user data area and those are not
         * allowed, leaves with KErrArgument.
         * If the file is not allowed to be changed
         * leaves with KErrAccessDenied.
         * If the selected profile is not allowed to be changed
         * (e.g. Drive-profile) leaves with KErrAccessDenied.
         * In case of a ringing tone, if Alternate Line Service is enabled,
         * finds out the line in use and sets the ringing tone for that
         * line respectively.
         * @since 2.6
         * @param aProfileId The ID of the profile (NOT an index).
         * @param aToneSetting Identifies the tone which is updated.
         * @param aFileName Full path and filename of the tone to be used
         *        as the new ringing or alert tone. An empty descriptor
         *        will be replaced with "No_Sound.wav".
         * @return Returns KErrNone if successful,
         *         KErrCancel if user answers No to a DRM activation query.
         *         (See DRMHelper::SetAutomatedPassive)
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
         */
        IMPORT_C TInt SetProfileToneL( TInt aProfileId,
            TProfileTones aToneSetting, const TDesC& aFileName );

        /**
         * Sets a ringing or alert tone for a profile and not check the file.
         * This function only can be used when user set a ringing or alert tone
         * for some profiles and avoid to repeat check the file.
         * Notice: the SetProfileToneL should be executed before this function, because
         * SetProfileToneL will check the file.
         * leaves with KErrPermissionDenied.
         * If the file is a WMA file in user data area and those are not
         * allowed, leaves with KErrArgument.
         * If the file is not allowed to be changed
         * leaves with KErrAccessDenied.
         * If the selected profile is not allowed to be changed
         * (e.g. Drive-profile) leaves with KErrAccessDenied.
         * In case of a ringing tone, if Alternate Line Service is enabled,
         * finds out the line in use and sets the ringing tone for that
         * line respectively.
         * @param aProfileId The ID of the profile (NOT an index).
         * @param aToneSetting Identifies the tone which is updated.
         * @param aFileName Full path and filename of the tone to be used
         *        as the new ringing or alert tone. An empty descriptor
         *        will be replaced with "No_Sound.wav".
         * @return Returns KErrNone this function didn't check the file, so return KErrNone.
         */
        IMPORT_C TInt SetProfileToneNotCheckL( TInt aProfileId,
            TProfileTones aToneSetting, const TDesC& aFileName );

        /**
         * Sets a ringing or alert tone for all profiles.
         * If the file does not exist, leaves with KErrNotFound.
         * If the file is of an unsupported type, leaves with KErrNotSupported.
         * If the file is an unprotected MP3/AAC in user data area
         * and if those are not allowed as ringing or alert tones,
         * leaves with KErrPermissionDenied.
         * If the file is a WMA file in user data area and those are not
         * allowed, leaves with KErrArgument.
         * If the file is not allowed to be changed
         * (e.g. Drive-profile) method ignores profile changes
         * and selects next profile.
         * In case of a ringing tone, if Alternate Line Service is enabled,
         * finds out the line in use and sets the ringing tone for that
         * line respectively.
         * Does not affect on user created profiles except if the active profile
         * is user created of which the ringing or alert tone is also changed.
         * @since 2.6
         * @param aToneSetting Identifies the tone which is updated.
         * @param aFileName Full path and filename of the tone to be used
         *        as the new ringing or alert tone. An empty descriptor
         *        will be replaced with "No_Sound.wav".
         * @return Returns KErrNone if successful,
         *         KErrCancel if user answers No to a DRM activation query.
         *         (See DRMHelper::SetAutomatedPassive)
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
         */
        IMPORT_C TInt SetToneForAllProfilesL( TProfileTones aToneSetting,
            const TDesC& aFileName );

    public:     // From base classes

        /**
         * From MSSSettings
         */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );

    protected:  // New functions

        /**
         * Stores the settings to Profiles Engine and adds the file to automated
         * content list and removes the old file from automated content list.
         * Do not call until the file is checked using CheckFileL.
         * @param aProfileId The ID of the profile (NOT an index).
         * @param aToneSetting Identifies the tone which is updated.
         * @param aFileName Full path and filename of the tone to be used
         *                  as the new ringing or alert tone.
         */
        void DoSetProfileToneL( TInt aProfileId,
            TProfileTones aToneSetting, const TDesC& aFileName );

        /**
         * Sets the tone to an MProfileExtended instance.
         * @param aProfile The MProfileExtended instance.
         * @param aToneSetting Identifies the tone which is updated.
         * @param aFileName The tone file name.
         */
        void SetToneL( MProfileExtended& aProfile,
            TProfileTones aToneSetting, const TDesC& aFileName );

        /**
         * Reads a tone setting from an MProfileExtended instance.
         * @param aProfile The MProfileExtended instance.
         * @param aToneSetting Identifies the tone which is read.
         * @return Returns the tone file name.
         */
        const TDesC& ReadToneL( const MProfileExtended& aProfile,
            TProfileTones aToneSetting ) const;

        /**
         * Checks that the given tone file exists and that it is
         * a legal file to be used as a ringing or alert tone.
         * @since 2.6
         * @param aFileName Full path and filename of the file to be checked.
         * @return KErrNone if the file can be used as a ringing or alert tone,
         *         KErrNotFound if the file cannot be found,
         *         KErrNotSupported if a file is of an unsupported type or
         *         KErrPermissionDenied if the file is not allowed
         *         to be used as a ringing or alert tone.
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
         */
        TInt CheckFileL( const TDesC& aFileName );

        /**
         * Checks if a DRM protected file can be used as a ringing or alert tone.
         * @since 2.6
         * @param aFileName Full path and filename of the file to be checked.
         * @return KErrNone if the file can be used as a ringing or alert tone or
         *         KErrGeneral if DRM rights are insufficient.
         */
        TInt CheckProtectedFileL( const TDesC& aFileName );

        /**
         * Tells whether a file can be registered as automated content or not.
         * If DRM is not supported, returns always EFalse.
         * @param aFileName File to be registered as automated content.
         * @return Returns ETrue if aFileName can be set as automated content.
         *         Returns EFalse if can't or DRM is not supported.
         */
        TBool CanSetAutomated( const TDesC& aFileName );

        /**
         * Asks user if he/she wants to activate unactivated content.
         * Shows a query. Does nothing if DRM is not supported.
         * @param aFileName File to be registered as automated content.
         * @return Returns KErrCancel if user cancels the query.
         */
        TInt AskAutomated( const TDesC& aFileName );

        /**
         * Registers a file to automated content list without any queries.
         * Errors are omitted. Does nothing if DRM is not supported.
         * @param aToneSetting The type of tone (e.g. ringing tone, SMS tone) to
         *        be registered as automated content.
         * @param aFileName File to be registered as automated content.
         * @return KErrNone or a system-wide error code.
         */
        TInt SetAutomated( TProfileTones aToneSetting, const TDesC& aFileName );

        /**
         * Removes a file from automated content list.
         * Errors are omitted. Does nothing if DRM is not supported.
         * @param aFileName File to be removed from automated content list.
         */
        void RemoveAutomated( const TDesC& aFileName );

        /**
         * Returns data type of a file.
         * @param aFileName Full path and filename.
         * @return Returns the data type of the file.
         */
        TBuf<KMaxDataTypeLength> DataTypeL( const TDesC& aFileName ) const;

        /**
         * Returns whether a file is protected content or not.
         * Returns always EFalse if DRM is not supported.
         * @param aFileName The file name.
         * @return Returns whether the file is protected content or not.
         */
        TBool IsProtected( const TDesC& aFileName ) const;

        /**
         * Returns whether a file is protected content or not.
         * Returns always EFalse if DRM is not supported.
         * @param aFileName The file name.
         * @return Returns whether the file is protected content or not.
         */
        TBool IsProtectedL( const TDesC& aFileName ) const;

		/**
         * Displays an error note.
         * @param aResourceId Resource ID to a TBUF resource.
         */
        void ShowErrorNoteL( TInt aResourceId ) const;

		/**
		* Returns info whether given filetype is allowed as ringingtone
		* @param aDataType Tone MIME type
		* @return ETrue if tone is allowed to be selected
		*/
		TBool AllowSelect( const TDesC& aDataType ) const;

        /**
         * Checks the result of CheckFileL() and leaves if needed. Also checks
         * if the file is protected and can be set as automated content.
         * @param aResult result of the CheckFileL() call made before this.
         * @param aFileName the name of the file to be checked for protection
         * and if it is settable as automated content.
         * @return KErrNone, if ok, otherwise a system-wide error code.
         */
        TInt CheckResultAndProtectionL( TInt aResult,
                                        const TDesC& aFileName );
		/**
         * Checks ringing tone file size.
         * @param aFile         file to check.
         * @param aSizeLimitKB  maximum allowed file size in KB.
         * @return KErrNone, if ok.
         *         KErrTooBig, if configurable file size limit exceeded.
         */
        TInt CheckToneFileSizeL( const TDesC& aFile, TInt aSizeLimitKB );

		/**
         * Reads maximimum ringing tone file size in KB from cenrep.
         */
        void GetMaxToneFileSizeL();

		/**
         * Displays error note about exceeded tone file size.
		 * @param aSizeLimitKB  file size limit.
		 */
        void ShowSizeErrorNoteL( TInt aSizeLimitKB );

    private:

        /// Default constructor
        CProfileToneHandler();

        /// 2nd phase constructor
        void ConstructL();

        /// Constructor for the DRM objects
        void DrmConstructL();

        /// Destructor for the DRM objects
        void ReleaseDrm();

		/// Functions related to WMDRM protection check
		TBool	IsFileWMDRMProtectedL( const TDesC& aFileName );

        /// Called by SetAutomated
        TInt SetAutomatedL( TProfileTones aToneSetting, const TDesC& aFileName );

    private:    // Data

        /// Owns: DRM Helper client.
        CDRMHelper* iDrmHelper;

        /// Owns: DRM Common client
        DRMCommon* iDrmCommon;

        /// Owns: Profile Engine Extended interface.
        MProfileEngineExtended* iProfileEngine;

        /// Owns: Profiles local features.
        MProfilesLocalFeatures* iProfilesFeatures;

        /// Supplementary services client for reading Alternate Line Service.
        RSSSettings iSSSettings;

        /// File Server client.
        RFs iFs;

        /// Alternate Line Setting value.
        TInt iAlternateLineSetting;
    
        /// Maximum allowed tone file size in kilo bytes.
        TInt iMaxSizeKB;
    
    };

#endif      // CPROFILETONEHANDLER_H

// End of File
