#!/bin/bash

dd if=/dev/urandom of=testfile bs=1M count=1
ln testfile testlink

dd if=/dev/zero of=testfile0 bs=1M count=1
ln testfile0 testlink0