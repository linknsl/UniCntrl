#!/bin/sh

[ -x /usr/bin/usb_charging ] || exit 0

case "$1" in
  start)
	/usr/bin/usb_charging 0 &
	;;
  stop)
	ps | grep "usb_charging 0" | grep -v grep | awk '{print $1}' | xargs kill
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