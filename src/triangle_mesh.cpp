#include "triangle_mesh.h"

TriangleMesh::TriangleMesh(uint32_t index_count, uint16_t* indices, uint32_t vertex_count, float* positions, float* normals, float* uvs)
{
    this->index_count = index_count;
    this->indices = indices;
    this->vertex_count = vertex_count;
    this->face_count = index_count / 3;
    this->positions = positions;
    this->normals = normals;
    this->uvs = uvs;
}

TriangleMesh::~TriangleMesh()
{
    if (indices)
        delete[] indices;

    if (positions)
        delete[] positions;

    if (normals)
        delete[] normals;

    if (uvs)
        delete[] uvs;

    vertices_faces.clear();
    vertices_edges.clear();
    faces_edges.clear();
}

void TriangleMesh::build_neighbors()
{
    std::vector<std::vector<unsigned int>> edges_list;
    edges_list.resize(vertex_count);

    vertices_faces.clear();
    vertices_faces.resize(vertex_count);
    vertices_edges.clear();
    vertices_edges.resize(vertex_count);
    faces_edges.clear();
    faces_edges.resize(face_count);

    uint16_t temp_v[3];
    uint32_t temp_edges[6];
    for (int i = 0; i < face_count; ++i)
    {
        faces_edges[i].resize(3);

        temp_v[0] = indices[i * 3 + 0];
        temp_v[1] = indices[i * 3 + 1];
        temp_v[2] = indices[i * 3 + 2];

        temp_edges[0] = temp_v[0];
        temp_edges[1] = temp_v[1];
        temp_edges[2] = temp_v[1];
        temp_edges[3] = temp_v[2];
        temp_edges[4] = temp_v[2];
        temp_edges[5] = temp_v[0];

        for (int j = 0; j < 3; ++j)
        {
            uint16_t index = indices[i * 3 + j];
            bool found = false;
            for (int k = 0; k < vertices_faces[index].size(); ++k)
            {
                if (vertices_faces[index][k] == i)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                vertices_faces[index].push_back(i);
            }

            uint32_t a = temp_edges[j * 2];
            uint32_t b = temp_edges[j * 2 + 1];
            uint32_t edge = 0xffffffff;
            for (int k = 0; k < edges_list[a].size(); ++k)
            {
                const Edge& e = edges[edges_list[a][k]];
                if (((e.vert[0] == a) || (e.vert[0] == b)) && ((e.vert[1] == a) || (e.vert[1] == b)))
                {
                    edge = edges_list[a][k];
                    break;
                }
            }
            if (edge == 0xffffffff)
            {
                Edge e{};
                e.vert[0] = a;
                e.vert[1] = b;
                e.face[0] = i;
                e.face[1] = 0xffffffff;
                edges.push_back(e);
                edge = edges.size() - 1;

                vertices_edges[a].push_back(edge);
                vertices_edges[b].push_back(edge);
            }
            else
            {
                Edge& e = edges[edge];
                e.face[1] = i;
            }
            edges_list[a].push_back(edge);
            edges_list[b].push_back(edge);
            faces_edges[i][j] = edge;
        }
    }
}

void TriangleMesh::update_normals()
{
    if (!normals)
        normals = new float[vertex_count * 3];

    memset(normals, 0, sizeof(float) * vertex_count * 3);

    for (int i = 0; i < face_count; ++i)
    {
        uint16_t index0 = indices[i * 3];
        uint16_t index1 = indices[i * 3 + 1];
        uint16_t index2 = indices[i * 3 + 2];
        glm::vec3 v0 = glm::vec3(positions[index0 * 3], positions[index0 * 3 + 1], positions[index0 * 3 + 2]);
        glm::vec3 v1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1], positions[index1 * 3 + 2]);
        glm::vec3 v2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1], positions[index2 * 3 + 2]);

        glm::vec3 e0 = v1 - v0;
        glm::vec3 e1 = v2 - v0;
        glm::vec3 n = glm::normalize(glm::cross(e0, e1));

        normals[index0 * 3] += n.x;
        normals[index0 * 3 + 1] += n.y;
        normals[index0 * 3 + 2] += n.z;
        normals[index1 * 3] += n.x;
        normals[index1 * 3 + 1] += n.y;
        normals[index1 * 3 + 2] += n.z;
        normals[index2 * 3] += n.x;
        normals[index2 * 3 + 1] += n.y;
        normals[index2 * 3 + 2] += n.z;
    }
}