#!/bin/bash

rm test -rf
mkdir -p  test/bin/dep_so 
mkdir test/bin/plugins
mkdir test/conf -p
cp -r web-frame/output/bin/* ./test/bin/
cp -r web-frame/output/conf test/
cp -r web-frame/output/so/* test/bin/dep_so
cp ../third-64/fcgi/lib/libfcgi.so.0 test/bin/dep_so/
cp  common/base/output/so/* test/bin/dep_so/
cp common/cwf/output/so/* test/bin/dep_so/
cp web-plugins/output/so/* test/bin/plugins
cp web-plugins/output/conf/* test/conf/
cp web-plugins/output/dep_so/* test/bin/dep_so/
cp web-frame/tools/web_control.sh test/bin
