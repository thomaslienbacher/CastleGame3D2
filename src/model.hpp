//
// Created by Thomas Lienbacher on 23.12.2020.
//

#ifndef CASTLEGAME3D2_MODEL_HPP
#define CASTLEGAME3D2_MODEL_HPP

#include "master.hpp"
#include "geometry.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include <glm/glm.hpp>

class Model {
    GeometryFormat *geometry;
    Mesh *mesh;
    Texture *texture;

public:
    glm::mat4 transform;

    Model(std::string geometryfile, std::string texturefile);

    ~Model();

    void draw();

};


#endif //CASTLEGAME3D2_MODEL_HPP
