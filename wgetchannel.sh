#!/bin/bash

mkdir -p /tmp/rissa
rm /tmp/rissa/$1
wget "https://www.youtube.com/$1" -O /tmp/rissa/$1 -q
