/******************************************************************************
 * Project         Persistency
 * (c) copyright   2012
 * Company         XS Embedded GmbH
 *****************************************************************************/
/******************************************************************************
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. If a  copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
******************************************************************************/
 /**
 * @file           persistence_client_library_test.c
 * @ingroup        Persistence client library test
 * @author         Ingo Huerner
 * @brief          Test of persistence client library
 * @see            
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>     /* exit */
#include <check.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../include/persistence_client_library_key.h"
#include "../include/persistence_client_library_file.h"
#include "../include/persistence_client_library_error_def.h"


// protected header, should be used only be persistence components
// included here for testing purpose
#include "../include_protected/persistence_client_library_db_access.h"


#define BUF_SIZE     64
#define NUM_OF_FILES 3
#define READ_SIZE    1024

// definition of weekday
char* dayOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



/**
 * Test the key value interface using different logicalDB id's, users and seats.
 * Each resource below has an entry in the resource configuration table where the
 * storage location (cached or write through) and type (e.g. custom) has been configured.
 */
START_TEST (test_GetData)
{
   int ret = 0;
   unsigned char buffer[READ_SIZE];
   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0xFF with user 0 and seat 0
    *       ==> local value accessible by all users (user 0, seat 0)
    */
   ret = pclKeyReadData(0xFF, "language/country_code",         0, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "Custom plugin -> plugin_get_data_handle",
               strlen((char*)buffer)) == 0, "Buffer not correctly read");
   fail_unless(ret = strlen("Custom plugin -> plugin_get_data_handle"));

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0xFF with user 0 and seat 0
    *       ==> local value accessible by all users (user 0, seat 0)
    */
   ret = pclKeyReadData(0xFF, "pos/last_position",         0, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "CACHE_ +48° 10' 38.95\", +8° 44' 39.06\"",
               strlen((char*)buffer)) == 0, "Buffer not correctly read");
   fail_unless(ret = strlen("CACHE_ +48° 10' 38.95\", +8° 44' 39.06\""));

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0 with user 3 and seat 0
    *       ==> public shared user value (user 3, seat 0)
    */
   ret = pclKeyReadData(0,    "language/current_language", 3, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "CACHE_ Kisuaheli", strlen((char*)buffer)) == 0, "Buffer not correctly read");

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0xFF with user 3 and seat 2
    *       ==> local USER value (user 3, seat 2)
    */
   ret = pclKeyReadData(0xFF, "status/open_document",      3, 2, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "WT_ /var/opt/user_manual_climateControl.pdf", strlen((char*)buffer)) == 0, "Buffer not correctly read");

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0x20 with user 4 and seat 0
    *       ==> shared user value accessible by a group (user 4 and seat 0)
    */
   ret = pclKeyReadData(0x20, "address/home_address",      4, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "WT_ 55327 Heimatstadt, Wohnstrasse 31", strlen((char*)buffer)) == 0, "Buffer not correctly read");

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0xFF with user 0 and seat 0
    *       ==> local value accessible by ALL USERS (user 0, seat 0)
    */
   ret = pclKeyReadData(0xFF, "pos/last_satellites",       0, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "WT_ 17", strlen((char*)buffer)) == 0, "Buffer not correctly read");

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0x84 with user 4 and seat 0
    *       ==> shared user value accessible by A GROUP (user 4 and seat 0)
    */
   ret = pclKeyReadData(0x84, "links/last_link",           2, 0, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "CACHE_ /last_exit/brooklyn", strlen((char*)buffer)) == 0, "Buffer not correctly read");

   memset(buffer, 0, READ_SIZE);

   /**
    * Logical DB ID: 0x84 with user 2 and seat 1
    *       ==> local merge value
    */
   ret = pclKeyReadData(0x84, "links/last_link",           2, 1, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "CACHE_ /last_exit/queens", strlen((char*)buffer)) == 0, "Buffer not correctly read");
}
END_TEST



/**
 * Test the key value  h a n d l e  interface using different logicalDB id's, users and seats
 * Each resource below has an entry in the resource configuration table where
 * the storage location (cached or write through) and type (e.g. custom) has bee configured.
 */
START_TEST (test_GetDataHandle)
{
   int ret = 0, handle = 0, handle2 = 0, handle3 = 0, handle4 = 0, size = 0;
   unsigned char buffer[READ_SIZE];
   struct tm *locTime;
   time_t t = time(0);

   char sysTimeBuffer[128];
   memset(buffer, 0, READ_SIZE);

   locTime = localtime(&t);

   snprintf(sysTimeBuffer, 128, "TimeAndData: \"%s %d.%d.%d - %d:%.2d:%.2d Uhr\"", dayOfWeek[locTime->tm_wday], locTime->tm_mday, locTime->tm_mon, (locTime->tm_year+1900),
                                                                  locTime->tm_hour, locTime->tm_min, locTime->tm_sec);


   // open handle ---------------------------------------------------
   /**
    * Logical DB ID: 0xFF with user 0 and seat 0
    *       ==> local value accessible by ALL USERS (user 0, seat 0)
    */
   handle = pclKeyHandleOpen(0xFF, "posHandle/last_position", 0, 0);
   fail_unless(handle >= 0, "Failed to open handle ==> /posHandle/last_position");

   ret = pclKeyHandleReadData(handle, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "WT_ H A N D L E: +48° 10' 38.95\", +8° 44' 39.06\"", ret-1) == 0, "Buffer not correctly read");

   size = pclKeyHandleGetSize(handle);
   fail_unless(size = strlen("WT_ H A N D L E: +48° 10' 38.95\", +8° 44' 39.06\""));
   // ---------------------------------------------------------------------------------------------


   // open handle ---------------------------------------------------
   /**
    * Logical DB ID: 0xFF with user 3 and seat 2
    *       ==> local USER value (user 3, seat 2)
    */
   handle2 = pclKeyHandleOpen(0xFF, "statusHandle/open_document", 3, 2);
   fail_unless(handle2 >= 0, "Failed to open handle /statusHandle/open_document");

   size = pclKeyHandleWriteData(handle2, (unsigned char*)sysTimeBuffer, strlen(sysTimeBuffer));
   fail_unless(size = strlen(sysTimeBuffer));
   // close
   ret = pclKeyHandleClose(handle2);
   // ---------------------------------------------------------------------------------------------


   // open handle ---------------------------------------------------
   /**
    * Logical DB ID: 0xFF with user 0 and seat 0
    *       ==> local value accessible by ALL USERS (user 0, seat 0)
    */
   memset(buffer, 0, READ_SIZE);
   handle4 = pclKeyHandleOpen(0xFF, "language/country_code", 0, 0);
   fail_unless(handle4 >= 0, "Failed to open handle /language/country_code");

   ret = pclKeyHandleReadData(handle4, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, "Custom plugin -> plugin_get_data_handle", -1) == 0, "Buffer not correctly read");

   size = pclKeyHandleGetSize(handle4);
   fail_unless(size = strlen("Custom plugin -> plugin_get_data_handle"));

   ret = pclKeyHandleWriteData(handle4, (unsigned char*)"Only dummy implementation behind custom library", READ_SIZE);
   // ---------------------------------------------------------------------------------------------


   // open handle ---------------------------------------------------
   /**
    * Logical DB ID: 0xFF with user 3 and seat 2
    *       ==> local USER value (user 3, seat 2)
    */
   handle3 = pclKeyHandleOpen(0xFF, "statusHandle/open_document", 3, 2);
   fail_unless(handle3 >= 0, "Failed to open handle /statusHandle/open_document");

   ret = pclKeyHandleReadData(handle3, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, sysTimeBuffer, strlen(sysTimeBuffer)) == 0, "Buffer not correctly read");

   size = pclKeyHandleGetSize(handle3);
   fail_unless(size = strlen(sysTimeBuffer));
   // ---------------------------------------------------------------------------------------------


   // close handle
   ret = pclKeyHandleClose(handle);
   ret = pclKeyHandleClose(handle3);
   ret = pclKeyHandleClose(handle4);
}
END_TEST



/*
 * Write data to a key using the key interface.
 * First write data to different keys and after
 * read the data for verification.
 */
START_TEST(test_SetData)
{
   int ret = 0;
   unsigned char buffer[READ_SIZE];
   char write1[READ_SIZE];
   char write2[READ_SIZE];
   char sysTimeBuffer[256];

   struct tm *locTime;
   time_t t = time(0);

   locTime = localtime(&t);
   memset(buffer, 0, READ_SIZE);
   memset(write1, 0, READ_SIZE);
   memset(write2, 0, READ_SIZE);

   // write data
   snprintf(sysTimeBuffer, 128, "\"%s %d.%d.%d - %d:%.2d:%.2d Uhr\"", dayOfWeek[locTime->tm_wday], locTime->tm_mday, locTime->tm_mon, (locTime->tm_year+1900),
                                                                 locTime->tm_hour, locTime->tm_min, locTime->tm_sec);

   /**
    * Logical DB ID: 0xFF with user 1 and seat 2
    *       ==> local USER value (user 1, seat 2)
    * Resource ID: 69
    */
   ret = pclKeyWriteData(0xFF, "69", 1, 2, (unsigned char*)sysTimeBuffer, strlen(sysTimeBuffer));
   fail_unless(ret == strlen(sysTimeBuffer), "Wrong write size");

   snprintf(write1, 128, "%s %s", "/70",  sysTimeBuffer);
   /**
    * Logical DB ID: 0xFF with user 1 and seat 2
    *       ==> local USER value (user 1, seat 2)
    * Resource ID: 70
    */
   ret = pclKeyWriteData(0xFF, "70", 1, 2, (unsigned char*)write1, strlen(write1));
   fail_unless(ret == strlen(write1), "Wrong write size");

   snprintf(write2, 128, "%s %s", "/key_70",  sysTimeBuffer);
   /**
    * Logical DB ID: 0xFF with user 1 and seat 2
    *       ==> local USER value (user 1, seat 2)
    * Resource ID: key_70
    */
   ret = pclKeyWriteData(0xFF, "key_70", 1, 2, (unsigned char*)write2, strlen(write2));
   fail_unless(ret == strlen(write2), "Wrong write size");



   /*
    * now read the data written in the previous steps to the keys
    * and verify data has been written correctly.
    */
   memset(buffer, 0, READ_SIZE);

   ret = pclKeyReadData(0xFF, "69", 1, 2, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, sysTimeBuffer, strlen(sysTimeBuffer)) == 0, "Buffer not correctly read");
   fail_unless(ret == strlen(sysTimeBuffer), "Wrong read size");

   memset(buffer, 0, READ_SIZE);

   ret = pclKeyReadData(0xFF, "70", 1, 2, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, write1, strlen(write1)) == 0, "Buffer not correctly read");
   fail_unless(ret == strlen(write1), "Wrong read size");

   memset(buffer, 0, READ_SIZE);

   ret = pclKeyReadData(0xFF, "key_70", 1, 2, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, write2, strlen(write2)) == 0, "Buffer not correctly read");
   fail_unless(ret == strlen(write2), "Wrong read size");

}
END_TEST



/**
 * Write data to a key using the key interface.
 * The key is not in the persistence resource table.
 * The key sill then be stored to the location local and cached.
 */
START_TEST(test_SetDataNoPRCT)
{
   int ret = 0;
   unsigned char buffer[READ_SIZE];
   struct tm *locTime;
   time_t t = time(0);

   char sysTimeBuffer[128];
   memset(buffer, 0, READ_SIZE);

   locTime = localtime(&t);

   snprintf(sysTimeBuffer, 128, "TimeAndData: \"%s %d.%d.%d - %d:%.2d:%.2d Uhr\"", dayOfWeek[locTime->tm_wday], locTime->tm_mday, locTime->tm_mon, (locTime->tm_year+1900),
                                                                  locTime->tm_hour, locTime->tm_min, locTime->tm_sec);

   /**
    * Logical DB ID: 0xFF with user 1 and seat 2
    *       ==> local USER value (user 1, seat 2)
    */
   ret = pclKeyWriteData(0xFF, "NoPRCT", 1, 2, (unsigned char*)sysTimeBuffer, strlen(sysTimeBuffer));
   fail_unless(ret == strlen(sysTimeBuffer), "Wrong write size");
   printf("Write Buffer : %s\n", sysTimeBuffer);

   // read data again and and verify datat has been written correctly
   memset(buffer, 0, READ_SIZE);

   ret = pclKeyReadData(0xFF, "NoPRCT", 1, 2, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, sysTimeBuffer, strlen(sysTimeBuffer)) == 0, "Buffer not correctly read");
   fail_unless(ret == strlen(sysTimeBuffer), "Wrong read size");
   printf("read buffer  : %s\n", buffer);

}
END_TEST



/*
 * Test the key interface.
 * Read the size of a key.
 */
START_TEST(test_GetDataSize)
{
   int size = 0;

   /**
    * Logical DB ID: 0xFF with user 3 and seat 2
    *       ==> local USER value (user 3, seat 2)
    */
   size = pclKeyGetSize(0xFF, "status/open_document", 3, 2);
   fail_unless(size == strlen("WT_ /var/opt/user_manual_climateControl.pdf"), "Invalid size");


   /**
    * Logical DB ID: 0x84 with user 2 and seat 1
    *       ==> shared user value accessible by A GROUP (user 2 and seat 1)
    */
   size = pclKeyGetSize(0x84, "links/last_link", 2, 1);
   fail_unless(size == strlen("CACHE_ /last_exit/queens"), "Invalid size");
}
END_TEST


/*
 * Delete a key using the key value interface.
 * First read a from a key, the delte the key
 * and then try to read again. The Last read must fail.
 */
START_TEST(test_DeleteData)
{
   int rval = 0;
   unsigned char buffer[READ_SIZE];

   // read data from key
   rval = pclKeyReadData(0xFF, "key_70", 1, 2, buffer, READ_SIZE);
   fail_unless(rval != EPERS_NOKEY, "Read form key key_70 fails");

   // delete key
   rval = pclKeyDelete(0xFF, "key_70", 1, 2);
   fail_unless(rval == 0, "Failed to delete key");

   // after deleting the key, reading from key must fail now!
   rval = pclKeyReadData(0xFF, "key_70", 1, 2, buffer, READ_SIZE);
   fail_unless(rval == EPERS_NOKEY, "Read form key key_70 works, but should fail");



   // read data from key
   rval = pclKeyReadData(0xFF, "70", 1, 2, buffer, READ_SIZE);
   fail_unless(rval != EPERS_NOKEY, "Read form key 70 fails");

   // delete key
   rval = pclKeyDelete(0xFF, "70", 1, 2);
   fail_unless(rval == 0, "Failed to delete key");

   // after deleting the key, reading from key must fail now!
   rval = pclKeyReadData(0xFF, "70", 1, 2, buffer, READ_SIZE);
   fail_unless(rval == EPERS_NOKEY, "Read form key 70 works, but should fail");
}
END_TEST



/*
 * Test the file interface:
 * - open file
 * - read / write
 * - remove file
 * - map file
 * - get size
 */
START_TEST(test_DataFile)
{
   int fd = 0, i = 0, idx = 0;
   int size = 0, ret = 0;
   int writeSize = 16*1024;
   unsigned char buffer[READ_SIZE];
   const char* refBuffer = "/Data/mnt-wt/lt-persistence_client_library_test/user/1/seat/1/media";
   char* writeBuffer;
   char* fileMap = NULL;
   writeBuffer = malloc(writeSize);


   // fill buffer a sequence
   for(i = 0; i<(writeSize/8); i++)
   {
      writeBuffer[idx++] = 'A';
      writeBuffer[idx++] = 'B';
      writeBuffer[idx++] = 'C';
      writeBuffer[idx++] = ' ';
      writeBuffer[idx++] = 'D';
      writeBuffer[idx++] = 'E';
      writeBuffer[idx++] = 'F';
      writeBuffer[idx++] = ' ';
   }
   memset(buffer, 0, READ_SIZE);

   // create file
   fd = open("/Data/mnt-wt/lt-persistence_client_library_test/user/1/seat/1/media/mediaDBWrite.db",
             O_CREAT|O_RDWR|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
   close(fd);

   // open ----------------------------------------------------------
   fd = pclFileOpen(0xFF, "media/mediaDB.db", 1, 1);
   fail_unless(fd != -1, "Could not open file ==> /media/mediaDB.db");

   size = pclFileGetSize(fd);
   fail_unless(size == 68, "Wrong file size");

   size = pclFileReadData(fd, buffer, READ_SIZE);
   fail_unless(strncmp((char*)buffer, refBuffer, strlen(refBuffer)) == 0, "Buffer not correctly read");
   fail_unless(size == (strlen(refBuffer)+1), "Wrong size returned");      // strlen + 1 ==> inlcude cr/lf

   ret = pclFileClose(fd);
   fail_unless(ret == 0, "Failed to close file");


   // open ----------------------------------------------------------
   fd = pclFileOpen(0xFF, "media/mediaDBWrite.db", 1, 1);
   fail_unless(fd != -1, "Could not open file ==> /media/mediaDBWrite.db");

   size = pclFileWriteData(fd, writeBuffer, strlen(writeBuffer));
   fail_unless(size == strlen(writeBuffer), "Failed to write data");

   ret = pclFileClose(fd);
   fail_unless(ret == 0, "Failed to close file");


   // remove ----------------------------------------------------------
   ret = pclFileRemove(0xFF, "media/mediaDBWrite.db", 1, 1);
   fail_unless(ret == 0, "File can't be removed ==> /media/mediaDBWrite.db");

   fd = pclFileOpen(0xFF, "media/mediaDBWrite.db", 1, 1);
   fail_unless(fd != -1, "File can't be opend ==> /media/mediaDBWrite.db");

   ret = pclFileRemove(0xFF, "media/mediaDBWrite.db", 1, 1);
   fail_unless(ret == 0, "File can't be removed ==> /media/mediaDBWrite.db");

   ret = pclFileClose(fd);
   fail_unless(ret == 0, "Failed to close file");


   // map file ------------------------------------------------------
   fd = pclFileOpen(0xFF, "media/mediaDB.db", 1, 1);

   size = pclFileGetSize(fd);
   pclFileMapData(fileMap, size, 0, fd);
   fail_unless(fileMap != MAP_FAILED, "Failed to map file");

   ret = pclFileUnmapData(fileMap, size);
   fail_unless(ret != -1, "Failed to unmap file");

   // negative test
   size = pclFileGetSize(1024);
   fail_unless(ret == 0, "Got size, but should not");

   ret = pclFileClose(fd);
   fail_unless(ret == 0, "Failed to close file");

   free(writeBuffer);
}
END_TEST



/*
 * The the handle function of the key and file interface.
 */
START_TEST(test_DataHandle)
{
   int handle1 = 0, handle2 = 0;
   int ret = 0;

   // test file handles
   handle1 = pclFileOpen(0xFF, "media/mediaDB.db", 1, 1);
   fail_unless(handle1 != -1, "Could not open file ==> /media/mediaDB.db");

   ret = pclFileClose(handle1);
   fail_unless(handle1 != -1, "Could not closefile ==> /media/mediaDB.db");

   ret = pclFileClose(1024);
   fail_unless(ret == -1, "Could close file, but should not!!");

   ret = pclFileClose(17);
   fail_unless(ret == -1, "Could close file, but should not!!");



   // test key handles
   handle2 = pclKeyHandleOpen(0xFF, "statusHandle/open_document", 3, 2);
   fail_unless(handle2 >= 0, "Failed to open handle /statusHandle/open_document");

   ret = pclKeyHandleClose(handle2);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(1024);
   fail_unless(ret == -1, "Could close, but should not!!");
}
END_TEST



/*
 * Extended key handle test.
 * Test have been created after a bug in the key handle function occured.
 */
START_TEST(test_DataHandleOpen)
{
   int hd1 = -2, hd2 = -2, hd3 = -2, hd4 = -2, hd5 = -2, hd6 = -2, hd7 = -2, hd8 = -2, hd9 = -2, ret = 0;

   // open handles ----------------------------------------------------
   hd1 = pclKeyHandleOpen(0xFF, "posHandle/last_position1", 0, 0);
   fail_unless(hd1 == 1, "Failed to open handle ==> /posHandle/last_position1");

   hd2 = pclKeyHandleOpen(0xFF, "posHandle/last_position2", 0, 0);
   fail_unless(hd2 == 2, "Failed to open handle ==> /posHandle/last_position2");

   hd3 = pclKeyHandleOpen(0xFF, "posHandle/last_position3", 0, 0);
   fail_unless(hd3 == 3, "Failed to open handle ==> /posHandle/last_position3");

   // close handles ---------------------------------------------------
   ret = pclKeyHandleClose(hd1);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd2);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd3);
   fail_unless(ret != -1, "Failed to close handle!!");

   // open handles ----------------------------------------------------
   hd4 = pclKeyHandleOpen(0xFF, "posHandle/last_position4", 0, 0);
   fail_unless(hd4 == 3, "Failed to open handle ==> /posHandle/last_position4");

   hd5 = pclKeyHandleOpen(0xFF, "posHandle/last_position5", 0, 0);
   fail_unless(hd5 == 2, "Failed to open handle ==> /posHandle/last_position5");

   hd6 = pclKeyHandleOpen(0xFF, "posHandle/last_position6", 0, 0);
   fail_unless(hd6 == 1, "Failed to open handle ==> /posHandle/last_position6");

   hd7 = pclKeyHandleOpen(0xFF, "posHandle/last_position7", 0, 0);
   fail_unless(hd7 == 4, "Failed to open handle ==> /posHandle/last_position7");

   hd8 = pclKeyHandleOpen(0xFF, "posHandle/last_position8", 0, 0);
   fail_unless(hd8 == 5, "Failed to open handle ==> /posHandle/last_position8");

   hd9 = pclKeyHandleOpen(0xFF, "posHandle/last_position9", 0, 0);
   fail_unless(hd9 == 6, "Failed to open handle ==> /posHandle/last_position9");

   // close handles ---------------------------------------------------
   ret = pclKeyHandleClose(hd4);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd5);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd6);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd7);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd8);
   fail_unless(ret != -1, "Failed to close handle!!");

   ret = pclKeyHandleClose(hd9);
   fail_unless(ret != -1, "Failed to close handle!!");
}
END_TEST



/**
 * Test for  i n t e r n a l  structures.
 * Test the cursor functions.
 */
START_TEST(test_Cursor)
{
   int handle = -1, rval = 0, size = 0, handle1 = 0;
   char bufferKeySrc[READ_SIZE];
   char bufferDataSrc[READ_SIZE];
   char bufferKeyDst[READ_SIZE];
   char bufferDataDst[READ_SIZE];

   memset(bufferKeySrc, 0, READ_SIZE);
   memset(bufferDataSrc, 0, READ_SIZE);

   memset(bufferKeyDst, 0, READ_SIZE);
   memset(bufferDataDst, 0, READ_SIZE);

   // create cursor
   handle = pers_db_cursor_create("/Data/mnt-c/lt-persistence_client_library_test/cached.itz");

   fail_unless(handle != -1, "Failed to create cursor!!");

   // create cursor
   handle1 = pers_db_cursor_create("/Data/mnt-c/lt-persistence_client_library_test/wt.itz");

   fail_unless(handle1 != -1, "Failed to create cursor!!");

   do
   {
      memset(bufferKeySrc, 0, READ_SIZE);
      memset(bufferDataSrc, 0, READ_SIZE);
      memset(bufferKeyDst, 0, READ_SIZE);
      memset(bufferDataDst, 0, READ_SIZE);

      // get key
      rval = pers_db_cursor_get_key(handle, bufferKeySrc, 128);
      fail_unless(rval != -1, "Cursor failed to get key!!");
      // get data
      rval = pers_db_cursor_get_data(handle, bufferDataSrc, 128);
      fail_unless(rval != -1, "Cursor failed to get data!!");
      // get size
      size = pers_db_cursor_get_data_size(handle);
      fail_unless(size != -1, "Cursor failed to get size!!");
      //printf("1. Key: %s | Data: %s » Size: %d \n", bufferKeySrc, bufferDataSrc, size);

      // get key
      rval = pers_db_cursor_get_key(handle1, bufferKeyDst, 128);
      fail_unless(rval != -1, "Cursor failed to get key!!");
      // get data
      rval = pers_db_cursor_get_data(handle1, bufferDataDst, 128);
      fail_unless(rval != -1, "Cursor failed to get data!!");
      // get size
      size = pers_db_cursor_get_data_size(handle1);
      fail_unless(size != -1, "Cursor failed to get size!!");
      //printf("  2. Key: %s | Data: %s » Size: %d \n", bufferKeyDst, bufferDataDst, size);
   }
   while( (pers_db_cursor_next(handle) == 0) && (pers_db_cursor_next(handle1) == 0) ); // next cursor

   // destory cursor
   rval = pers_db_cursor_destroy(handle);
   fail_unless(rval != -1, "Failed to destroy cursor!!");

   rval = pers_db_cursor_destroy(handle1);
   fail_unless(rval != -1, "Failed to destroy cursor!!");
}
END_TEST



static Suite * persistencyClientLib_suite()
{
   Suite * s  = suite_create("Persistency client library");

   TCase * tc_persGetData = tcase_create("GetData");
   tcase_add_test(tc_persGetData, test_GetData);

   TCase * tc_persSetData = tcase_create("SetData");
   tcase_add_test(tc_persSetData, test_SetData);

   TCase * tc_persSetDataNoPRCT = tcase_create("SetDataNoPRCT");
   tcase_add_test(tc_persSetDataNoPRCT, test_SetDataNoPRCT);

   TCase * tc_persGetDataSize = tcase_create("GetDataSize");
   tcase_add_test(tc_persGetDataSize, test_GetDataSize);

   TCase * tc_persDeleteData = tcase_create("DeleteData");
   tcase_add_test(tc_persDeleteData, test_DeleteData);

   TCase * tc_persGetDataHandle = tcase_create("GetDataHandle");
   tcase_add_test(tc_persGetDataHandle, test_GetDataHandle);

   TCase * tc_persDataHandle = tcase_create("DataHandle");
   tcase_add_test(tc_persDataHandle, test_DataHandle);

   TCase * tc_persDataHandleOpen = tcase_create("DataHandleOpen");
   tcase_add_test(tc_persDataHandleOpen, test_DataHandleOpen);

   TCase * tc_persDataFile = tcase_create("DataFile");
   tcase_add_test(tc_persDataFile, test_DataFile);

   TCase * tc_Cursor = tcase_create("Cursor");
   tcase_add_test(tc_Cursor, test_Cursor);

   suite_add_tcase(s, tc_persGetData);
   suite_add_tcase(s, tc_persSetData);
   suite_add_tcase(s, tc_persSetDataNoPRCT);
   suite_add_tcase(s, tc_persGetDataSize);
   suite_add_tcase(s, tc_persDeleteData);
   suite_add_tcase(s, tc_persGetDataHandle);
   suite_add_tcase(s, tc_persDataHandle);
   suite_add_tcase(s, tc_persDataHandleOpen);
   suite_add_tcase(s, tc_persDataFile);
   suite_add_tcase(s, tc_Cursor);

   return s;
}




int main(int argc, char *argv[])
{
   int nr_failed = 0;

   Suite * s = persistencyClientLib_suite();
   SRunner * sr = srunner_create(s);
   srunner_run_all(sr, CK_VERBOSE);
   nr_failed = srunner_ntests_failed(sr);

   srunner_free(sr);
   return (0==nr_failed)?EXIT_SUCCESS:EXIT_FAILURE;

}

