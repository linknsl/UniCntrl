#!/bin/sh

[ -x /usr/bin/sht21 ] || exit 0

case "$1" in
  start)
	/usr/bin/sht21 0 &
	;;
  stop)
	ps | grep "sht21 0" | grep -v grep | awk '{print $1}' | xargs kill
	;;
  restart|reload)
	$0" stop
	$0" start
	;;
  *)
	echo "Usage: $0 {start|stop|restart}"
	exit 1
esac

exit $?