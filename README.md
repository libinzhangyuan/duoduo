

### Key value database for const data on SSD disk.
* This nosql db store data on a SSD disk. It's using little memory for data cache.
* It's for data that do not change frequently.
* The common usage of DuoDb is initialize the data in your app initialize phase. Then using it in running phase.
* It do not support transaction because changing data when running server is not our feature.


#### Warning: Changing data frequently will damage your SSD disk. Add data is ok.
#### One change per second will be OK. But 100 changes per second may let SSD disk dying in few months.


### Config
*  Get block size of your ssd.
*  sudo /sbin/dumpe2fs /dev/hda2 | grep 'Block size'
*  then config the ssd block size.

### Compile static lib for production using.
*  $ cd essential
*  $ vim Makefile : open the line: CPPFLAGS  = -Wall
*  $ make
*  $ cd ../ddlib
*  $ vim Makefile : open the line:  CPPFLAGS  = -Wall -I ../essential
*  $ make
*  then coping the ddlib.a to your production lib dir.

### Compile for cppunit test

#### Install cppunit
*   download from http://nchc.dl.sourceforge.net/project/cppunit/cppunit/1.12.1/cppunit-1.12.1.tar.gz
*   unpack it. cp cppunit-1.12.1 folder to the folder of duoduo project. 
*   $ cd cppunit-1.12.1
*   $ ./configure
*   $ make

#### Compile
*   $ cd essential
*   $ vim Makefile : open the line: CPPFLAGS  = -Wall -D \_UTEST\_
*   $ make
*   $ cd ddlib
*   $ vim Makefile : open the line:  CPPFLAGS  = -Wall -I ../essential -D \_UTEST\_
*   $ make
*   $ cd ..
*   $ vim Makefile : open the line: CPPFLAGS  = -Wall -I essential -I ddlib -D \_UTEST\_
*   $ make
*   $ ./duoduo\_utest


### contribute

