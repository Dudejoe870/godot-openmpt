#ifndef AUDIO_STREAM_MPT_H
#define AUDIO_STREAM_MPT_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>

#include <godot_cpp/templates/vector.hpp>

#include <libopenmpt/libopenmpt.hpp>

using namespace godot;

class AudioStreamMPT;

class AudioStreamPlaybackMPT : public AudioStreamPlayback {
    GDCLASS(AudioStreamPlaybackMPT, AudioStreamPlayback);
private:
	Ref<AudioStreamMPT> base;
	openmpt::module *mpt_module = nullptr;

	bool active = false;

	friend class AudioStreamMPT;
protected:
	static void _bind_methods();
public:
	virtual void _start(double p_from_pos = 0.0) override;
	virtual void _stop() override;
	virtual bool _is_playing() const override;

	virtual int _get_loop_count() const override; //times it looped

	virtual double _get_playback_position() const override;

	virtual void _seek(double p_time) override;
	void seek(int32_t p_order_position, int32_t p_row);

	int32_t get_order_position();
	int32_t get_current_row();

	int32_t get_current_pattern();

	virtual int32_t _mix(AudioFrame *p_buffer, double p_rate_scale, int32_t p_frames) override;

	AudioStreamPlaybackMPT();
	~AudioStreamPlaybackMPT();
};

class AudioStreamMPT : public AudioStream {
    GDCLASS(AudioStreamMPT, AudioStream);
public:
	enum LoopMode {
		LOOP_DISABLED,
		LOOP_ENABLED
	};
private:
	LoopMode loop_mode = LoopMode::LOOP_DISABLED;
	bool stereo = true;
	PackedByteArray data;

	// We need to create a module to parse some information about the file
	// otherwise we would only need one in the Playback class.
	openmpt::module *mpt_module = nullptr;

	Vector<AudioStreamPlaybackMPT*> open_playback_objects;

	Error module_error = Error::OK;

	friend class AudioStreamPlaybackMPT;
protected:
    static void _bind_methods();
public:
	void set_loop_mode(LoopMode p_loop_mode);
	LoopMode get_loop_mode() const;

	void set_stereo(bool p_enable);
	bool is_stereo() const;

	virtual double _get_length() const override;
	virtual bool _is_monophonic() const override;

	void set_data(const PackedByteArray& p_data);
	const PackedByteArray& get_data() const;

	Error get_module_error() const;

	virtual Ref<AudioStreamPlayback> _instantiate_playback() const override;
	virtual String _get_stream_name() const override;

	AudioStreamMPT();
	~AudioStreamMPT();
};

VARIANT_ENUM_CAST(AudioStreamMPT::LoopMode)

#endif // AUDIO_STREAM_MPT_H
