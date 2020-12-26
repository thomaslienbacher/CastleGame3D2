//
// Created by Thomas Lienbacher on 10.12.2020.
//

#ifndef CASTLEGAME3D2_GEOMETRY_HPP
#define CASTLEGAME3D2_GEOMETRY_HPP

#include "master.hpp"
#include <fstream>
#include <cstdint>

//TODO: put constants into namespace or struct
const uint32_t GEOMETRY_MAGIC = 0x4c444f4d;
const uint32_t GEOMETRY_VERSION = 0x2;

struct GeometryFormatHeader {
    uint32_t magic = 0;
    uint32_t version = 0;
    uint32_t flags = 0;
    char identifier[32] = {0};
    uint32_t num_indices = 0;
    uint32_t num_vertices = 0;
} __attribute__ ((packed));

class GeometryFormat {
    bool allocated;

public:
    GeometryFormatHeader header;
    uint32_t *indices = nullptr;
    float *vertices = nullptr;
    float *texcoords = nullptr;
    float *normals = nullptr;

    explicit GeometryFormat(GeometryFormatHeader header = {});

    explicit GeometryFormat(std::ifstream &binfile);

    explicit GeometryFormat(std::string &binfile);

    ~GeometryFormat();

    static GeometryFormat import_from_file(std::ifstream &file);

    static GeometryFormat import_from_obj(std::ifstream &file);

    void export_to_file(std::ofstream &file);
};


#endif //CASTLEGAME3D2_GEOMETRY_HPP
