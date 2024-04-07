#include "audio_stream_mpt.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>

#include <godot_cpp/templates/local_vector.hpp>

#include <libopenmpt/libopenmpt.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

#define MOD_NOT_LOADED_MSG "Module not loaded."
#define INT_NOT_LOADED_MSG "Interactive extension isn't supported. (perhaps the file format does not implement it)"
#define INT2_NOT_LOADED_MSG "Interactive2 extension isn't supported. (perhaps the file format does not implement it)"
#define INT3_NOT_LOADED_MSG "Interactive3 extension isn't supported. (perhaps the file format does not implement it)"

#define CHECK_MOD_LOADED_RET(retval) if (!this->mpt_module) { WARN_PRINT_ED(MOD_NOT_LOADED_MSG); return retval; }
#define CHECK_MOD_LOADED_RETV() if (!this->mpt_module) { WARN_PRINT_ED(MOD_NOT_LOADED_MSG); return; }

#define CHECK_INT_LOADED_RET(retval) CHECK_MOD_LOADED_RET(retval); if (!this->mpt_interactive) { WARN_PRINT_ED(INT_NOT_LOADED_MSG); return retval; }
#define CHECK_INT_LOADED_RETV() CHECK_MOD_LOADED_RETV(); if (!this->mpt_module) { WARN_PRINT_ED(INT_NOT_LOADED_MSG); return; }

#define CHECK_INT2_LOADED_RET(retval) CHECK_MOD_LOADED_RET(retval); if (!this->mpt_interactive) { WARN_PRINT_ED(INT2_NOT_LOADED_MSG); return retval; }
#define CHECK_INT2_LOADED_RETV() CHECK_MOD_LOADED_RETV(); if (!this->mpt_module) { WARN_PRINT_ED(INT2_NOT_LOADED_MSG); return; }

#define CHECK_INT3_LOADED_RET(retval) CHECK_MOD_LOADED_RET(retval); if (!this->mpt_interactive) { WARN_PRINT_ED(INT3_NOT_LOADED_MSG); return retval; }
#define CHECK_INT3_LOADED_RETV() CHECK_MOD_LOADED_RETV(); if (!this->mpt_module) { WARN_PRINT_ED(INT3_NOT_LOADED_MSG); return; }

void AudioStreamPlaybackMPT::_start(double p_from_pos) {
	_seek(p_from_pos);
	active = true;
}

void AudioStreamPlaybackMPT::_stop() {
	active = false;
}

bool AudioStreamPlaybackMPT::_is_playing() const {
	return active;
}

int AudioStreamPlaybackMPT::_get_loop_count() const {
	return 0;
}

double AudioStreamPlaybackMPT::_get_playback_position() const {
	CHECK_MOD_LOADED_RET(0.0);
	return this->mpt_module->get_position_seconds();
}

void AudioStreamPlaybackMPT::_seek(double p_time) {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->set_position_seconds(p_time);
}

void AudioStreamPlaybackMPT::seek(int32_t p_order_position, int32_t p_row) {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->set_position_order_row(p_order_position, p_row);
}

bool AudioStreamPlaybackMPT::is_module_loaded() const {
	return this->mpt_module != nullptr;
}

bool AudioStreamPlaybackMPT::is_interactive_supported() const {
	return this->mpt_interactive != nullptr;
}

bool AudioStreamPlaybackMPT::is_interactive2_supported() const {
	return this->mpt_interactive2 != nullptr;
}

bool AudioStreamPlaybackMPT::is_interactive3_supported() const {
	return this->mpt_interactive3 != nullptr;
}

int32_t AudioStreamPlaybackMPT::get_current_order() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_order();
}

void AudioStreamPlaybackMPT::set_current_order(int32_t order) {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->set_position_order_row(order, 0);
}

int32_t AudioStreamPlaybackMPT::get_current_pattern() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_pattern();
}

int32_t AudioStreamPlaybackMPT::get_current_playing_channels() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_playing_channels();
}

int32_t AudioStreamPlaybackMPT::get_current_row() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_row();
}

void AudioStreamPlaybackMPT::set_current_row(int32_t row) const {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->set_position_order_row(this->mpt_module->get_current_order(), row);
}

float AudioStreamPlaybackMPT::get_current_channel_vu_left(int32_t channel) const {
	CHECK_MOD_LOADED_RET(0.0f);
	return this->mpt_module->get_current_channel_vu_left(channel);
}

float AudioStreamPlaybackMPT::get_current_channel_vu_mono(int32_t channel) const {
	CHECK_MOD_LOADED_RET(0.0f);
	return this->mpt_module->get_current_channel_vu_mono(channel);
}

float AudioStreamPlaybackMPT::get_current_channel_vu_rear_left(int32_t channel) const {
	CHECK_MOD_LOADED_RET(0.0f);
	return this->mpt_module->get_current_channel_vu_rear_left(channel);
}

float AudioStreamPlaybackMPT::get_current_channel_vu_rear_right(int32_t channel) const {
	CHECK_MOD_LOADED_RET(0.0f);
	return this->mpt_module->get_current_channel_vu_rear_right(channel);
}

float AudioStreamPlaybackMPT::get_current_channel_vu_right(int32_t channel) const {
	CHECK_MOD_LOADED_RET(0.0f);
	return this->mpt_module->get_current_channel_vu_right(channel);
}

double AudioStreamPlaybackMPT::get_current_estimated_bpm() const {
	CHECK_MOD_LOADED_RET(0.0);
	return this->mpt_module->get_current_estimated_bpm();
}

void AudioStreamPlaybackMPT::select_subsong(int32_t subsong) {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->select_subsong(subsong);
}

int32_t AudioStreamPlaybackMPT::get_selected_subsong() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_selected_subsong();
}

void AudioStreamPlaybackMPT::set_current_speed(int32_t speed) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_current_speed(speed);
}

int32_t AudioStreamPlaybackMPT::get_current_speed() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_speed();
}

void AudioStreamPlaybackMPT::set_current_tempo(double tempo) {
	CHECK_INT3_LOADED_RETV();
	return this->mpt_interactive3->set_current_tempo2(tempo);
}

double AudioStreamPlaybackMPT::get_current_tempo() const {
	CHECK_MOD_LOADED_RET(0.0);
	return this->mpt_module->get_current_tempo2();
}

void AudioStreamPlaybackMPT::set_tempo_factor(double factor) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_tempo_factor(factor);
}

double AudioStreamPlaybackMPT::get_tempo_factor() const {
	CHECK_INT_LOADED_RET(0.0);
	return this->mpt_interactive->get_tempo_factor();
}

void AudioStreamPlaybackMPT::set_pitch_factor(double factor) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_pitch_factor(factor);
}

double AudioStreamPlaybackMPT::get_pitch_factor() const {
	CHECK_INT_LOADED_RET(0.0);
	return this->mpt_interactive->get_pitch_factor();
}

void AudioStreamPlaybackMPT::set_global_volume(double volume) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_global_volume(volume);
}

double AudioStreamPlaybackMPT::get_global_volume() const {
	CHECK_INT_LOADED_RET(0.0);
	return this->mpt_interactive->get_global_volume();
}

void AudioStreamPlaybackMPT::set_channel_volume(int32_t channel, double volume) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_channel_volume(channel, volume);
}

double AudioStreamPlaybackMPT::get_channel_volume(int32_t channel) const {
	CHECK_INT_LOADED_RET(0.0);
	return this->mpt_interactive->get_channel_volume(channel);
}

void AudioStreamPlaybackMPT::set_channel_mute_status(int32_t channel, bool mute) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_channel_mute_status(channel, mute);
}

bool AudioStreamPlaybackMPT::get_channel_mute_status(int32_t channel) const {
	CHECK_INT_LOADED_RET(false);
	return this->mpt_interactive->get_channel_mute_status(channel);
}

void AudioStreamPlaybackMPT::set_instrument_mute_status(int32_t instrument, bool mute) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->set_instrument_mute_status(instrument, mute);
}

bool AudioStreamPlaybackMPT::get_instrument_mute_status(int32_t instrument) const {
	CHECK_INT_LOADED_RET(false);
	return this->mpt_interactive->get_instrument_mute_status(instrument);
}

int32_t AudioStreamPlaybackMPT::play_note(int32_t instrument, int32_t note, double volume, double panning) {
	CHECK_INT_LOADED_RET(-1);
	return this->mpt_interactive->play_note(instrument, note, volume, panning);
}

void AudioStreamPlaybackMPT::stop_note(int32_t channel) {
	CHECK_INT_LOADED_RETV();
	this->mpt_interactive->stop_note(channel);
}

void AudioStreamPlaybackMPT::note_off(int32_t channel) {
	CHECK_INT2_LOADED_RETV();
	this->mpt_interactive2->note_off(channel);
}

void AudioStreamPlaybackMPT::note_fade(int32_t channel) {
	CHECK_INT2_LOADED_RETV();
	this->mpt_interactive2->note_fade(channel);
}

void AudioStreamPlaybackMPT::set_channel_panning(int32_t channel, double panning) {
	CHECK_INT2_LOADED_RETV();
	this->mpt_interactive2->set_channel_panning(channel, panning);
}

double AudioStreamPlaybackMPT::get_channel_panning(int32_t channel) const {
	CHECK_INT2_LOADED_RET(0.0);
	return this->mpt_interactive2->get_channel_panning(channel);
}

void AudioStreamPlaybackMPT::set_note_finetune(int32_t channel, double finetune) {
	CHECK_INT2_LOADED_RETV();
	this->mpt_interactive2->set_note_finetune(channel, finetune);
}

double AudioStreamPlaybackMPT::get_note_finetune(int32_t channel) const {
	CHECK_INT2_LOADED_RET(0.0);
	return this->mpt_interactive2->get_note_finetune(channel);
}

int32_t AudioStreamPlaybackMPT::_mix(AudioFrame *p_buffer, double p_rate_scale, int32_t p_frames) {
	if (!this->mpt_module) {
		active = false;
		for (int i = 0; i < p_frames; i++)
			p_buffer[i] = AudioFrame{ 0, 0 };
		return 0;
	}

	this->mpt_module->set_repeat_count(base->loop_mode == AudioStreamMPT::LoopMode::LOOP_DISABLED ? 0 : -1);

	double srate = (AudioServer::get_singleton()->get_mix_rate() * p_rate_scale) * AudioServer::get_singleton()->get_playback_speed_scale();

	if (base->stereo) {
		if (this->mpt_module->read_interleaved_stereo((int32_t)srate, p_frames, reinterpret_cast<float*>(p_buffer)) == 0) {
			active = false;
			for (int i = 0; i < p_frames; i++)
				p_buffer[i] = AudioFrame{ 0, 0 };
			return 0;
		}
	} else {
		LocalVector<float> mono;
		mono.resize(p_frames);
		if (this->mpt_module->read((int32_t)srate, p_frames, mono.ptr()) == 0) {
			active = false;
			for (int i = 0; i < p_frames; i++)
				p_buffer[i] = AudioFrame{ 0, 0 };
			return 0;
		}

		for (int i = 0; i < p_frames; i++)
			p_buffer[i] = AudioFrame{ mono[i], mono[i] };
	}
	return p_frames;
}

void AudioStreamPlaybackMPT::_bind_methods() {
	ClassDB::bind_method(D_METHOD("seek", "order_position", "row"), &AudioStreamPlaybackMPT::seek);

	ClassDB::bind_method(D_METHOD("is_module_loaded"), &AudioStreamPlaybackMPT::is_module_loaded);
	ClassDB::bind_method(D_METHOD("is_interactive_supported"), &AudioStreamPlaybackMPT::is_interactive_supported);
	ClassDB::bind_method(D_METHOD("is_interactive2_supported"), &AudioStreamPlaybackMPT::is_interactive2_supported);
	ClassDB::bind_method(D_METHOD("is_interactive3_supported"), &AudioStreamPlaybackMPT::is_interactive3_supported);

	ClassDB::bind_method(D_METHOD("get_current_channel_vu_left", "channel"), &AudioStreamPlaybackMPT::get_current_channel_vu_left);
	ClassDB::bind_method(D_METHOD("get_current_channel_vu_mono", "channel"), &AudioStreamPlaybackMPT::get_current_channel_vu_mono);
	ClassDB::bind_method(D_METHOD("get_current_channel_vu_rear_left", "channel"), &AudioStreamPlaybackMPT::get_current_channel_vu_rear_left);
	ClassDB::bind_method(D_METHOD("get_current_channel_vu_rear_right", "channel"), &AudioStreamPlaybackMPT::get_current_channel_vu_rear_right);
	ClassDB::bind_method(D_METHOD("get_current_channel_vu_right", "channel"), &AudioStreamPlaybackMPT::get_current_channel_vu_right);

	ClassDB::bind_method(D_METHOD("get_current_estimated_bpm"), &AudioStreamPlaybackMPT::get_current_estimated_bpm);

	ClassDB::bind_method(D_METHOD("set_current_order", "order"), &AudioStreamPlaybackMPT::set_current_order);
	ClassDB::bind_method(D_METHOD("get_current_order"), &AudioStreamPlaybackMPT::get_current_order);

	ClassDB::bind_method(D_METHOD("get_current_pattern"), &AudioStreamPlaybackMPT::get_current_pattern);
	ClassDB::bind_method(D_METHOD("get_current_playing_channels"), &AudioStreamPlaybackMPT::get_current_playing_channels);

	ClassDB::bind_method(D_METHOD("set_current_row", "row"), &AudioStreamPlaybackMPT::set_current_row);
	ClassDB::bind_method(D_METHOD("get_current_row"), &AudioStreamPlaybackMPT::get_current_row);

	ClassDB::bind_method(D_METHOD("select_subsong", "subsong"), &AudioStreamPlaybackMPT::select_subsong);
	ClassDB::bind_method(D_METHOD("get_selected_subsong"), &AudioStreamPlaybackMPT::get_selected_subsong);

	ClassDB::bind_method(D_METHOD("set_current_speed", "speed"), &AudioStreamPlaybackMPT::set_current_speed);
	ClassDB::bind_method(D_METHOD("get_current_speed"), &AudioStreamPlaybackMPT::get_current_speed);

	ClassDB::bind_method(D_METHOD("set_current_tempo", "tempo"), &AudioStreamPlaybackMPT::set_current_tempo);
	ClassDB::bind_method(D_METHOD("get_current_tempo"), &AudioStreamPlaybackMPT::get_current_tempo);

	ClassDB::bind_method(D_METHOD("set_tempo_factor", "factor"), &AudioStreamPlaybackMPT::set_tempo_factor);
	ClassDB::bind_method(D_METHOD("get_tempo_factor"), &AudioStreamPlaybackMPT::get_tempo_factor);

	ClassDB::bind_method(D_METHOD("set_pitch_factor", "factor"), &AudioStreamPlaybackMPT::set_pitch_factor);
	ClassDB::bind_method(D_METHOD("get_pitch_factor"), &AudioStreamPlaybackMPT::get_pitch_factor);

	ClassDB::bind_method(D_METHOD("set_global_volume", "volume"), &AudioStreamPlaybackMPT::set_global_volume);
	ClassDB::bind_method(D_METHOD("get_global_volume"), &AudioStreamPlaybackMPT::get_global_volume);

	ClassDB::bind_method(D_METHOD("set_channel_volume", "channel", "volume"), &AudioStreamPlaybackMPT::set_channel_volume);
	ClassDB::bind_method(D_METHOD("get_channel_volume", "channel"), &AudioStreamPlaybackMPT::get_channel_volume);

	ClassDB::bind_method(D_METHOD("set_channel_mute_status", "channel", "mute"), &AudioStreamPlaybackMPT::set_channel_mute_status);
	ClassDB::bind_method(D_METHOD("get_channel_mute_status", "channel"), &AudioStreamPlaybackMPT::get_channel_mute_status);

	ClassDB::bind_method(D_METHOD("set_instrument_mute_status", "instrument", "mute"), &AudioStreamPlaybackMPT::set_instrument_mute_status);
	ClassDB::bind_method(D_METHOD("get_instrument_mute_status", "instrument"), &AudioStreamPlaybackMPT::get_instrument_mute_status);

	ClassDB::bind_method(D_METHOD("play_note", "instrument", "note", "volume", "panning"), &AudioStreamPlaybackMPT::play_note);
	ClassDB::bind_method(D_METHOD("stop_note", "channel"), &AudioStreamPlaybackMPT::stop_note);
	ClassDB::bind_method(D_METHOD("note_off", "channel"), &AudioStreamPlaybackMPT::note_off);
	ClassDB::bind_method(D_METHOD("note_fade", "channel"), &AudioStreamPlaybackMPT::note_fade);

	ClassDB::bind_method(D_METHOD("set_channel_panning", "channel", "panning"), &AudioStreamPlaybackMPT::set_channel_panning);
	ClassDB::bind_method(D_METHOD("get_channel_panning", "channel"), &AudioStreamPlaybackMPT::get_channel_panning);

	ClassDB::bind_method(D_METHOD("set_note_finetune", "channel", "finetune"), &AudioStreamPlaybackMPT::set_note_finetune);
	ClassDB::bind_method(D_METHOD("get_note_finetune", "channel"), &AudioStreamPlaybackMPT::get_note_finetune);
}

AudioStreamPlaybackMPT::AudioStreamPlaybackMPT() {}

AudioStreamPlaybackMPT::~AudioStreamPlaybackMPT() {
	int index = base->open_playback_objects.find(this, 0);
	if (index > 0)
		base->open_playback_objects.remove_at(index);
	if (this->mpt_module) delete mpt_module;
}

/////////////////////

void AudioStreamMPT::set_loop_mode(LoopMode p_loop_mode) {
	this->loop_mode = p_loop_mode;
}

AudioStreamMPT::LoopMode AudioStreamMPT::get_loop_mode() const {
	return this->loop_mode;
}

void AudioStreamMPT::set_stereo(bool p_enable) {
	this->stereo = p_enable;
}

bool AudioStreamMPT::is_stereo() const {
	return this->stereo;
}

double AudioStreamMPT::_get_length() const {
	CHECK_MOD_LOADED_RET(0.0);
	double duration = this->mpt_module->get_duration_seconds();
	if (std::isinf(duration))
		return 0.0;
	return duration;
}

bool AudioStreamMPT::_is_monophonic() const {
	return false;
}

void AudioStreamMPT::set_data(const PackedByteArray& p_data) {
	module_error = Error::OK;

	if (p_data.is_empty())
	{
		if (this->mpt_module) delete mpt_module;
		goto set_empty_module;
	}
	this->data = p_data;

	if (this->mpt_module) delete mpt_module;
	try {
		this->mpt_module = new openmpt::module(this->data.ptr(), this->data.size());
	} catch (openmpt::exception& e) {
		module_error = Error::ERR_PARSE_ERROR;
		goto set_empty_module;
	}

	if (open_playback_objects.is_empty()) return;
	AudioServer::get_singleton()->lock();
	for (AudioStreamPlaybackMPT* playback : open_playback_objects) {
		if (playback->mpt_module) delete playback->mpt_module;
		playback->mpt_module = new openmpt::module_ext(this->data.ptr(), this->data.size());
		playback->mpt_interactive = static_cast<openmpt::ext::interactive*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive_id));
		playback->mpt_interactive2 = static_cast<openmpt::ext::interactive2*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive2_id));
		playback->mpt_interactive3 = static_cast<openmpt::ext::interactive3*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive3_id));
	}
	AudioServer::get_singleton()->unlock();
	return;

set_empty_module:
	this->mpt_module = nullptr;
	this->data = PackedByteArray();

	if (open_playback_objects.is_empty()) return;
	AudioServer::get_singleton()->lock();
	for (AudioStreamPlaybackMPT* playback : open_playback_objects) {
		if (playback->mpt_module) delete playback->mpt_module;
		playback->mpt_module = nullptr;
		playback->mpt_interactive = nullptr;
		playback->mpt_interactive2 = nullptr;
		playback->mpt_interactive3 = nullptr;
	}
	AudioServer::get_singleton()->unlock();
	return;
}

const PackedByteArray& AudioStreamMPT::get_data() const {
	return this->data;
}

void AudioStreamMPT::select_subsong(int32_t subsong) {
	CHECK_MOD_LOADED_RETV();
	this->mpt_module->select_subsong(subsong);
}

int32_t AudioStreamMPT::get_selected_subsong() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_selected_subsong();
}

TypedArray<String> AudioStreamMPT::get_channel_names() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_channel_names())
		result.append(name.c_str());
	return result;
}

TypedArray<String> AudioStreamMPT::get_instrument_names() const {
	CHECK_MOD_LOADED_RET({});
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_instrument_names())
		result.append(name.c_str());
	return result;
}

TypedArray<String> AudioStreamMPT::get_metadata_keys() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_metadata_keys())
		result.append(name.c_str());
	return result;
}

String AudioStreamMPT::get_metadata(String key) const {
	CHECK_MOD_LOADED_RET("");
	return this->mpt_module->get_metadata(key.ascii().get_data()).c_str();
}

Dictionary AudioStreamMPT::get_all_metadata() const {
	CHECK_MOD_LOADED_RET(Dictionary());
	Dictionary result;
	std::vector<std::string> keys = this->mpt_module->get_metadata_keys();
	for (std::string key : keys)
		result[key.c_str()] = this->mpt_module->get_metadata(key).c_str();
	return result;
}

double AudioStreamMPT::get_estimated_bpm() const {
	CHECK_MOD_LOADED_RET(0.0);
	return this->mpt_module->get_current_estimated_bpm();
}

int32_t AudioStreamMPT::get_speed() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_current_speed();
}

double AudioStreamMPT::get_tempo() const {
	CHECK_MOD_LOADED_RET(0.0);
	return this->mpt_module->get_current_tempo2();
}

int32_t AudioStreamMPT::get_num_channels() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_channels();
}

int32_t AudioStreamMPT::get_num_instruments() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_instruments();
}

int32_t AudioStreamMPT::get_num_orders() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_orders();
}

int32_t AudioStreamMPT::get_num_patterns() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_patterns();
}

int32_t AudioStreamMPT::get_num_samples() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_samples();
}

int32_t AudioStreamMPT::get_num_subsongs() const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_num_subsongs();
}

TypedArray<String> AudioStreamMPT::get_order_names() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_order_names())
		result.append(name.c_str());
	return result;
}

int32_t AudioStreamMPT::get_order_pattern(int32_t order) const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_order_pattern(order);
}

TypedArray<String> AudioStreamMPT::get_pattern_names() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_pattern_names())
		result.append(name.c_str());
	return result;
}

int32_t AudioStreamMPT::get_pattern_num_rows(int32_t pattern) const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_pattern_num_rows(pattern);
}

TypedArray<String> AudioStreamMPT::get_sample_names() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_sample_names())
		result.append(name.c_str());
	return result;
}

TypedArray<String> AudioStreamMPT::get_subsong_names() const {
	CHECK_MOD_LOADED_RET(TypedArray<String>());
	TypedArray<String> result;
	for (std::string name : this->mpt_module->get_subsong_names())
		result.append(name.c_str());
	return result;
}

uint8_t AudioStreamMPT::get_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const {
	CHECK_MOD_LOADED_RET(0);
	return this->mpt_module->get_pattern_row_channel_command(pattern, row, channel, (int)command);
}

String AudioStreamMPT::format_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const {
	CHECK_MOD_LOADED_RET("");
	return this->mpt_module->format_pattern_row_channel_command(pattern, row, channel, (int)command).c_str();
}

String AudioStreamMPT::highlight_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const {
	CHECK_MOD_LOADED_RET("");
	return this->mpt_module->highlight_pattern_row_channel_command(pattern, row, channel, (int)command).c_str();
}

String AudioStreamMPT::format_pattern_row_channel(int32_t pattern, int32_t row, int32_t channel, int64_t width, bool pad) const {
	CHECK_MOD_LOADED_RET("");
	return this->mpt_module->format_pattern_row_channel(pattern, row, channel, (size_t)width, pad).c_str();
}

String AudioStreamMPT::highlight_pattern_row_channel(int32_t pattern, int32_t row, int32_t channel, int64_t width, bool pad) const {
	CHECK_MOD_LOADED_RET("");
	return this->mpt_module->highlight_pattern_row_channel(pattern, row, channel, (size_t)width, pad).c_str();
}

Error AudioStreamMPT::get_module_error() const {
	return module_error;
}

Ref<AudioStreamPlayback> AudioStreamMPT::_instantiate_playback() const {
	Ref<AudioStreamPlaybackMPT> playback;
	playback.instantiate();
	playback->base = Ref<AudioStreamMPT>(this);
	playback->mpt_module = !data.is_empty() ? new openmpt::module_ext(this->data.ptr(), this->data.size()) : nullptr;
	if (playback->mpt_module)
	{
		playback->mpt_interactive = static_cast<openmpt::ext::interactive*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive_id));
		playback->mpt_interactive2 = static_cast<openmpt::ext::interactive2*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive2_id));
		playback->mpt_interactive3 = static_cast<openmpt::ext::interactive3*>(
			playback->mpt_module->get_interface(openmpt::ext::interactive3_id));
	}
	const_cast<AudioStreamMPT*>(this)->open_playback_objects.append(playback.ptr());
	return playback;
}

String AudioStreamMPT::_get_stream_name() const {
	return "";
}

void AudioStreamMPT::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_loop_mode", "loop_mode"), &AudioStreamMPT::set_loop_mode);
	ClassDB::bind_method(D_METHOD("get_loop_mode"), &AudioStreamMPT::get_loop_mode);

	ClassDB::bind_method(D_METHOD("set_stereo", "stereo"), &AudioStreamMPT::set_stereo);
	ClassDB::bind_method(D_METHOD("is_stereo"), &AudioStreamMPT::is_stereo);

	ClassDB::bind_method(D_METHOD("set_data", "data"), &AudioStreamMPT::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &AudioStreamMPT::get_data);

	ClassDB::bind_method(D_METHOD("select_subsong", "subsong"), &AudioStreamMPT::select_subsong);
	ClassDB::bind_method(D_METHOD("get_selected_subsong"), &AudioStreamMPT::get_selected_subsong);
	
	ClassDB::bind_method(D_METHOD("get_channel_names"), &AudioStreamMPT::get_channel_names);
	ClassDB::bind_method(D_METHOD("get_instrument_names"), &AudioStreamMPT::get_instrument_names);

	ClassDB::bind_method(D_METHOD("get_metadata_keys"), &AudioStreamMPT::get_metadata_keys);
	ClassDB::bind_method(D_METHOD("get_metadata", "key"), &AudioStreamMPT::get_metadata);
	ClassDB::bind_method(D_METHOD("get_all_metadata"), &AudioStreamMPT::get_all_metadata);

	ClassDB::bind_method(D_METHOD("get_estimated_bpm"), &AudioStreamMPT::get_estimated_bpm);
	ClassDB::bind_method(D_METHOD("get_speed"), &AudioStreamMPT::get_speed);
	ClassDB::bind_method(D_METHOD("get_tempo"), &AudioStreamMPT::get_tempo);

	ClassDB::bind_method(D_METHOD("get_num_channels"), &AudioStreamMPT::get_num_channels);
	ClassDB::bind_method(D_METHOD("get_num_instruments"), &AudioStreamMPT::get_num_instruments);
	ClassDB::bind_method(D_METHOD("get_num_orders"), &AudioStreamMPT::get_num_orders);
	ClassDB::bind_method(D_METHOD("get_num_patterns"), &AudioStreamMPT::get_num_patterns);
	ClassDB::bind_method(D_METHOD("get_num_samples"), &AudioStreamMPT::get_num_samples);
	ClassDB::bind_method(D_METHOD("get_num_subsongs"), &AudioStreamMPT::get_num_subsongs);

	ClassDB::bind_method(D_METHOD("get_order_names"), &AudioStreamMPT::get_order_names);
	ClassDB::bind_method(D_METHOD("get_order_pattern", "order"), &AudioStreamMPT::get_order_pattern);

	ClassDB::bind_method(D_METHOD("get_pattern_names"), &AudioStreamMPT::get_pattern_names);
	ClassDB::bind_method(D_METHOD("get_pattern_num_rows", "pattern"), &AudioStreamMPT::get_pattern_num_rows);

	ClassDB::bind_method(D_METHOD("get_sample_names"), &AudioStreamMPT::get_sample_names);
	ClassDB::bind_method(D_METHOD("get_subsong_names"), &AudioStreamMPT::get_subsong_names);

	ClassDB::bind_method(D_METHOD("get_pattern_row_channel_command", "pattern", "row", "channel", "command"), &AudioStreamMPT::get_pattern_row_channel_command);
	ClassDB::bind_method(D_METHOD("format_pattern_row_channel_command", "pattern", "row", "channel", "command"), &AudioStreamMPT::format_pattern_row_channel_command);
	ClassDB::bind_method(D_METHOD("highlight_pattern_row_channel_command", "pattern", "row", "channel", "command"), &AudioStreamMPT::highlight_pattern_row_channel_command);
	ClassDB::bind_method(D_METHOD("format_pattern_row_channel", "pattern", "row", "channel", "width", "pad"), &AudioStreamMPT::format_pattern_row_channel);
	ClassDB::bind_method(D_METHOD("highlight_pattern_row_channel", "pattern", "row", "channel", "width", "pad"), &AudioStreamMPT::highlight_pattern_row_channel);

	ClassDB::bind_method(D_METHOD("get_module_error"), &AudioStreamMPT::get_module_error);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_data", "get_data");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "loop_mode", PROPERTY_HINT_ENUM, "Disabled,Enabled"), "set_loop_mode", "get_loop_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "stereo"), "set_stereo", "is_stereo");

	BIND_ENUM_CONSTANT(LOOP_DISABLED);
	BIND_ENUM_CONSTANT(LOOP_ENABLED);
	
	BIND_ENUM_CONSTANT(COMMAND_NOTE);
	BIND_ENUM_CONSTANT(COMMAND_INSTRUMENT); 
	BIND_ENUM_CONSTANT(COMMAND_VOLUMEFFECT);
	BIND_ENUM_CONSTANT(COMMAND_EFFECT);
	BIND_ENUM_CONSTANT(COMMAND_VOLUME);
	BIND_ENUM_CONSTANT(COMMAND_PARAMETER);
}

AudioStreamMPT::AudioStreamMPT() {
}

AudioStreamMPT::~AudioStreamMPT() {
	if (this->mpt_module) delete mpt_module;
}
