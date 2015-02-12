OLD_PWD="$( pwd )"

rm duoduo_utest
cd essential/ && make && cd ../ddlib/ && make && cd .. && make

# restore old path.
cd $OLD_PWD
