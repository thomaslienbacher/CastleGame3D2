//
// Created by Thomas Lienbacher on 30.12.2020.
//

#include "sound.hpp"

#define STB_VORBIS_MAX_CHANNELS 2

#include <stb/stb_vorbis.c>
#include <iostream>
#include "utils.hpp"

Sound::Sound(const std::string &ogg) {
    alGenBuffers(1, &buffer);
    int channels, sample_rate;
    short *data = nullptr;
    int samples = stb_vorbis_decode_filename(ogg.c_str(), &channels, &sample_rate, &data);

    ALenum format;

    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
        std::cout << "Sound warning stereo sound can't be played in 3D space! " << ogg << std::endl;
    } else {
        std::cerr << "Incorrect channels loaded: " << channels << panic;
    }

    alBufferData(buffer, format, data, channels * samples * sizeof(short), sample_rate);
    free(data);
}

Sound::~Sound() {
    alDeleteBuffers(1, &buffer);
}

SoundSource::SoundSource() {
    alGenSources(1, &source);
}

SoundSource::~SoundSource() {
    alDeleteSources(1, &source);
}

void SoundSource::play(Sound &sound) {
    alSourcei(source, AL_BUFFER, sound.buffer);
    alSourcePlay(source);
}

bool SoundSource::is_playing() const {
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void SoundSource::set_pitch(float pitch) {
    alSourcef(source, AL_PITCH, pitch);
}

void SoundSource::set_position(glm::vec3 position) {
    alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}

void SoundSource::set_looping(bool looping) {
    alSourcei(source, AL_LOOPING, looping);
}

void SoundSource::set_volume(float gain) {
    alSourcef(source, AL_GAIN, gain);
}

void SoundListener::set_position(glm::vec3 position) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void SoundListener::set_volume(float gain) {
    alListenerf(AL_GAIN, gain);
}
