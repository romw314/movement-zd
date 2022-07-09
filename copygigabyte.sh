#!/bin/sh
set -x
sudo /root/mount.sh
[ -z "$NO_BEST_COPY" ] && exec ./best-copy.sh
ROMLOCAL="/mnt/gigabyte/romanko/.local"
ROMFG="/home/romanko/.local"
if [ -n "$SSH_COPY" -o -n "$DEFAULT_COPY" ]; then
	exit 24
	ssh romanko@gigabyte.lan mkdir -p "$ROMFG/share/man" "$ROMFG/bin"
	for s in man/man*; do
		for p in $s/*.*; do
			echo "@$p"
			ssh romanko@gigabyte.lan sh -c "cat > '$ROMFG/share/$p'" < "$p"
		done
	done
	for e in build/*; do
		echo "@$e"
		if readlink "$e" &>/dev/null; then
			ssh romanko@gigabyte.lan sh -c "cat ln -s '$(basename $(realpath "$e")) '$ROMFG/bin/$(echo $e | sed "s/^.*?\\///")'" < "$e"
		else
			ssh romanko@gigabyte.lan sh -c "cat > '$ROMFG/bin/$(basename "$e")'" < "$e"
		fi
	done
else
	mkdir -p "$ROMLOCAL/share/man" "$ROMLOCAL/bin"
	cp -r "man" "$ROMLOCAL/share"
	cp "build/movement-zd-stable" "$ROMLOCAL/bin"
fi
ssh romanko@gigabyte.lan ln -srf "$ROMFG/bin/movement-zd-stable" "$ROMFG/bin/movement-zd"
