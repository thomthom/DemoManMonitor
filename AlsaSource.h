// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// ALSA-based audio source.

#pragma once

#include <chrono>
#include <stdexcept>

#include <alsa/asoundlib.h>

#include "AlsaError.h"
#include "AudioSource.h"

class AlsaSource: public AudioSource {
public:
	AlsaSource();
	~AlsaSource();
	void open(const std::string& hw, const int rate, const int channels, const snd_pcm_format_t format);
	virtual bool record(std::vector<uint8_t>& buffer);
	virtual void pause();
	virtual void resume();

private:
	snd_pcm_t* _device;
	ssize_t _formatSize;
	size_t _record_calls;
	std::chrono::steady_clock::time_point _last_record;

};
