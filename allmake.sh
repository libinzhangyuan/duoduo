OLD_PWD="$( pwd )"

cd essential/ && make && cd ../ddlib/ && make && cd .. && make

# restore old path.
cd $OLD_PWD
