#!/bin/bash

for i in `whereis $1`; do
	if [[ $i == *man* ]]; then
		xx="`gzip -d < $i`"
		xx="${xx/$'\n'/""}"
		xx="${xx/.br/$'\n'}"
		less <<< "$xx"
	fi
done
