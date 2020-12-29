# exports mesh into the binary level format for CG3D2

import bpy
import os
import bmesh
import struct
from pathlib import Path
import math
import mathutils
import array
from pprint import pprint

MAGIC = 0x4c56454c
VERSION = 3
FLAGS = 1


def write_string(file, string, max_length):
    if len(string) > (max_length - 1):
        raise Exception("str too long")
    
    for i in range(max_length):
        c = '\0'
        
        if i < len(string):
            c = string[i]
        
        file.write(struct.pack("<c", c.encode('ascii')))


def get_num_meshes():
    return len(bpy.data.collections["meshes"].all_objects)


"""
struct LevelFormatHeader {
    uint32_t magic = 0;
    uint32_t version = 0;
    uint32_t flags = 0;
    char identifier[32] = {0};
    float spawnpoint[3] = {0.f, 0.f, 0.f};
    uint32_t num_meshes = 0;
    uint32_t num_objects = 0;
} __attribute__ ((packed));
"""


def write_header(file, identifier, spawnpoint, num_meshes, num_objects):
    file.write(struct.pack("<III", MAGIC, VERSION, FLAGS))
    write_string(file, identifier, 32)
    file.write(struct.pack("<fff", spawnpoint[0], spawnpoint[1], spawnpoint[2]))
    file.write(struct.pack("<II", num_meshes, num_objects))



"""
struct LevelFormatMeshHeader {
    uint32_t blob_size = 0;
    uint32_t num_indices = 0;
    uint32_t num_vertices = 0;
} __attribute__ ((packed));
"""


def write_level_meshes(file):
    mesh_list = bpy.data.collections["meshes"].all_objects
            
    for me in mesh_list:
        bm = bmesh.new()
        bm.from_mesh(me.data)

        bmesh.ops.translate(bm, vec=me.matrix_world.to_translation(), verts=bm.verts)
        bmesh.ops.rotate(bm,
            verts=bm.verts,
            cent=(0.0, 0.0, 0.0),
            matrix=mathutils.Matrix.Rotation(math.radians(-90.0), 3, 'X'))    
        bmesh.ops.rotate(bm,
            verts=bm.verts,
            cent=(0.0, 0.0, 0.0),
            matrix=mathutils.Matrix.Rotation(math.radians(-90.0), 3, 'Y'))
        bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method="BEAUTY")
        bm.verts.ensure_lookup_table()
        bm.faces.ensure_lookup_table()
        bm.edges.ensure_lookup_table()
                
        uv_lay = bm.loops.layers.uv.active
        
        print(me.name)
        print("  n faces:", len(bm.faces))
        
        # create vertices and indices
        list_vertices = list()
        indices = list()
        current_index = 0

        for face in bm.faces:
            for loop in face.loops:
                uv = loop[uv_lay].uv
                vert = loop.vert
                position = vert.co
                texcoord = uv
                normal = face.normal
                
                pac = (position, texcoord, normal)
                
                if pac in list_vertices:
                    indices.append(list_vertices.index(pac))
                else:
                    indices.append(current_index)
                    list_vertices.append(pac)
                    current_index += 1       
        
        # write mesh header and blob
        blob_size = len(indices) * 4 + len(list_vertices) * (3 + 2 + 3) * 4
        file.write(struct.pack("<III", blob_size, len(indices), len(list_vertices)))
        
        for idx in indices:
            file.write(struct.pack("<I", idx))
        
        for v in list_vertices:
            file.write(struct.pack("<fff", v[0].x, v[0].y, v[0].z))
            
        for v in list_vertices:
            file.write(struct.pack("<ff", v[1].x, v[1].y))
            
        for v in list_vertices:
            file.write(struct.pack("<fff", v[2].x, v[2].y, v[2].z))
        
        print("  size", blob_size)
        print("  indices", len(indices))
        print("  list_vertices", len(list_vertices))
        
        bm.free()

    
"""
struct LevelFormatObject {
    enum Type : uint32_t {
        None = 0,
        Rune = 1,
        Door = 2
    };

    uint32_t id = 0;
    Type type = Type::None;
    char identifier[32] = {0};
    float position[3] = {0.f, 0.f, 0.f};

    union CustomData {
        char _phantom[24] = {0};

        struct RuneObject {
            enum Kind : uint32_t {
                A = 0,
                B = 1,
                C = 2,
            };

            Kind kind;
            float yrot;
        } rune;
        
        struct DoorObject {
            float dimensions[3];
            float yrot;
            RuneObject::Kind trigger;
        } door;
    } custom_data;
} __attribute__ ((packed));
"""


def extract_level_objects():
    level_objects = []
    objects_list = bpy.data.collections["objects"].all_objects
    
    for o in objects_list:
        new = dict()
        new["id"] = len(level_objects)
        
        type = o.data.get("type")
        if type == None:
            raise Exception("type not defined for level object " + o.name)
        
        new["type"] = type
        new["identifier"] = o.name
        trans = o.matrix_world.to_translation()
        new["position"] = (trans.y, trans.z, trans.x)
        
        if type == 0:
            new["custom_data"] = list(range(24))
            for i in range(24):
                new["custom_data"][i] = 0x41 + i
                
            new["custom_data"] = bytes(new["custom_data"])
        
        if type == 1:
            kind = o.data.get("kind")
            yrot = math.degrees(o.rotation_euler.z)
            
            if kind == None:
                raise Exception("incomplete definition " + o.name)
            
            buffer = array.array('B', [0 for i in range(24)])
            struct.pack_into("<If", buffer, 0, kind, yrot)
            new["custom_data"] = bytes(buffer)
            
        if type == 2:
            dimensions = o.dimensions
            yrot = math.degrees(o.rotation_euler.z)
            trigger = o.data.get("trigger")
            
            if trigger == None:
                raise Exception("incomplete definition " + o.name)
            
            buffer = array.array('B', [0 for i in range(24)])
            struct.pack_into("<ffffI", buffer, 0, dimensions.y, dimensions.z, dimensions.x, yrot, trigger)
            new["custom_data"] = bytes(buffer)
            
        if new.get("custom_data") == None:
            raise Exception("no custom data defined for type " + str(type) + " (obj: " + o.name + ")")
        
        level_objects.append(new)
        
    return level_objects


def write_level_object(file, level_object):
    file.write(struct.pack("<II", level_object["id"], level_object["type"]))
    write_string(file, level_object["identifier"], 32)
    file.write(struct.pack("<fff", level_object["position"][0], level_object["position"][1], level_object["position"][2]))
    
    if len(level_object["custom_data"]) != 24:
        raise Exception("custom data has to have length 24 (type: " + str(level_object["type"]) + ")")
    file.write(level_object["custom_data"])
    

basedir = os.path.dirname(bpy.data.filepath)
if not basedir:
    raise Exception("Blend file is not saved")

level_name = Path(bpy.data.filepath).stem
level_file_path = os.path.join(basedir, level_name + ".level")
identifier = level_name + "@bl@lv"

spawnpoint = (0, 0, 0) # in opengl coords

level_objects = extract_level_objects()

pprint(level_objects)

with open(level_file_path, 'wb') as file:
    write_header(file, identifier, spawnpoint, get_num_meshes(), len(level_objects))
    write_level_meshes(file)
    
    for lo in level_objects:
        write_level_object(file, lo)
        
    file.flush()
    

print("Finished exporting level to: ", level_file_path)
