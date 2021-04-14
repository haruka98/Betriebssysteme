#!/bin/bash

echo $(ls uni/studierende/aiw | wc -l) "AIW Studierende"
echo $(ls uni/studierende/cs | wc -l) "CS Studierende"
echo $(ls uni/studierende/et | wc -l) "ET Studierende"
echo $(ls uni/studierende/iiw | wc -l) "IIW Studierende"

for user in $(grep -E -w "|1" uni/vorlesungen/bs/klausur.txt | sort | cut -d '|' -f 1)
do
	echo $(echo $user | cut -d ',' -f 2) $(echo $user | cut -d ',' -f 1)":" $(find uni/studierende -name "$(echo $user | cut -d ',' -f 1 | tr "[:upper:]" "[:lower:]").dat" -print | cut -d '/' -f 3 | tr "[:lower:]" "[:upper:]")
done

for user in $(cat uni/vorlesungen/bs/klausur.txt)
do
	find uni/studierende -name "$(echo $user | cut -d ',' -f 1 | tr "[:upper:]" "[:lower:]").dat" -exec sh -c "echo $(echo $user | cut -d '|' -f 2) > {}" \;
done
