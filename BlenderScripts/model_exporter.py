# exports mesh into the binary format for CG3D2

import bpy
import os
import bmesh
import struct
from pathlib import Path
import math
import mathutils

me = bpy.context.object.data
bm = bmesh.new()
bm.from_mesh(me)

bmesh.ops.rotate(bm,
    verts=bm.verts,
    cent=(0.0, 0.0, 0.0),
    matrix=mathutils.Matrix.Rotation(math.radians(-90.0), 3, 'X'))
    
bmesh.ops.rotate(bm,
    verts=bm.verts,
    cent=(0.0, 0.0, 0.0),
    matrix=mathutils.Matrix.Rotation(math.radians(-90.0), 3, 'Y'))

bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method="BEAUTY")

uv_lay = bm.loops.layers.uv.active

basedir = os.path.dirname(bpy.data.filepath)

if not basedir:
    raise Exception("Blend file is not saved")


geometry_name = Path(bpy.data.filepath).stem
newfile = os.path.join(basedir, geometry_name + ".model")
    
bm.verts.ensure_lookup_table()
bm.faces.ensure_lookup_table()
bm.edges.ensure_lookup_table()

print("n face:", len(bm.faces))
print("n per face loop", len(bm.faces[0].loops))

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
        
print("n vertices:", len(list_vertices))
print("n indices:", len(indices))


"""
struct GeometryFormatHeader {
    uint32_t magic = MAGIC_NUMBER;
    uint32_t version = 0;
    uint32_t flags = 0;
    char identifier[32] = {0};
    uint32_t num_indices = 0;
    uint32_t num_vertices = 0;
} __attribute__ ((packed));
"""

MAGIC = 0x4c444f4d
VERSION = 2
FLAGS = 1

identifier = geometry_name + "@" + bpy.context.object.name + "@blender"

print(identifier)

if len(identifier) > 31:
    print("ERROR: identifier too long!")
    exit(1)

with open(newfile, 'wb') as file:
    # write basic info
    file.write(struct.pack("<III", MAGIC, VERSION, FLAGS))
    
    # write identifier
    for i in range(32):
        c = '\0'
        
        if i < len(identifier):
            c = identifier[i]
        
        file.write(struct.pack("<c", c.encode('ascii')))
        
    # write number ofs
    file.write(struct.pack("<II", len(indices), len(list_vertices)))
    
    # mesh info
    for idx in indices:
        file.write(struct.pack("<I", idx))
    
    for v in list_vertices:
        file.write(struct.pack("<fff", v[0].x, v[0].y, v[0].z))
        
    for v in list_vertices:
        file.write(struct.pack("<ff", v[1].x, v[1].y))
        
    for v in list_vertices:
        file.write(struct.pack("<fff", v[2].x, v[2].y, v[2].z))
        
    file.flush()
    

print("exported to:", newfile)

