#compile third-src
set -x
CUR_PATH=`pwd`
#1.compile third-src

#compile clog
cd $CUR_PATH/third-src/c-log/src && make && cp -r output/* $CUR_PATH/third-64/clog

#compile fcgi
cd $CUR_PATH/third-src
tar -zxvf fcgi-2.4.1.tar.gz
cd fcgi-2.4.1
./configure --prefix=$CUR_PATH/third-64/fcgi
make && make install

#2.compile common
cd $CUR_PATH/common/base
make
cd $CUR_PATH/common/cwf
make
cd $CUR_PATH/service/web-frame
make clean && make
cd $CUR_PATH/service/web-plugins
make clean && make

