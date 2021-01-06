//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "mesh.hpp"

Mesh::Mesh(unsigned int num_indices, unsigned int *indices, unsigned int num_vertices, float *vertices,
           float *texcoords, float *normals) {
    num_elements = num_indices;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glGenBuffers(3, this->vbos);

    const unsigned int stride = 0;
    const void *offset = nullptr;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbos[VBO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbos[VBO_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 2 * sizeof(float), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, offset);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbos[VBO_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(float), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, offset);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

Mesh::Mesh(GeometryFormat &geometry) :
        Mesh(geometry.header.num_indices, geometry.indices, geometry.header.num_vertices, geometry.vertices,
             geometry.texcoords, geometry.normals) {}

Mesh::Mesh(GeometryFormat *geometry) :
        Mesh(geometry->header.num_indices, geometry->indices,
             geometry->header.num_vertices, geometry->vertices,
             geometry->texcoords, geometry->normals) {}

Mesh::~Mesh() {
    glDeleteBuffers(3, this->vbos);
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
}

static GLuint vao_in_use = 0;

void Mesh::bind() {
    if (vao != vao_in_use) {
        glBindVertexArray(this->vao);
        vao_in_use = vao;
    }
}

void Mesh::bind(GLuint vao) {
    if (vao != vao_in_use) {
        glBindVertexArray(vao);
        vao_in_use = vao;
    }
}

unsigned int Mesh::get_num_elements() const {
    return num_elements;
}
