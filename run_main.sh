#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib
./DemoManMonitor -hmm ./models/en-us/ -dict ./models/cmudict-en-us.dict -lm ./models/en-us.lm.bin -logfn /dev/null
