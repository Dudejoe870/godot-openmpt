#ifndef AUDIO_STREAM_MPT_H
#define AUDIO_STREAM_MPT_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>

using namespace godot;

class AudioStreamMPT;

class AudioStreamPlaybackMPT : public AudioStreamPlayback {
    GDCLASS(AudioStreamPlaybackMPT, AudioStreamPlayback);
protected:
	static void _bind_methods();
private:
    
};

class AudioStreamMPT : public AudioStream {
    GDCLASS(AudioStreamMPT, AudioStream);
protected:
    static void _bind_methods();
private:

};

#endif // AUDIO_STREAM_MPT_H
