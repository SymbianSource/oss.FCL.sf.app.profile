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
* Description:  Interface for loading/storing the settings outside Profiles
*                itself.
*
*/



#ifndef MPROFILEEXTERNALSETTINGSHANDLER_H
#define MPROFILEEXTERNALSETTINGSHANDLER_H

//  INCLUDES
#include <e32std.h>

//  FORWARD DECLARATIONS
class CAknSettingItem;
class CAknSettingItemArray;
class MProfileExternalSettingsObserver;
class TCoeHelpContext;
// CLASS DECLARATION

/**
*  Interface for loading/storing the settings outside Profiles itself.
*
*  @lib ProfileSettingsView.dll
*  @since 3.1
*/
class MProfileExternalSettingsHandler
    {

    public: // New functions

        /**
        * Creates a setting item for a listbox for editing a setting.
        * @param aIdentifier identifies the setting to be edited by the item.
        * @return a setting item for the given setting, the ownership is
        * transferred to the caller.
        * @since 3.1
        */
		virtual CAknSettingItem* CreateSettingItemL( TInt aIdentifier ) = 0;

        /**
        * Sets an observer for the external settings.
        * @param aObserver the observer to be notified when the settings are
        * changed.
        * @since 3.1
        */
		virtual void SetObserver(
                MProfileExternalSettingsObserver& aObserver ) = 0;

        /**
        * Check if the user should be allowed to change the value of the given
        * setting item or not.
        * @param aSettingItemId the ID of the setting item to check for.
        * @return ETrue if modification is allowed, EFalse otherwise.
        * @since 3.1
        */
        virtual TBool AllowedToChange( TInt aSettingItemId ) const = 0;
            
        /**
         * Gets the help context for context sensitive help.
         * @param aContext the context object in which the help context of the
         *        component must be set in the implementation of this method.
         * @return ETrue if this method has set the context appropriately,
         *         otherwise the caller should set the context by itself.
         * @since 3.1
         */
        virtual TBool GetHelpContext( TCoeHelpContext& /* aContext */ ) const
            {
            return EFalse;
            }

        /**
         * Sets hidden items in the setting item array.
         * @param aSettingItemArray array containing the settings items to set
         *        visible/invisible.
         * @since 3.1
         */
        virtual void SetHiddenItems( CAknSettingItemArray& /* aSettingItemArray */ )
            const
            {}

        /**
         * Checks if the setting with the given setting ID has been modified by
         * the user and saves the value in storage if it has been changed.
         * @param aSettingItemId the setting to be checked.
         * @since 3.1
         */
        virtual void StoreIfChangedL( TInt /* aSettingItemId */ ) {}

    };

#endif      // MPROFILEEXTERNALSETTINGSHANDLER_H

// End of File

