#pragma once

#include "cgp/cgp.hpp"

using cgp::vec2;


cgp::mesh create_cylinder_mesh(float radius, float height);
cgp::mesh create_cone_mesh(float radius, float height, float z_offset);
cgp::mesh create_roof(float length, float width, float height,float roof_height, float e_left, float e_right, float e_y, float alpha_left, float alpha_right, float crop_up, float height_image);
cgp::mesh mesh_primitive_pave(float x, float y, float z, float length, float width, float height, float alpha_left, float alpha_right, float crop);
cgp::mesh create_balcon(float length, float width, float height);
cgp::mesh create_tree();


std::vector<float> lengths_from_points(std::vector<vec2> points);
std::vector<vec2> directions_from_points(std::vector<vec2> points);
