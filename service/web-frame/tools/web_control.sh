#!/usr/bin
if [ $# -ne 1 ];then
	echo "run as:sh web_control.sh [start/stop]"
fi
if [ $1 = "start" ];then
	spawn-fcgi -a 127.0.0.1 -p 9000 -C 25 -u yxy -f ./service_main
	echo "start successfully..."
elif [ $1 = "stop" ];then
	killall service_main
	echo "kill successfully..."
else
	echo "run as:sh web_control.sh [start/stop"
fi
