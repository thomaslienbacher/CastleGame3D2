//
// Created by Thomas Lienbacher on 10.12.2020.
//

#include <fstream>
#include <cstdint>

const uint32_t MAGIC_NUMBER = 0x30464433;
const uint32_t VERSION_0 = 0x0;

struct GeometryFormatHeader {
    uint32_t magic = MAGIC_NUMBER;
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

    ~GeometryFormat();

    static GeometryFormat import_from_file(std::ifstream &file);

    static GeometryFormat import_from_obj(std::ifstream &file);

    void export_to_file(std::ofstream &file);

};

