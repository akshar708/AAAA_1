#include "storage_mgr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* manipulating page files */

RC initStorageManager(void)
{
    return RC_OK;
}

RC createPageFile(char *Name_of_file)
{
    FILE *fp = fopen(Name_of_file, "w+");
    if (fp == NULL) return RC_FILE_NOT_FOUND;
    else
    {
        // allocate a page of memory and fill the page with `\0` bytes
        void *page_is_Empty = malloc(PAGE_SIZE); 
        memset(page_is_Empty, '\0', PAGE_SIZE);

        // write the page to disk and close the file
        size_t bytesWritten = fwrite(page_is_Empty, sizeof(char), PAGE_SIZE, fp);
        fclose(fp);
        free(page_is_Empty);

        // make sure the page was entirely written
        if (bytesWritten != PAGE_SIZE) return RC_WRITE_FAILED;
        else return RC_OK;
    }
}

long _getFileSize(FILE *file)
{
    // move the file pointer to the end
    fseek(file, 0L, SEEK_END); 

    // find the position of the file pointer from the start of the file
    long size = ftell(file); 

     // move the file pointer back to the beginning
    rewind(file);
    return size;
}

RC openPageFile(char *name_of_File, SM_FileHandle *files_Handle)
{
    FILE *fp = fopen(name_of_File, "r+");

    if (fp == NULL) return RC_FILE_NOT_FOUND;
    else
    {
        // get the size of the file and divide by page size to get `totalNumPages`
        int size = _getFileSize(fp);
        int totalNumPages = size / PAGE_SIZE;
        
        // set metadata
        files_Handle -> name_of_File = name_of_File;
        files_Handle -> totalNumPages = totalNumPages;
        files_Handle -> current_Page_Number = 0;

        // store the file pointer in `informationOfManagement` to use else where
        files_Handle->informationOfManagement = (void *)fp;
        return RC_OK;
    }
}

RC closePageFile (SM_FileHandle *files_Handle)
{
    if (fclose((FILE *)files_Handle->informationOfManagement) == 0)
    {
        // unset the file pointer
        files_Handle -> informationOfManagement = NULL;
        return RC_OK;
    }
    else return RC_FILE_NOT_FOUND;
}

RC destroyPageFile (char *Name_of_file)
{
    if (remove(Name_of_file) == 0) return RC_OK;
	else return RC_FILE_NOT_FOUND;
}

/* reading blocks from disc */

RC readBlock (int pageNum, SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    // check the handle to see if pageNum is in range
    if (pageNum < 0 || pageNum >= files_Handle->totalNumPages) 
        return RC_READ_NON_EXISTING_PAGE;
    FILE *fp = (FILE *)files_Handle->informationOfManagement;

    // check to see if the position was set successfully
    if (fseek(fp, pageNum * PAGE_SIZE, SEEK_SET) == 0)
    {
        size_t bytesRead = fread(memPage, sizeof(char), PAGE_SIZE, fp);

        // make sure the page was entirely read
        if (bytesRead != PAGE_SIZE) return RC_READ_NON_EXISTING_PAGE;
        else return RC_OK;
    }    
    else return RC_FILE_NOT_FOUND;
}

int getBlockPos (SM_FileHandle *files_Handle)
{
    return files_Handle->current_Page_Number;
}

RC readFirstBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    return readBlock(0, files_Handle, memPage);
}

RC readPreviousBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    // get the previous `pageNum`
    int pageNum = files_Handle->current_Page_Number - 1;
    RC result = readBlock(pageNum, files_Handle, memPage);

    // if the block was successfully read (and the pageNum was in range), update `pageNum`
    if (result == RC_OK)
        files_Handle->current_Page_Number = pageNum;
    return result;
}

RC readCurrentBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    return readBlock(files_Handle->current_Page_Number, files_Handle, memPage);
}

RC readNextBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    // get the next `pageNum`
    int pageNum = files_Handle->current_Page_Number + 1;
    RC result = readBlock(pageNum, files_Handle, memPage);

    // if the block was successfully read (and the pageNum was in range), update `pageNum`
    if (result == RC_OK)
        files_Handle->current_Page_Number = pageNum;
    return result;
}

RC readLastBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    return readBlock(files_Handle->totalNumPages - 1, files_Handle, memPage);
}

/* writing blocks to a page file */

RC writeBlock (int pageNum, SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    // check the handle to see if pageNum is in range
    if (pageNum < 0 || pageNum >= files_Handle->totalNumPages) 
        return RC_READ_NON_EXISTING_PAGE;
    FILE *fp = (FILE *)files_Handle->informationOfManagement;

    // check to see if the position was set successfully
    if (fseek(fp, pageNum * PAGE_SIZE, SEEK_SET) == 0)
    {
        size_t bytesWritten = fwrite(memPage, sizeof(char), PAGE_SIZE, fp);
        if (bytesWritten != PAGE_SIZE) return RC_WRITE_FAILED;
        else return RC_OK;
    }    
    else return RC_FILE_NOT_FOUND;
}

RC writeCurrentBlock (SM_FileHandle *files_Handle, SM_PageHandle memPage)
{
    return writeBlock(files_Handle->current_Page_Number, files_Handle, memPage);
}


RC appendEmptyBlock (SM_FileHandle *files_Handle)
{
    FILE *fp = (FILE *)files_Handle->informationOfManagement;

    // check to see if the position was set successfully
    if (fseek(fp, 0, SEEK_END) == 0) 
    {
        // allocate a page of memory and fill the page with `\0` bytes
        void *page_is_Empty = malloc(PAGE_SIZE); 
        memset(page_is_Empty, '\0', PAGE_SIZE);

        // write the page to disk
        size_t bytesWritten = fwrite(page_is_Empty, sizeof(char), PAGE_SIZE, fp);
        free(page_is_Empty);

        // make sure the page was entirely written
        if (bytesWritten != PAGE_SIZE) 
            return RC_WRITE_FAILED;
        else
        {
            files_Handle->totalNumPages++;
            return RC_OK;
        }
    }
    else return RC_FILE_NOT_FOUND;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *files_Handle)
{
    // while `totalNumPages` is less than `numberOfPages` keep calling `appendEmptyBlock`
    while (files_Handle->totalNumPages < numberOfPages)
    {
        RC result = appendEmptyBlock(files_Handle);
        if (result != RC_OK) 
            return result;
    }
    return RC_OK;
}