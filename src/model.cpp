//
// Created by Thomas Lienbacher on 23.12.2020.
//

#include "model.hpp"

Model::Model(std::string geometryfile, std::string texturefile) {
    geometry = new GeometryFormat(geometryfile);
    texture = new Texture(texturefile);
    mesh = new Mesh(geometry);
}

Model::~Model() {
    delete mesh;
    delete geometry;
    delete texture;
}

void Model::draw() {
    mesh->bind();
    texture->bind();
    glDrawElements(GL_TRIANGLES, mesh->get_num_elements(), GL_UNSIGNED_INT, nullptr);
}
