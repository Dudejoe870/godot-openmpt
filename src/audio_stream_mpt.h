#ifndef AUDIO_STREAM_MPT_H
#define AUDIO_STREAM_MPT_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>

#include <godot_cpp/templates/vector.hpp>

#include <libopenmpt/libopenmpt.hpp>
#include <libopenmpt/libopenmpt_ext.hpp>

using namespace godot;

class AudioStreamMPT;

class AudioStreamPlaybackMPT : public AudioStreamPlayback {
    GDCLASS(AudioStreamPlaybackMPT, AudioStreamPlayback);
private:
	Ref<AudioStreamMPT> base;
	openmpt::module_ext *mpt_module = nullptr;
	openmpt::ext::interactive  *mpt_interactive = nullptr;
	openmpt::ext::interactive2 *mpt_interactive2 = nullptr;
	openmpt::ext::interactive3 *mpt_interactive3 = nullptr;

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

	bool is_module_loaded() const;
	bool is_interactive_supported() const;
	bool is_interactive2_supported() const;
	bool is_interactive3_supported() const;

	float get_current_channel_vu_left(int32_t channel) const;
	float get_current_channel_vu_mono(int32_t channel) const;
	float get_current_channel_vu_rear_left(int32_t channel) const;
	float get_current_channel_vu_rear_right(int32_t channel) const;
	float get_current_channel_vu_right(int32_t channel) const;

	double get_current_estimated_bpm() const;

	int32_t get_current_order() const;
	void set_current_order(int32_t order);

	int32_t get_current_pattern() const;
	int32_t get_current_playing_channels() const;

	int32_t get_current_row() const;
	void set_current_row(int32_t row) const;

	void select_subsong(int32_t subsong);
	int32_t get_selected_subsong() const;

	void set_current_speed(int32_t speed);
	int32_t get_current_speed() const;

	void set_current_tempo(double tempo);
	double get_current_tempo() const;

	void set_tempo_factor(double factor);
	double get_tempo_factor() const;

	void set_pitch_factor(double factor);
	double get_pitch_factor() const;

	void set_global_volume(double volume);
	double get_global_volume() const;

	void set_channel_volume(int32_t channel, double volume);
	double get_channel_volume(int32_t channel) const;

	void set_channel_mute_status(int32_t channel, bool mute);
	bool get_channel_mute_status(int32_t channel) const;

	void set_instrument_mute_status(int32_t instrument, bool mute);
	bool get_instrument_mute_status(int32_t instrument) const;

	int32_t play_note(int32_t instrument, int32_t note, double volume, double panning);
	void stop_note(int32_t channel);
	void note_off(int32_t channel);
	void note_fade(int32_t channel);
	
	void set_channel_panning(int32_t channel, double panning);
	double get_channel_panning(int32_t channel) const;

	void set_note_finetune(int32_t channel, double finetune);
	double get_note_finetune(int32_t channel) const;

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

	enum CommandIndex {
		COMMAND_NOTE = 0,
		COMMAND_INSTRUMENT = 1, 
		COMMAND_VOLUMEFFECT = 2,
		COMMAND_EFFECT = 3,
		COMMAND_VOLUME = 4,
		COMMAND_PARAMETER = 5
	};
private:
	LoopMode loop_mode = LoopMode::LOOP_DISABLED;
	bool stereo = true;
	PackedByteArray data;

	// We need to create a module to parse any information about the file,
	// otherwise we would only need to have one in the Playback class.
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

	TypedArray<String> get_channel_names() const;
	TypedArray<String> get_instrument_names() const;

	TypedArray<String> get_metadata_keys() const;
	String get_metadata(String key) const;
	Dictionary get_all_metadata() const;

	double get_estimated_bpm() const;
	int32_t get_speed() const;
	double get_tempo() const;

	int32_t get_num_channels() const;
	int32_t get_num_instruments() const;
	int32_t get_num_orders() const;
	int32_t get_num_patterns() const;
	int32_t get_num_samples() const;
	int32_t get_num_subsongs() const;

	void select_subsong(int32_t subsong);
	int32_t get_selected_subsong() const;
	
	TypedArray<String> get_order_names() const;
	int32_t get_order_pattern(int32_t order) const;

	TypedArray<String> get_pattern_names() const;
	int32_t get_pattern_num_rows(int32_t pattern) const;

	TypedArray<String> get_sample_names() const;
	TypedArray<String> get_subsong_names() const;

	uint8_t get_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const;
	String format_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const;
	String highlight_pattern_row_channel_command(int32_t pattern, int32_t row, int32_t channel, CommandIndex command) const;
	String format_pattern_row_channel(int32_t pattern, int32_t row, int32_t channel, int64_t width = 0, bool pad = true) const;
	String highlight_pattern_row_channel(int32_t pattern, int32_t row, int32_t channel, int64_t width = 0, bool pad = true) const;

	Error get_module_error() const;

	virtual Ref<AudioStreamPlayback> _instantiate_playback() const override;
	virtual String _get_stream_name() const override;

	AudioStreamMPT();
	~AudioStreamMPT();
};

VARIANT_ENUM_CAST(AudioStreamMPT::LoopMode)
VARIANT_ENUM_CAST(AudioStreamMPT::CommandIndex)

#endif // AUDIO_STREAM_MPT_H
