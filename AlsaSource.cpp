// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "AlsaSource.h"

#include <chrono>
#include <iostream>

#include "AppConfig.h"

using namespace std;

AlsaSource::AlsaSource():
	_device(nullptr),
	_formatSize(0),
	_record_calls(0)
{
	_last_record = std::chrono::steady_clock::now();
}

AlsaSource::~AlsaSource() {
	// Close the device if it's open.
	if (_device != nullptr) {
		snd_pcm_close(_device);
	}
}

void AlsaSource::open(const string& hw, const int rate, const int channels, const snd_pcm_format_t format) {
	// Open the device for audio capture.
	ALSA_THROW(snd_pcm_open(&_device, hw.c_str(), SND_PCM_STREAM_CAPTURE, 0));
	// Set device parameters.
	ALSA_THROW(snd_pcm_set_params(_device, format, SND_PCM_ACCESS_RW_INTERLEAVED, channels, rate, 1, 100000));
	// Save the number of bytes per sample for later buffer calculations.
	_formatSize = snd_pcm_format_size(format, 1);
}

bool AlsaSource::record(std::vector<uint8_t>& buffer) {
	++_record_calls;
#if TRACE_TIMINGS
	auto now = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(now - _last_record).count() <<std::endl;
#endif
	// Fill buffer with captured audio.
	snd_pcm_sframes_t size = buffer.size() / _formatSize;
	snd_pcm_sframes_t captured = snd_pcm_readi(_device, buffer.data(), size);
	if (captured != size) {
#if TRACE_OVERFLOW
		std::cout << "[" << _record_calls << "]" << "Buffer: " << buffer.size() << ", Frames requested: " << size << ", Captured: " << captured << std::endl;
		std::cout << "  EBADFD: " << EBADFD << ", EPIPE: " << EPIPE << ", ESTRPIPE: " << ESTRPIPE << std::endl;
#endif
		// Audio buffer underrun, try to recover device and warn that audio wasn't captured.
		ALSA_THROW(snd_pcm_recover(_device, captured, 1));
		return false;
	}
	_last_record = std::chrono::steady_clock::now();
	return true;
}

void AlsaSource::pause() {
	ALSA_THROW(snd_pcm_drop(_device));
}

void AlsaSource::resume() {
	ALSA_THROW(snd_pcm_prepare(_device));
}
