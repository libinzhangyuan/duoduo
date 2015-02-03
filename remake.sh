OLD_PWD="$( pwd )"

rm test/BlockStructureTest.o
cd essential/ && make && cd ../ddlib/ && make && cd .. && make

# restore old path.
cd $OLD_PWD
