#include "tree.hpp"
#include <cmath>
using namespace cgp;


mesh mesh_primitive_pave(float x, float y, float z, float length, float width, float height, float alpha_left, float alpha_right, float crop) {

    assert (length > 0);
    //std::cout << length << "/" << width << "/" << height << std::endl;
    assert(width > 0);
    assert(height > 0);


    vec3 p000 = vec3{ x, y, z };
    vec3 p100 = p000 + vec3{ length,0,0 };
    vec3 p110 = p000 + vec3{ length - width * tan(alpha_right / 2),width,0 };
    vec3 p010 = p000 + vec3{ width * tan(alpha_left / 2),width,0 };
    vec3 p001 = p000 + vec3{ 0,0,height };
    vec3 p101 = p000 + vec3{ length,0, height };
    vec3 p111 = p000 + vec3{ length - width * tan(alpha_right / 2),width,height };
    vec3 p011 = p000 + vec3{ width * tan(alpha_left / 2),width,height };

    mesh shape;
    shape.push_back(mesh_primitive_quadrangle(p000, p100, p101, p001));
    shape.push_back(mesh_primitive_quadrangle(p100, p110, p111, p101));
    shape.push_back(mesh_primitive_quadrangle(p110, p010, p011, p111));
    shape.push_back(mesh_primitive_quadrangle(p010, p000, p001, p011));
    shape.push_back(mesh_primitive_quadrangle(p001, p101, p111, p011));
    shape.push_back(mesh_primitive_quadrangle(p100, p000, p010, p110));
    shape.uv = { { 0, 0 }, { 1, 0 }, { 1, crop }, { 0, crop },
        {1.5f, 1.5f }, {1.5f, 1.5f },{1.5f, 1.5f },{1.5f, 1.5f },
        {1.5f, 1.5f }, {1.5f, 1.5f },{1.5f, 1.5f },{1.5f, 1.5f },
        {1.5f, 1.5f }, {1.5f, 1.5f },{1.5f, 1.5f },{1.5f, 1.5f },
        {1.5f, 1.5f }, {1.5f, 1.5f },{1.5f, 1.5f },{1.5f, 1.5f },
        {1.5f, 1.5f }, {1.5f, 1.5f },{1.5f, 1.5f },{1.5f, 1.5f } };

    return shape;
}

mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; 
    int maximalSize = 50;
    // To do: fill this mesh ...
    // ...
    // To add a position: 
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    // 
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});

    m.position.resize(maximalSize);


    for (int i = 0;  2 * i < maximalSize; i += 1) {
        float x = radius * std::cos(i * 6.28 / (maximalSize / 2.00f - 1.00f));
        float y = radius * std::sin(i * 6.28 / (maximalSize / 2.00f - 1.00f));
        m.position[2*i] = vec3{ x, y, 0 };
        m.position[2*i+1] = vec3{ x, y, height };
    }

    for (int j = 0; j < maximalSize - 2; j += 2) {
        m.connectivity.push_back(uint3{ j + 2, j + 1, j });
        m.connectivity.push_back(uint3{ j + 3, j + 1, j + 2 });
    }


    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_roof(float length, float width, float height, float roof_height, float e_left, float e_right, float e_y, float alpha_left, float alpha_right, float crop_up, float height_image) {
    mesh m;

    vec3 a;
    vec3 b;
    vec3 c;
    vec3 d;
    vec3 a1;
    vec3 b1;
    vec3 c1;
    vec3 d1;

    a = vec3{0, 0, height };
    b = vec3{ width * tan(alpha_left / 2), width, height };
    c = vec3{ length - width * tan(alpha_right / 2), width, height };
    d = vec3{ length, 0, height };


    float epsilon_y = e_y * width;

    float epsilon_right_c = epsilon_y * tan(alpha_right / 2);
    float epsilon_right = e_right * length;
    float epsilon_left_c = epsilon_y * tan(alpha_left / 2);
    float epsilon_left = e_left * length;
    
    //m.uv.resize(16 * N);

 
    int N = 2;
    vec2 percent_heigth = { 0.7, 0.3 };
    for (int i = 0; i < N; i++) {
        //float height_couche = epsilon_z * (N - i) / (N - 1);
        a1 = a + vec3{ epsilon_left_c + epsilon_left, epsilon_y, percent_heigth[i] * roof_height};
        b1 = b + vec3{ -epsilon_left_c + epsilon_left, -epsilon_y, percent_heigth[i] * roof_height };
        c1 = c + vec3{ epsilon_right_c - epsilon_right, -epsilon_y, percent_heigth[i] * roof_height };
        d1 = d + vec3{ -epsilon_right_c - epsilon_right, epsilon_y, percent_heigth[i] * roof_height };

        m.push_back(mesh_primitive_quadrangle(b, a, a1, b1));
        m.push_back(mesh_primitive_quadrangle(c, b, b1, c1));
        m.push_back(mesh_primitive_quadrangle(d, c, c1, d1));
        m.push_back(mesh_primitive_quadrangle(a, d, d1, a1));

        for (int j = 0; j < 4; j++) {
            m.uv[16*i + 4 * j] = { 0 + ((epsilon_right_c + epsilon_right)  * i )/ length, crop_up};
            m.uv[16*i  +4*j +1] = { 1 - ((epsilon_left_c + epsilon_left) * i) /length, crop_up };
            m.uv[16*i +4*j + 2] = {1 - ((epsilon_left_c + epsilon_left) * (i+1)) / length, crop_up + percent_heigth[i] * height_image};
            m.uv[16*i +4*j + 3] = { ((epsilon_right_c + epsilon_right)* (i+1)) / length, crop_up + percent_heigth[i] * height_image };
        }

        crop_up += percent_heigth[i] * height_image;

        a = a1;
        b = b1;
        c = c1;
        d = d1;

    }


    m.push_back(mesh_primitive_quadrangle(d, c, b, a));
    m.color.fill(vec3{0.46f, 0.53f, 0.58f });

    //std::cout << m.position.size() << std::endl;
    //std::cout << "UV" << m.uv.size() << std::endl;
    return m;
    
}

mesh create_balcon(float length, float width, float height) {
    mesh m;
    float h_platform = 0.1f;

    //le sol
    float offset = 0.05f;
   
    vec3 p000 = vec3{ 0, -width, 0 };
    vec3 p100 = p000 + vec3{ length,0,0 };
    vec3 p110 = p000 + vec3{ length,width,0 };
    vec3 p010 = p000 + vec3{ 0,width,0 };
    vec3 p001 = p000 + vec3{ -offset,-offset,h_platform };
    vec3 p101 = p000 + vec3{ length+offset,-offset, h_platform };
    vec3 p111 = p000 + vec3{ length+offset,width,h_platform };
    vec3 p011 = p000 + vec3{ -offset,width,h_platform };

    m.push_back(mesh_primitive_quadrangle(p000, p100, p101, p001));
    m.push_back(mesh_primitive_quadrangle(p100, p110, p111, p101));
    m.push_back(mesh_primitive_quadrangle(p110, p010, p011, p111));
    m.push_back(mesh_primitive_quadrangle(p010, p000, p001, p011));
    m.push_back(mesh_primitive_quadrangle(p001, p101, p111, p011));
    m.push_back(mesh_primitive_quadrangle(p100, p000, p010, p110));


    //les côtes
    vec3 origin = p000 + vec3{ 0, 0, h_platform };
    m.push_back(mesh_primitive_quadrangle(origin, origin + vec3{ length, 0 , 0 }, origin + vec3{ length, 0, height }, origin + vec3{ 0,0, height }));
    m.push_back(mesh_primitive_quadrangle(origin, origin + vec3{ 0, 0, height }, origin + vec3{ 0, width, height }, origin + vec3{ 0, width, 0})); //cote droit
    m.push_back(mesh_primitive_quadrangle(origin + vec3{length, width, 0}, origin + vec3{length, width, height}, origin + vec3{length, 0, height}, origin + vec3{length, 0, 0})); //cote g

    m.color.fill(vec3{ 0.46f, 0.53f, 0.58f });

    //les en-dessous
    float y_spacing = 0.1f * width;
    float x_spacing = 0.2f * length;
    float y_offset = 0.3f * width; //width back
    float h_bast = 1.2 * h_platform;
    float e = 0.15f;
    vec3 l = vec3 (e, 0, 0);

    mesh bastaing1;
    mesh bastaing2;
    

    vec3 p1 = vec3{ 0, - width + y_spacing, 0 };
    vec3 p2 = vec3{ 0, - width + y_spacing + y_offset, -h_bast };
    vec3 p3 = vec3{ 0, 0, -h_bast };
    vec3 p4 = vec3{ 0, 0, 0 };



    bastaing1.push_back(mesh_primitive_quadrangle(p4 + l, p3 + l, p2 + l, p1 + l));
    bastaing1.push_back(mesh_primitive_quadrangle(p1 + l + l, p2 + l + l, p3 + l + l, p4 + l + l));
    bastaing1.push_back(mesh_primitive_quadrangle(p1 + l, p2 + l, p2 + l + l, p1 + l + l));
    bastaing1.push_back(mesh_primitive_quadrangle(p2 + l, p3 + l, p3 + l + l, p2 + l + l));
    bastaing1.push_back(mesh_primitive_quadrangle(p3 + l, p4 + l, p4 + l + l, p3 + l + l));


    vec3 f = vec3{length - x_spacing - e, 0, 0 };

    bastaing2.push_back(mesh_primitive_quadrangle(p4 + f, p3 + f, p2 + f, p1 + f));
    bastaing2.push_back(mesh_primitive_quadrangle(p1 + l + f, p2 + l + f, p3 + l + f, p4 + l + f));
    bastaing2.push_back(mesh_primitive_quadrangle(p1 + f, p2 + f, p2 + l + f, p1 + l + f));
    bastaing2.push_back(mesh_primitive_quadrangle(p2 + f, p3 + f, p3 + l + f, p2 + l + f));
    bastaing2.push_back(mesh_primitive_quadrangle(p3 + f, p4 + f, p4 + l + f, p3 + l + f));


    bastaing1.color.fill(vec3{ 0.87f, 0.81f, 0.75 });
    bastaing2.color.fill(vec3{ 0.87f, 0.81f, 0.75 });

    m.push_back(bastaing1);
    m.push_back(bastaing2);



    return m;
}








mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m; 
    int maximalSize = 50;

    m.position.resize(maximalSize + 2);

    m.position[maximalSize] = vec3{ 0, 0, height + z_offset};
    m.position[maximalSize + 1] = vec3{ 0, 0, z_offset};

    for (int i = 0; i < maximalSize; i += 1) {
        float x = radius * std::cos(i * 6.28 / (maximalSize - 1.00f));
        float y = radius * std::sin(i * 6.28 / (maximalSize - 1.00f));
        m.position[i] = vec3{ x, y, z_offset };
    }

    for (int j = 0 ; j + 1 < maximalSize; j += 1) {
        m.connectivity.push_back(uint3{ j + 1, maximalSize, j });
        m.connectivity.push_back(uint3{ j, maximalSize + 1, j +1 });
    }
    // To do: fill this mesh ...
    // ...

    m.fill_empty_field();
    return m;
}


mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.4f, 0.3f, 0.3f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r));   // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r));   // top-cone
    foliage.apply_translation_to_position({ 0,0,h });       // place foliage at the top of the trunk
    foliage.color.fill({ 0.4f, 0.6f, 0.3f });

    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}

std::vector<float> lengths_from_points(std::vector<vec2> points) {
    std::vector<float> lengths;
    for (int i = 0; i < points.size()-1; i++) {
        lengths.push_back(norm(points[i] - points[i + 1]));
    }
    lengths.push_back(norm(points[points.size() - 1] - points[0]));
    return lengths;
}


std::vector<vec2> directions_from_points(std::vector<vec2> points) {
    std::vector<vec2> directions;
    for (int i = 0; i < points.size() - 1; i++) {
        directions.push_back(points[i] - points[i + 1]);
    }
    directions.push_back(points[points.size() - 1] - points[0]);
    return directions;
}
