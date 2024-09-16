
# CS_525 Assignment 1 - Storage Manager


Implement a storage manager that allows read/writing of blocks to/from a file on disk

Aim - Creating a basic storage manager that can read blocks from a disk file into memory and write blocks from memory to a disk file. The storage manager will handle, (PAGE_SIZE), pages of fixed size. It will have methods for closing, opening and creating files besides writing and reading pages from files.

In an open file, storage manager will keep track of various types of information, which includes, name of the file, current page number, total number of pages present in the file and file pointer or POSIX file descriptor.
## Running Tests

To run the first set of test cases 'test_assign1_1.c', run the following command -
```bash
  make
./test_assign1_1.o
```
To run the second set of test cases 'test_assign1_2.c', run the following command -
```bash
  make
./test_Assign1_2.o
```
To clean solution, run the following command - 

```bash
  make clean
```
To check leak
```bash
sudo apt install valgrind 
valgrind --leak-check=full --track-origins=yes ./test_assign1_1.o
valgrind --leak-check=full --track-origins=yes ./test_assign1_2.o
```

## Solution Description

This command will create new file named  as name_of_File. File with similar name should not exist before creating one.
```bash
RC createPageFile ( char * name_of_File );
```

The file 'name_of_File', which was created with previous 'createPageFile' will be opened with this command. After successful implementation, files_Handle object parameter will become handle to open this file. 
This handle will be used to modify changes in pages of file. Opening file more than once is not an error with the help of different files_Handle object every time. New 'instance' will be created for each request of OpenFile. But it will  be an error if given files_Handle is already a handle for another open file while calling OpenFile.

```bash
RC openPageFile ( char * name_of_File , SM_FileHandle * files_Handle ) ;
```

This command will close the open file instance that files_Handle refers to,which was opened by OpenFile command. When file is closed, all pages in the buffer pool is removed. Trying to close this file while pages are still present in the buffer pool will generate an error. 
```bash
RC closePageFile ( SM_FileHandle * files_Handle );
```
To  run this command successfully, file with given name must exist. This command will destroy name_of_File paged file.

```bash
RC destroyPageFile ( char * name_of_File );
```
This command read blocks of data from a file which is related to open file descriptor. It reads block at -pageNum- and stores content in -memPage- handle. If file does not contain enough pages it should return RC_NO_PAGE.

```bash
RC readBlock ( int pageNum , SM_FileHandle * files_Handle , SM_PageHandle memPage );
```

This command will give current position of page in this file.
```bash
getBlockPos ( SM_FileHandle * files_Handle ) ;
```
This command reads the first page of a block in a given file.
```bash
RC readFirstBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage ) ;
```
This command reads previous page of the current_Page_Number. RC_NO_PAGE error will occur if current_Page_Number is first page of block.
```bash
RC readPreviousBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command reads the present page according to current_Page_Number.
```bash
RC readCurrentBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command reads the next page of the current_Page_Number. RC_NO_PAGE error will occur if current_Page_Number is last page of block.
```bash
RC readNextBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command reads the last page of a block in a given file.
```bash
RC readLastBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command will write content of memPage to block indexed at -pageNum- using absolute current position.
```bash
RC writeBlock (int pageNum , SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command will write content of -memPage- to files_Handle indexed at current_Page_Number using current position.
```bash
RC writeCurrentBlock ( SM_FileHandle * files_Handle , SM_PageHandle memPage );
```
This command will check whether files_Handle contains any value or not. Move the position of pointer towards the end of the file. Create empty block and this page will have 0 bytes. totalNumPages will be increased by 1.
```bash
RC appendEmptyBlock ( SM_FileHandle * files_Handle );

```
This command will check the totalNumPages in the file and if they are less than numberOfPages, then it will increase totalNumberPages by running appendEmptyBlock() in the loop.
```bash
RC ensureCapacity (int numberOfPages , SM_FileHandle * files_Handle ) ;
```
## Authors



Akshar Patel - A20563554

Ankita Chirame - A20543083

Tushar Patel - A20561012