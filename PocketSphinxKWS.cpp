// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "PocketSphinxKWS.h"

using namespace std;

PocketSphinxKWS::PocketSphinxKWS():
	_ps(nullptr)
{}

PocketSphinxKWS::~PocketSphinxKWS() {
	// Free PocketSphinx resources.
	if (_ps != nullptr) {
		ps_free(_ps);
	}
}

void PocketSphinxKWS::initialize(cmd_ln_t* config, const std::string& keywordFileName) {
	// Initialize PocketSphinx.
	_ps = ps_init(config);
	if (_ps == nullptr) {
		throw runtime_error("Call to ps_init failed.");
	}
	// Turn keyword spotting on.
	if (ps_set_kws(_ps, "keywords", keywordFileName.c_str()) != 0) {
		throw runtime_error("Call to ps_set_kws failed.");
	}
	if (ps_set_search(_ps, "keywords") != 0) {
		throw runtime_error("Call to ps_set_search failed.");
	}
	// Start expecting the first utterance.
	ps_start_utt(_ps);
}

std::string PocketSphinxKWS::process(std::vector<uint8_t>& buffer) {
	if (_ps == nullptr) {
		throw runtime_error("Must call initialize before calling process.");
	}
	// Update speech recognition with data from buffer.
	ps_process_raw(_ps, (short*)buffer.data(), buffer.size()/2, FALSE, FALSE);
	// Check if a keyword was spotted.
	int score = 0;
	const char* hyp = ps_get_hyp(_ps, &score);
	if (hyp != NULL) {
		// Found a keyword, reset the utterance and return the keyword.
		// First copy hypothesis to a string because it will be cleared by starting a new utterance.
		string keyword(hyp);
		ps_end_utt(_ps);
		ps_start_utt(_ps);
		return keyword;
	}
	// No keyword spotted.
	return "";
}

cmd_ln_t* PocketSphinxKWS::parseConfig(int argc, char* argv[]) {
	arg_t extra[] = { POCKETSPHINX_OPTIONS, CMDLN_EMPTY_OPTION };
	return cmd_ln_parse_r(NULL, extra, argc, argv, TRUE);
}
