#!/bin/sh

[ -x /usr/bin/apds9300 ] || exit 0

case "$1" in
  start)
	/usr/bin/apds9300 1 &
	;;
  stop)
	ps | grep "apds9300 1" | grep -v grep | awk '{print $1}' | xargs kill
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