# Notes on building DemoManMonitor for RPi3

## Install dependencies

### apt-get

    sudo apt-get install build-essential autoconf libtool automake bison swig python-dev libasound2-dev gcc-4.7 g++-4.7 libasound2-dev


### PocketSphinx & SphinxBase

Download latest release. This now include support for keyword spotting.

Rename their extracted folders and ensure they have common parent directory.

Build and install. (Refer to their README files)

If there are errors later when running tests or the app about loading
shared libraries, run `sudo ldconfig` which is needed after manually
installing libraries.

#### Models

The original project describe
different names for the models. PocketSphinx V5 pre-alpha ship with the
following model filenames:

* model/hmm/en_US/hub4wsj_sc_8k  => models/en-us/
* model/lm/en_US/cmu07a.dic      => models/cmudict-en-us.dict
* model/lm/en_US/wsj0vp.5000.DMP => models/en-us.lm.bin 

Additionally `run_main.sh` needs updating:

	#!/bin/bash
	export LD_LIBRARY_PATH=/usr/local/lib
	./DemoManMonitor -hmm ./models/en-us/ -dict ./models/cmudict-en-us.dict -lm ./models/en-us.lm.bin -logfn /dev/null

In the `Makefile` the `run` command also needs to be updated to reflect
the model filenames.


### wiringPi

This appear to be out of the box with RPi3, (August 2017). `gpio` is available.


## Update Source Code

PocketSphinx' API changed. Needed to remove utterance ID from:
* `ps_start_utt`
* `ps_get_hyp`


## Build

    make DemoManMonitor


## Models

`model` folder is different from the hackaday article.

CLs suggest BIN files are the new DMP.
https://github.com/cmusphinx/pocketsphinx/commit/f7bc9e7cc715712fe4b8f8010961b96569da5f85#diff-2bf05019b9bba7ad6ead4e4c193c6373


## Troubleshooting

### ALSA Errors

#### snd_pcm_open

`ALSA ERROR No such file or directory (-2)` can be a result of device
configuration not matching the system.

On my system I had to change `RECORD_HW` to `"plughw:1,0"` and
`PLAYBACK_HW` to `"plughw:0,1"`. Not sure how to determine correct
values. I just used trial and errors.


#### std::exception

This can be wrong path to the models. See comment futher up.
