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
* Description:  Profile index handler (for use of Settings Container
*                if multiple profiles are edited)
*
*/


#ifndef CPROFILEINDEXHANDLER_H
#define CPROFILEINDEXHANDLER_H

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class CProfileEngineHandler;
class CEikListBox;

//  CLASS DECLARATION

/**
*  Profile index handler.
*  For use of Settings Container if multiple profiles are edited.
*
*  @lib ProfileSettingsView.lib
*  @since 2.5
*/
class CProfileIndexHandler : public CBase
    {
    public:

        /**
        * Symbian 2-phase constructor.
        * @param aEngineHandler An instance of Engine Handler.
        * @return Returns an instance of CProfileIndexHandler.
        */
        IMPORT_C static CProfileIndexHandler* NewL(
            CProfileEngineHandler& aEngineHandler );

        // Destructor
        ~CProfileIndexHandler();

    public:     // New methods

        /**
        * Returns the index of current profile.
        * @return Returns the index of current profile.
        */
        IMPORT_C TInt CurrentProfileIndex() const;

        /**
        * Sets the index of current profile.
        * @param aIndex The index of the new current profile.
        */
        IMPORT_C void SetCurrentProfileIndex( TInt aIndex );

        /**
        * Creates the index array.
        */
        void CreateIndexArrayL();

        /**
        * Stores current indices (top item index and current item index).
        */
        void StoreIndices();

        /**
        * Loads previous indices.
        */
        void LoadIndices();

        /**
        * Indexes the index array.
        * @param aIndex Index of the item index.
        * @return Returns item index at index.
        */
        TInt IndexAt( TInt aIndex ) const;

        /**
        * Indexes the top index array.
        * @param aIndex Index of the top item index.
        * @return Returns top item index at index.
        */
        TInt TopIndexAt( TInt aIndex ) const;

        /**
        * Removes items from the index arrays and inserts a new ones.
        * @param aOldIndex Index of the old items to be removed.
        * @param aNewIndex Index of the new items to be added.
        * @param aIndexValue Value of the new item index.
        * @param aTopIndexValue Value of the new top item index.
        * @return Returns KErrNotFound if the old index is not found,
        *         KErrArgument if the new index is out of bounds,
        *         KErrNone if no errors.
        */
        TInt RemoveAndInsert(
            TInt aOldIndex, TInt aNewIndex,
            TInt aIndexValue, TInt aTopIndexValue );

        /**
        * Sets a list box pointer so indices can be stored and loaded.
        * @param aListBox The List box pointer.
        *        Remember to set back to NULL when listbox is deleted!
        */
        void SetListBox( CEikListBox* aListBox );

        /**
        * Calls Engine Handler's ReadIdArrayL and re-arranges index array.
        * @return Returns the index of the edited profile after re-arrange.
        */
        TInt ReadIdArrayAndUpdateL();

    protected:

        /**
        * C++ constuctor.
        * @param aEngineHandler An instance of Engine Handler.
        */
        CProfileIndexHandler(
            CProfileEngineHandler& aEngineHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Profile Engine handler.
        CProfileEngineHandler& iEngineHandler;

		// Index of the currently focused profile.
		TInt iCurrentProfileIndex;

        // Ref: Listbox pointer to load and store top item and current item indices.
        CEikListBox* iListBox;

        // Struct for one index
        struct TItem
            {
            // Current item index in listbox
            TInt iIndex;
	        // Top item index in listbox
	        // Value 0 means that the focused item is the first in view,
	        // 1 means that it's the second and 2 the last.
            TInt iTopIndex;
            };

		// Array of item indices.
		RArray<TItem> iIndexArray;

    };

#endif // CPROFILEINDEXHANDLER_H

// End of File
