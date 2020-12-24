//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_MESH_HPP
#define CASTLEGAME3D2_MESH_HPP

#include "master.hpp"
#include "geometry.hpp"

class Mesh {
    GLuint vao;
    GLuint ebo;
    GLuint vbos[3];

    unsigned int num_elements;

    static constexpr int VBO_VERTICES = 0;
    static constexpr int VBO_TEXCOORDS = 1;
    static constexpr int VBO_NORMALS = 2;

public:
    Mesh() = default;

    Mesh(unsigned int num_indices, unsigned int *indices, unsigned int num_vertices, float *vertices, float *texcoords,
         float *normals);

    explicit Mesh(GeometryFormat *geometry);

    explicit Mesh(GeometryFormat &geometry);

    ~Mesh();

    void bind();

    unsigned int get_num_elements() const;
};


#endif //CASTLEGAME3D2_MESH_HPP
