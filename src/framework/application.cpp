#include "application.h"

#include "math/vec3.h"

#include "animations/pose.h"
#include "animations/skeleton.h"

Camera* Application::camera = nullptr;
Application* Application::instance;

void Application::init(GLFWwindow* window)
{
    instance = this;
    glfwGetFramebufferSize(window, &window_width, &window_height);

    flag_grid = true;
    flag_wireframe = false;

    // Create camera
    camera = new Camera();
    camera->look_at(vec3(0.f, 1.5f, 7.f), vec3(0.f, 0.0f, 0.f), vec3(0.f, 1.f, 0.f));
    camera->set_perspective(45.f, window_width / (float)window_height, 0.1f, 500.f);
    /* ADD ENTITIES TO THE SCENE */

// Add spheres
    Entity* dot_ent = new Entity("Dot Sphere");
    dot_ent->mesh = Mesh::get("res/meshes/sphere.obj");
    dot_ent->material = new FlatMaterial();
    dot_ent->set_transform(Transform(vec3(-3.f, 0.f, 0.f), quat(), vec3(1.f)));
    entity_list.push_back(dot_ent);

    Entity* cross_ent = new Entity("Cross Sphere");
    cross_ent->mesh = Mesh::get("res/meshes/sphere.obj");
    cross_ent->material = new NormalMaterial();
    cross_ent->set_transform(Transform(vec3(-1.f, 0.f, 0.f), quat(), vec3(1.f)));
    entity_list.push_back(cross_ent);

    Entity* quat_ent = new Entity("Quat Sphere");
    quat_ent->mesh = Mesh::get("res/meshes/sphere.obj");
    quat_ent->material = new NormalMaterial();
    quat_ent->set_transform(Transform(vec3(1.f, 0.f, 0.f), quat(), vec3(1.f)));
    entity_list.push_back(quat_ent);

    Entity* lerp_ent = new Entity("Lerp Sphere");
    lerp_ent->mesh = Mesh::get("res/meshes/sphere.obj");
    lerp_ent->material = new FlatMaterial();
    lerp_ent->set_transform(Transform(vec3(3.f, 0.f, 0.f), quat(), vec3(1.f)));
    entity_list.push_back(lerp_ent);

    // Then, add debug lines (so they are rendered on top of the previous entities)
    LineHelper* dot_l1 = new LineHelper(vec3(-3.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
    dot_l1->color = vec4(1.f, 0.f, 0.f, 1.f);
    dot_l1->unlocked = false;
    entity_list.push_back(dot_l1);

    LineHelper* dot_l2 = new LineHelper(vec3(-3.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f));
    entity_list.push_back(dot_l2);

    LineHelper* cross_l3 = new LineHelper(vec3(-1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
    cross_l3->color = vec4(1.f, 0.f, 0.f, 1.f);
    cross_l3->unlocked = false;
    entity_list.push_back(cross_l3);

    LineHelper* cross_l4 = new LineHelper(vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f));
    entity_list.push_back(cross_l4);

    LineHelper* cross_l5 = new LineHelper(vec3(-1.f, 0.f, 0.f), vec3(1.f, 0.f, 0.f));
    cross_l5->color = vec4(0.f, 1.f, 0.f, 1.f);
    cross_l5->unlocked = false;
    entity_list.push_back(cross_l5);

    LineHelper* quat_l6 = new LineHelper(vec3(-1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f));
    quat_l6->color = vec4(1.f, 0.f, 0.f, 1.f);
    quat_l6->unlocked = false;
    entity_list.push_back(quat_l6);

    LineHelper* quat_l7 = new LineHelper(vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 1.f));
    entity_list.push_back(quat_l7);

    LineHelper* quat_l8 = new LineHelper(vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.0f));
    entity_list.push_back(quat_l8);
}

void Application::update(float dt)
{
    float curr_time = glfwGetTime();

    // Exercise 1: 
    vec3 sphere_up = vec3(entity_list[0]->get_model().forward.x, entity_list[0]->get_model().forward.y, entity_list[0]->get_model().forward.z);
    vec3 sphere_forward = vec3(entity_list[0]->get_model().up.x, entity_list[0]->get_model().up.y, entity_list[0]->get_model().up.z);
    vec3 pos = vec3(entity_list[0]->get_model().position.x, entity_list[0]->get_model().position.y, entity_list[0]->get_model().position.z);
    entity_list[0]->material->color.y = dot(pos, sphere_forward);
    entity_list[0]->material->color.x = dot(pos, sphere_up);

    vec3 white = entity_list[7]->as<LineHelper>()->end;
    vec3 green = entity_list[8]->as<LineHelper>()->end;
    vec3 red = entity_list[6]->as<LineHelper>()->end;
    vec3 cross_product = cross(white, green);
    quat q1 = from_to(red,cross_product);
    Transform t1;
    t1.rotation = q1;
    entity_list[6]->set_transform(t1);
    vec3 cross_forward;
    cross_forward.x = entity_list[1]->get_model().forward.x;
    cross_forward.y = entity_list[1]->get_model().forward.y;
    cross_forward.z = entity_list[1]->get_model().forward.z;
    cross_product = cross(white, cross_product);


    q1 = from_to(cross_forward, cross_product);
    t1.rotation = q1;
    t1.position = vec3(-1.f, 0.f, 0.f);
    entity_list[1]->set_transform(t1);


    // Exercise 2: Quaternion to rotate linehelper
    vec3 line_vector = entity_list[11]->as<LineHelper>()->end; // line has origin 0,0,0 so we can rotate the end point of the line
    mat4 rotation_matrix = quat_to_mat4(quat(0.250, 0.250, 0.067, 0.933)); // Rotate 30 degrees in x and y axis
    vec4 rotated_line = rotation_matrix * vec4(line_vector.x, line_vector.y, line_vector.z,0);
    
    entity_list[11]->as<LineHelper>()->end = vec3(rotated_line.x, rotated_line.y, rotated_line.z);

    //quat q(-0.4, 0.0, -0.2, 0.8);
    //mat4 m = quat_to_mat4(q);
    //Transform t = entity_list[4]->get_transform();
    //mat4 model_q = entity_list[5]->get_model();
    //t.rotation = q;
    //model_q = model_q * m;
    //entity_list[4]->set_transform(t);
    //entity_list[5]->set_transform(t);
    
    
    //Excersice 3: Lerp

    mat4 model_lerp = entity_list[3]->get_model();

    vec3 position;
    position.x = model_lerp.position.x;
    position.y = model_lerp.position.y;
    position.z = model_lerp.position.z;

    vec3 original = vec3(3.f, 0.f, 0.f);
    vec3 wanted = vec3(3.f, 2.f, 0.f);
    vec3 new_position;
    if (!go_back) {
        new_position = lerp(position, wanted, 0.02);
    }
    else {
        new_position = lerp(position, original, 0.02);
    }

    if (original == position) go_back = false;
    else if (position == wanted) go_back = true;
    
    model_lerp.position = vec4(new_position, 0);
    entity_list[3]->set_model(model_lerp);


    // Update entities of the scene
    for (unsigned int i = 0; i < entity_list.size(); i++) {
        entity_list[i]->update(dt);
    }

    // Mouse update
    vec2 delta = last_mouse_position - mouse_position;
    if (orbiting) {
        camera->orbit(-delta.x * dt, delta.y * dt);
    }
    if (moving_2D) {
        camera->move(vec2(delta.x * dt, -delta.y * dt));
    }
    last_mouse_position = mouse_position;
}

void Application::render()
{
    // set the clear color (the background color)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set OpenGL flags
    glEnable(GL_CULL_FACE); // render both sides of every triangle
    glEnable(GL_DEPTH_TEST); // check the occlusions using the Z buffer

    for (unsigned int i = 0; i < entity_list.size(); i++)
    {         
        entity_list[i]->render(camera);        
    }

    // Draw the floor grid
    if (flag_grid) draw_grid();
}

void Application::render_gui()
{
    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Camera")) {
            camera->render_gui();
            ImGui::TreePop();
        }

        unsigned int count = 0;
        std::stringstream ss;
        for (auto& node : entity_list) {
            ss << count;
            if (ImGui::TreeNode(node->name.c_str())) {
                node->render_gui();
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}

void Application::shut_down() { }

// keycodes: https://www.glfw.org/docs/3.3/group__keys.html
void Application::on_key_down(int key, int scancode)
{
    switch (key) {
    case GLFW_KEY_ESCAPE: // quit
        close = true;
        break;
    case GLFW_KEY_R:
        Shader::reload_all();
        break;
    }
}

// keycodes: https://www.glfw.org/docs/3.3/group__keys.html
void Application::on_key_up(int key, int scancode)
{
    switch (key) {
    case GLFW_KEY_T:
        std::cout << "T released" << std::endl;
        break;
    }
}

void Application::on_right_mouse_down()
{
    moving_2D = true;
    last_mouse_position = mouse_position;
}

void Application::on_right_mouse_up()
{
    moving_2D = false;
    last_mouse_position = mouse_position;
}

void Application::on_left_mouse_down()
{
    orbiting = true;
    last_mouse_position = mouse_position;
}

void Application::on_left_mouse_up()
{
    orbiting = false;
    last_mouse_position = mouse_position;
}

void Application::on_middle_mouse_down() { }

void Application::on_middle_mouse_up() { }

void Application::on_mouse_position(double xpos, double ypos) { }

void Application::on_scroll(double xOffset, double yOffset)
{
    int min = camera->min_fov;
    int max = camera->max_fov;

    if (yOffset < 0) {
        camera->fov += 4.f;
        if (camera->fov > max) {
            camera->fov = max;
        }
    }
    else {
        camera->fov -= 4.f;
        if (camera->fov < min) {
            camera->fov = min;
        }
    }
    camera->update_projection_matrix();
}