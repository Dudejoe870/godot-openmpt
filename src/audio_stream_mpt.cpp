#include "audio_stream_mpt.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>

#include <godot_cpp/templates/local_vector.hpp>

#include <libopenmpt/libopenmpt.hpp>

using namespace godot;

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
	if (!this->mpt_module) return 0.0;
	return this->mpt_module->get_position_seconds();
}

void AudioStreamPlaybackMPT::_seek(double p_time) {
	if (!this->mpt_module) return;
	this->mpt_module->set_position_seconds(p_time);
}

void AudioStreamPlaybackMPT::seek(int32_t p_order_position, int32_t p_row) {
	if (!this->mpt_module) return;
	this->mpt_module->set_position_order_row(p_order_position, p_row);
}

int32_t AudioStreamPlaybackMPT::get_order_position() {
	if (!this->mpt_module) return 0;
	return this->mpt_module->get_current_order();
}

int32_t AudioStreamPlaybackMPT::get_current_row() {
	if (!this->mpt_module) return 0;
	return this->mpt_module->get_current_row();
}

int32_t AudioStreamPlaybackMPT::get_current_pattern() {
	if (!this->mpt_module) return 0;
	return this->mpt_module->get_current_pattern();
}

int32_t AudioStreamPlaybackMPT::_mix(AudioFrame *p_buffer, double p_rate_scale, int32_t p_frames) {
	if (!this->mpt_module) {
		active = false;
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

	ClassDB::bind_method(D_METHOD("get_order_position"), &AudioStreamPlaybackMPT::get_order_position);
	ClassDB::bind_method(D_METHOD("get_current_row"), &AudioStreamPlaybackMPT::get_current_row);
	ClassDB::bind_method(D_METHOD("get_current_pattern"), &AudioStreamPlaybackMPT::get_current_pattern);
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
	if (this->mpt_module)
	{
		double duration = this->mpt_module->get_duration_seconds();
		if (isinf(duration))
			return 0.0;
	}
	return 0.0;
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
		playback->mpt_module = new openmpt::module(this->data.ptr(), this->data.size());
	}
	AudioServer::get_singleton()->unlock();
	return;

set_empty_module:
	this->mpt_module = nullptr;

	if (open_playback_objects.is_empty()) {
		this->data = PackedByteArray();
		return;
	}
	AudioServer::get_singleton()->lock();
	for (AudioStreamPlaybackMPT* playback : open_playback_objects) {
		if (playback->mpt_module) delete playback->mpt_module;
		playback->mpt_module = nullptr;
	}
	AudioServer::get_singleton()->unlock();

	this->data = PackedByteArray();
	return;
}

const PackedByteArray& AudioStreamMPT::get_data() const {
	return this->data;
}

Error AudioStreamMPT::get_module_error() const {
	return module_error;
}

Ref<AudioStreamPlayback> AudioStreamMPT::_instantiate_playback() const {
	Ref<AudioStreamPlaybackMPT> playback;
	playback.instantiate();
	playback->base = Ref<AudioStreamMPT>(this);
	playback->mpt_module = !data.is_empty() ? new openmpt::module(this->data.ptr(), this->data.size()) : nullptr;
	const_cast<AudioStreamMPT*>(this)->open_playback_objects.append(playback.ptr());
	return playback;
}

String AudioStreamMPT::_get_stream_name() const {
	return "";
}

void AudioStreamMPT::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_data", "data"), &AudioStreamMPT::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &AudioStreamMPT::get_data);

	ClassDB::bind_method(D_METHOD("set_loop_mode", "loop_mode"), &AudioStreamMPT::set_loop_mode);
	ClassDB::bind_method(D_METHOD("get_loop_mode"), &AudioStreamMPT::get_loop_mode);

	ClassDB::bind_method(D_METHOD("set_stereo", "stereo"), &AudioStreamMPT::set_stereo);
	ClassDB::bind_method(D_METHOD("is_stereo"), &AudioStreamMPT::is_stereo);

	ClassDB::bind_method(D_METHOD("get_module_error"), &AudioStreamMPT::get_module_error);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_data", "get_data");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "loop_mode", PROPERTY_HINT_ENUM, "Disabled,Enabled"), "set_loop_mode", "get_loop_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "stereo"), "set_stereo", "is_stereo");

	BIND_ENUM_CONSTANT(LOOP_DISABLED);
	BIND_ENUM_CONSTANT(LOOP_ENABLED);
}

AudioStreamMPT::AudioStreamMPT() {
}

AudioStreamMPT::~AudioStreamMPT() {
	if (this->mpt_module) delete mpt_module;
}
