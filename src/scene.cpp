#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"
#include "interpolation.hpp"
#include <iostream>
#include <fstream>
using namespace cgp;

float dt = 0.1f;





//int n_bat = 5;

myRectangle::myRectangle(float x, float y, float l, float h)
{
	this->x = x;
	this->y = y;
	this->l = l;
	this->h = h;
}




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 15.0f,6.0f,6.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	int N_terrain_samples = 100;
	float terrain_length = 20;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.material.color = { 0.6f,0.85f,0.5f };
	terrain.material.phong.specular = 0.0f; // non-specular terrain material

	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg",
		GL_REPEAT,
		GL_REPEAT);


	
	



	std::string PATH = "assets/rectangle_info/";

	

	std::vector <myRectangle> rect_all;
	//rect_all.resize(76);
	std::vector <std::vector <myRectangle>> rect_balc;
	//rect_balc.resize(76);
	std::vector <myRectangle> rect_roof;
	//rect_roof.resize(76);

	
	float facteur = 20.0f;

	/* Initialisation des données */


	//bat_lengths.resize(76);
	//std::cout << bat_lengths2.size() << std::endl;

	for (int num = 1; num < 80; num++) {
		

		std::ifstream file_all(PATH + "all_" + std::to_string(num) + ".txt");
		std::ifstream file_balc(PATH + "balc_" + std::to_string(num) + ".txt");
		std::ifstream file_roof(PATH + "roof_" + std::to_string(num) + ".txt");
		

		

		int x, y, w, h;

		//Building coordinates
		std::vector<myRectangle> rect_all_num;
		while (file_all >> x >> y >> w >> h) { 
			rect_all_num.push_back(myRectangle(x / facteur, y / facteur, w / facteur, h / facteur)); }
		rect_all.push_back(rect_all_num[0]);
		bat_lengths.push_back(rect_all_num[0].l);


		
		//Balcony coordinates
		std::vector <myRectangle> rect_balc_num;
		while (file_balc >> x >> y >> w >> h) {
			rect_balc_num.push_back(myRectangle(x / facteur, y / facteur, w / facteur, h / facteur));
			//std::cout << x << "/" << y << "/" << w << "/" << h << std::endl;
		}
		rect_balc.push_back(rect_balc_num);


		//Roof coordinates
		std::vector <myRectangle> rect_roof_num;
		while (file_roof >> x >> y >> w >> h) {
			rect_roof_num.push_back(myRectangle(x / facteur, y / facteur, w / facteur, h / facteur));
		}
		rect_roof.push_back(rect_roof_num[0]);

		
	}
	std::cout << "Les données sont initialisées" << std::endl;
	
	assert(rect_all.size() == rect_roof.size());
	assert(rect_all.size() == rect_balc.size());


	/*Creation des batiments*/


		


		//blocks.push_back( { vec2{2, 0}, vec2{5, 0}, vec2{5, 6}, vec2{1, 6}, vec2{0, 5} } );
		//blocks.push_back({ vec2{0, 0}, vec2{5, 0}, vec2{0, 5} });
		//blocks.push_back({ vec2{0, 0}, vec2{5, 0}, vec2{5, 6}, vec2{1, 6}, vec2{0, 5} });

		//int L = 5;



		

		//int n_bat = points.size();

		//int building_size = 0;

		

		float e_y = 0.2f;


		//int L = 150;
		//blocks.push_back({ vec2{0, 0 }, vec2{L, 0}, vec2{L, L}, vec2{0, L} });
		num_list = { 2, 3, 4, 5, 6, 7, 8, 9, 10};
		int pas = 60;
		origin_blocks = { vec3{-2*pas, 0, 0}, vec3{-pas, 0, 0}, vec3{ 0, 0, 0 }, vec3{ pas, 0, 0 }, vec3{2*pas, 0, 0}, vec3{3*pas, 0, 0} };
		int N_max = num_list.size();
		buildings.resize(N_max);


		for (int N = 0; N < N_max; N++) {

			//int num = numbers[N];
			//int num = num_list[N];
			std::vector<vec2> points = { vec2{ 354, 27 }, vec2{384, 19}, vec2{ 403, 14 }, vec2{ 408, 36 }, vec2{ 384, 42 }, vec2{ 390, 70 }, vec2{ 364, 77 },  vec2{ 354, 27 } };


			//lengths.push_back(lengths_from_points(points));

			width = 5.0f;
			//std::cout << "Lengths" << lengths.size() << std::endl;


			std::vector<cgp::vec2> direction = directions_from_points(points);
			directions.push_back(direction);

			//std::cout << "Directions" << directions.size() << std::endl;


			float dot_final = direction[points.size() - 1].x * direction[0].x + direction[points.size() - 1].y * direction[0].y;
			float det_final = direction[points.size() - 1].x * direction[0].y - direction[points.size() - 1].y * direction[0].x;
			//float det = x1 * y2 - y1 * x2;      //Determinant
			float a_final = atan2(det_final, dot_final);

			float a_left = a_final;
			float e_left = 0;
			float a_right;
			float e_right = 0;



			bat.resize(points.size()-1);
			roof.resize(points.size()-1);


			//building_size = building.size();
			buildings[N].resize(points.size()-1);


			/* Iteration on points of the block */
			for (int i = 0; i < points.size() - 1; i++) {
				int num = num_list[i];
				//int length = rect_all[num-1].l;
				std::vector<float> lengths = lengths_from_points(points);
				//int height = rect_all[num-1].h;
				int height = rect_all[0].h;
				
				if (i == points.size() - 2) {

					a_right = a_final;
					e_right = 0;
					//std::cout << a_right * 180 / 3.14 << std::endl;


				}
				else {
					//compute angle between the two building
					float dot_right = direction[i].x * direction[i + 1].x + direction[i].y * direction[i + 1].y;
					float det_right = direction[i].x * direction[i + 1].y - direction[i].y * direction[i + 1].x;

					//Determinant
					a_right = atan2(det_right, dot_right);
					e_right = 0;
				}

				/* Batiment généré */
				float crop = (rect_roof[num-1].y + rect_roof[num-1].h) / (rect_all[num-1].y + rect_all[num-1].h);

				//std::cout << rect_roof[num-1].y << " /"<< rect_roof[num-1].h << " /"<< std::endl;
				mesh bat_mesh = mesh_primitive_pave(0, 0, 0, lengths[i], width, height, a_left, a_right, 1 - crop);
				bat_mesh.color.fill(vec3{ 0.87f, 0.81f, 0.75 });

				bat[i].initialize_data_on_gpu(bat_mesh);
				bat[i].texture.load_and_initialize_texture_2d_on_gpu("assets/images/facade_" + std::to_string(num) + ".png", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);


				//float crop2 = rect_roof[N].y / (rect_all[N].y + rect_all[N].h);
				
				/* Generate roof */
				mesh roof_mesh = create_roof(lengths[i], width, height, rect_roof[num - 1].h, e_left, e_right, e_y, a_left, a_right, 1 - crop, rect_roof[num - 1].h / (rect_all[num - 1].y + rect_all[num - 1].h));
				roof[i].initialize_data_on_gpu(roof_mesh);
				roof[i].texture.load_and_initialize_texture_2d_on_gpu("assets/images/facade_" + std::to_string(num) + ".png", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
				//roof[i].material.color(0.1f);

				a_left = a_right;
				e_left = 0;



				//float balcon_length = rect_balc[0].w;



				buildings[N][i].add(bat[i], "Batiment");
				buildings[N][i].add(roof[i], "Roof", "Batiment");

				/* Creating balconies */
				for (int j = 0; j < rect_balc[num-1].size(); j++) {
					myRectangle un_balcon = rect_balc[num-1][j];
					float facteur_balc = lengths[i] / bat_lengths[num - 1];


					mesh balcon_mesh = create_balcon(un_balcon.l * facteur_balc, 1.0f, un_balcon.h);
					balcon.initialize_data_on_gpu(balcon_mesh);
					
					buildings[N][i].add(balcon, "Balcon" + std::to_string(j), "Batiment", { un_balcon.x * facteur_balc, 0, rect_all[num-1].h + rect_all[num-1].y - un_balcon.y - un_balcon.h });

				}
				



			}

		}

	
	// Definition of the initial data
	//--------------------------------------//



	
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	//draw(terrain, environment);

	for (int i = 0; i < tree_position.size(); i++) {
		tree.model.translation = tree_position[i];
		draw(tree, environment);
	}
	
	//draw(cone, environment);


	
	
	vec3 road_dist = vec3{ 0, 0, 0};
	int building_size = 0;
	int N_max = num_list.size();

	for (int N = 0; N < N_max; N++) {
		road_dist = origin_blocks[N];
		//std::vector<vec2> points = blocks[N];

		int num = num_list[N];
		float L = bat_lengths[num - 1];
		std::vector<vec2> points = { vec2{ 354, 27 }, vec2{384, 19}, vec2{ 403, 14 }, vec2{ 408, 36 }, vec2{ 384, 42 }, vec2{ 390, 70 }, vec2{ 364, 77 } };
		std::vector<float> lengths = lengths_from_points(points);

		for (int i = 0; i < points.size(); i++) {

			
			//std::cout << building_size + i << std::endl;
			buildings[N][i]["Batiment"].transform_local.translation = vec3{ road_dist.x, road_dist.y, 0 };
			buildings[N][i]["Batiment"].transform_local.rotation = rotation_transform::from_vector_transform(vec3{ 1, 0, 0 }, normalize(vec3{ directions[N][i].x, directions[N][i].y, 0 }));


			buildings[N][i].update_local_to_global_coordinates();

			draw(buildings[N][i], environment);

			road_dist += lengths[i] * normalize(vec3{ directions[N][i].x, directions[N][i].y, 0 });
			

		}
		building_size += points.size();
	}

	

	if (gui.display_wireframe)
		draw_wireframe(terrain, environment);

	display_semiTransparent();

	// Update the current time
	timer.update();
	float t = timer.t;




	

}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_semiTransparent()
{
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	auto const& camera = camera_control.camera_model;



	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}

