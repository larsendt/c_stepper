#!/bin/sh

find /sys/class/gpio -type f | sudo xargs chmod 666
find /sys/devices/virtual/gpio -type f | sudo xargs chmod 666
