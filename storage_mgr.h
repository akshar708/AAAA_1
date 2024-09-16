#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include "dberror.h"

/************************************************************
 *                    handle data structures                *
 ************************************************************/
typedef struct SM_FileHandle {
	char *name_of_File;
	int totalNumPages;
	int current_Page_Number;
	void *informationOfManagement;
} SM_FileHandle;

typedef char* SM_PageHandle;

/************************************************************
 *                    interface                             *
 ************************************************************/
/* manipulating page files */
extern RC initStorageManager (void);
extern RC createPageFile (char *name_of_File);
extern RC openPageFile (char *name_of_File, SM_FileHandle *files_Handle);
extern RC closePageFile (SM_FileHandle *files_Handle);
extern RC destroyPageFile (char *name_of_File);

/* reading blocks from disc */
extern RC readBlock (int pageNum, SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern int getBlockPos (SM_FileHandle *files_Handle);
extern RC readFirstBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC readPreviousBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC readCurrentBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC readNextBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC readLastBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);

/* writing blocks to a page file */
extern RC writeBlock (int pageNum, SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC writeCurrentBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage);
extern RC appendEmptyBlock (SM_FileHandle *files_Handle);
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *files_Handle);

#endif
