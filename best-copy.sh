#!/bin/sh
set -x
export NO_BEST_COPY=1
unset SSH_COPY
unset DEFAULT_COPY
sudo /root/mount.sh
MY_RH=/mnt/gigabyte/romanko/.local
rm -rf $MY_RH/bin/movement-zd $MY_RH/bin/movement-zd-stable
exec ./copygigabyte.sh
