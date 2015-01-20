

### Key value database for const data on SSD disk.
This nosql db store data on a SSD disk. It's using little memory for data cache.
It's for data that do not change frequently.
The common usage of DuoDb is initialize the data in your app initialize phase. Then using it in running phase.
It do not support transaction because changing data when running server is not our feature.


Warning: Changing data frequently will damage your SSD disk. Add data is ok.
One change per second will be OK. But 100 changes per second may let SSD disk dying in few months.


### Config
1. Get block size of your ssd.
sudo /sbin/dumpe2fs /dev/hda2 | grep 'Block size'
then config the ssd block size.



### contribute
#### Install cppunit
   download from http://nchc.dl.sourceforge.net/project/cppunit/cppunit/1.12.1/cppunit-1.12.1.tar.gz
   unpack it. 
   $ cd cppunit-1.12.1
   $ ./configure
   $ make
   $ make check
   $ make install
   $ cp cppunit /usr/include/
