//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_MESH_HPP
#define CASTLEGAME3D2_MESH_HPP

#include "master.hpp"

class Mesh {
    GLuint vao;
    GLuint ebo;
    GLuint vbos[3];

    unsigned int num_elements;

    const int VBO_VERTICES = 0;
    const int VBO_TEXCOORDS = 1;
    const int VBO_NORMALS = 2;

public:
    Mesh(unsigned int num_indices, unsigned int *indices, unsigned int num_vertices, float* vertices, float* texcoords, float* normals);
    ~Mesh();

    void bind();

    unsigned int get_num_elements() const;
};


#endif //CASTLEGAME3D2_MESH_HPP
