#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "key_positions_structure.hpp"


// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::vec2;
using cgp::numarray;
using cgp::timer_basic;

// Variables associated to the GUI
struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_orbit_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable terrain;

	cgp::mesh_drawable tree;

	cgp::mesh_drawable cone;

	std::vector< std::vector<vec2> > blocks;
	std::vector<vec3> origin_blocks;

	std::vector<std::vector<float>> lengths;

	float width;
	float height;
	std::vector<std::vector<cgp::vec2>> directions;


	std::vector<std::vector<cgp::hierarchy_mesh_drawable>> buildings;


	std::vector < cgp::mesh_drawable> bat;
	std::vector < cgp::mesh_drawable> roof;
	cgp::mesh_drawable balcon;

	std::vector<cgp::vec3> tree_position;

	cgp::mesh_drawable grass;

	std::vector<cgp::vec3> grass_position;

	cgp::mesh_drawable bird;

	cgp::hierarchy_mesh_drawable hierarchy;


	// A helper structure used to store and display the key positions/time
	keyframe_structure keyframe;

	// Timer used for the interpolation of the position
	cgp::timer_interval timer;

	vec3 p0;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

	void display_semiTransparent();

	void create_bird();

	std::vector <int> bat_lengths;
	std::vector <int> num_list;
};

class myRectangle
{
public:
	myRectangle(float x, float y, float l, float h);
	
	float x;
	float y;
	float l;
	float h;

	
		/* {
		this->x = x;
		this->y = y;
		this->l = l;
		this->h = h;
	}
	*/
};





