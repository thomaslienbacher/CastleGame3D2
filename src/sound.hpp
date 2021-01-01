//
// Created by Thomas Lienbacher on 30.12.2020.
//

#ifndef CASTLEGAME3D2_SOUND_HPP
#define CASTLEGAME3D2_SOUND_HPP

#include "glalheaders.hpp"
#include <string>
#include <glm/glm.hpp>

class SoundSource;

class Sound {
    friend SoundSource;

    ALuint buffer;
public:

    explicit Sound(const std::string &ogg);

    ~Sound();
};

class SoundSource {
    ALuint source;

public:
    SoundSource();

    ~SoundSource();

    void play(Sound &sound);

    bool is_playing() const;

    void set_pitch(float pitch);

    void set_position(glm::vec3 position);

    void set_looping(bool looping);

    void set_volume(float gain);
};

class SoundListener {
public:
    static void set_position(glm::vec3 position);

    static void set_volume(float gain);
};

#endif //CASTLEGAME3D2_SOUND_HPP
