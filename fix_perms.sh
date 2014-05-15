#!/bin/sh

find /sys/class/gpio -type f | xargs chmod 666
find /sys/devices/virtual/gpio -type f | xargs chmod 666
