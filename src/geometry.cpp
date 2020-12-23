//
// Created by Thomas Lienbacher on 10.12.2020.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include "geometry.hpp"

GeometryFormat::GeometryFormat(GeometryFormatHeader header) : header(header) {
    allocated = false;
}

GeometryFormat::~GeometryFormat() {
    if (allocated) {
        delete[] indices;
        delete[] vertices;
        delete[] texcoords;
        delete[] normals;
    }
}

GeometryFormat GeometryFormat::import_from_file(std::ifstream &file) {
    GeometryFormat g;
    g.allocated = true;

    //TODO: check for MAGIC NUMBER and read errors

    file.read((char *) &g.header, sizeof(GeometryFormatHeader));
    g.indices = new uint32_t[g.header.num_indices];
    g.vertices = new float[g.header.num_vertices * 3];
    g.texcoords = new float[g.header.num_vertices * 2];
    g.normals = new float[g.header.num_vertices * 3];

    file.read((char *) g.indices, sizeof(uint32_t) * g.header.num_indices);
    file.read((char *) g.vertices, sizeof(float) * g.header.num_vertices * 3);
    file.read((char *) g.texcoords, sizeof(float) * g.header.num_vertices * 2);
    file.read((char *) g.normals, sizeof(float) * g.header.num_vertices * 3);

    return g;
}

void GeometryFormat::export_to_file(std::ofstream &file) {
    file.write((char *) &this->header, sizeof(GeometryFormatHeader));
    file.write((char *) this->indices, sizeof(uint32_t) * header.num_indices);
    file.write((char *) this->vertices, sizeof(float) * header.num_vertices * 3);
    file.write((char *) this->texcoords, sizeof(float) * header.num_vertices * 2);
    file.write((char *) this->normals, sizeof(float) * header.num_vertices * 3);
    file.flush();
}

GeometryFormat GeometryFormat::import_from_obj(std::ifstream &file) {
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> face_indices;
    std::vector<std::tuple<float, float, float>> all_vertices;
    std::vector<std::tuple<float, float>> all_texcoords;
    std::vector<std::tuple<float, float, float>> all_normals;

    std::string line;

    while (std::getline(file, line)) {
        if (line.starts_with('#')) continue;


        if (line.starts_with("v ")) {
            float a, b, c;
            std::stringstream stream(line.substr(2));
            stream >> a >> b >> c;
            all_vertices.emplace_back(a, b, c);
        }

        if (line.starts_with("vt ")) {
            float a, b;
            std::stringstream stream(line.substr(3));
            stream >> a >> b;
            all_texcoords.emplace_back(a, b);
        }

        if (line.starts_with("vn ")) {
            float a, b, c;
            std::stringstream stream(line.substr(3));
            stream >> a >> b >> c;
            all_normals.emplace_back(a, b, c);
        }

        if (line.starts_with("f ")) {
            uint32_t a, b, c;
            std::stringstream stream(line.substr(2));
            for (int i = 0; i < 3; ++i) {
                stream >> a;
                stream.ignore();
                stream >> b;
                stream.ignore();
                stream >> c;
                stream.ignore();
                face_indices.emplace_back(a - 1, b - 1, c - 1); //obj starts counting at 0
            }
        }
    }

    unsigned int num_indices = 0;
    unsigned int num_vertices = 0;
    std::unordered_map<unsigned long, unsigned int> finished_faces;
    std::vector<uint32_t> indices;
    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;

    for (auto f : face_indices) {
        unsigned int v = std::get<0>(f);
        unsigned int vt = std::get<1>(f);
        unsigned int vn = std::get<2>(f);
        unsigned long face_value = (unsigned long) (v + 1) * 100'000'000 + (vt + 1) * 10'000 +
                                   (vn + 1);//TODO: fix max vertices per model is 9999

        if (finished_faces.contains(face_value)) {
            indices.emplace_back(finished_faces[face_value]);
        } else {
            vertices.emplace_back(std::get<0>(all_vertices[v]));
            vertices.emplace_back(std::get<1>(all_vertices[v]));
            vertices.emplace_back(std::get<2>(all_vertices[v]));

            texcoords.emplace_back(std::get<0>(all_texcoords[vt]));
            texcoords.emplace_back(std::get<1>(all_texcoords[vt]));

            normals.emplace_back(std::get<0>(all_normals[vn]));
            normals.emplace_back(std::get<1>(all_normals[vn]));
            normals.emplace_back(std::get<2>(all_normals[vn]));

            finished_faces[face_value] = num_vertices;
            indices.emplace_back(num_vertices);
            num_vertices++;
        }
        num_indices++;
    }

    GeometryFormat g;
    g.allocated = true;
    g.header.num_vertices = num_vertices;
    g.header.num_indices = num_indices;

    g.indices = new uint32_t[g.header.num_indices];
    g.vertices = new float[g.header.num_vertices * 3];
    g.texcoords = new float[g.header.num_vertices * 2];
    g.normals = new float[g.header.num_vertices * 3];

    std::memcpy(g.indices, indices.data(), indices.size() * sizeof(uint32_t));
    std::memcpy(g.vertices, vertices.data(), vertices.size() * sizeof(float));
    std::memcpy(g.texcoords, texcoords.data(), texcoords.size() * sizeof(float));
    std::memcpy(g.normals, normals.data(), normals.size() * sizeof(float));

    return g;
}
