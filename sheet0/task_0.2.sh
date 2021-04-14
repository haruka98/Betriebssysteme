#!/bin/bash

unzip sheet0.zip

cp -r uni uni_copy

find uni_copy/studierende -name "rotbauer.dat" -exec rm {} \;

find uni_copy/studierende -name "mustermann.dat" -execdir mv {} "musterfrau.dat" \;

find uni_copy/studierende -name "*.copy" -exec rm {} \;

rm -r uni

mv uni_copy uni
