#!/bin/bash

example_file=$1

if [ -z "$example_file" ]
	then
	echo "Put a config file to Rehtse as first parameter. Select one of the following"
	ls  | grep json
	exit 1
fi
cp nftables_queue_everything.conf /etc/nftables.conf
systemctl restart nftables

rm config.json
ln -s $example_file config.json
../Debug/RehtSe