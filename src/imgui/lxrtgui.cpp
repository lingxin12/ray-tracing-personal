#include "lxrtgui.h"
#include "denoise.h"

namespace lxrt {

int LXRTGUI::Main(int argc, char* argv[], const std::string& output_path) {
    glfwSetErrorCallback([](int error, const char* description){ fprintf(stderr, "GLFW Error %d: %s\n", error, description); });
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    RenderingParameters data;
    data.Initialize();
    
    ImVec4 background(data.background[0], data.background[1], data.background[2], 1);
    ImVec4 look_from(data.look_from[0], data.look_from[1], data.look_from[2], 0);
    ImVec4 look_at(data.look_at[0], data.look_at[1], data.look_at[2], 0);

    GLuint textureID = GenerateImageID();
    LoadImage(output_path, textureID);

    // show setting
    bool is_rendering = false;
    bool is_load_output_image = true;
    int rendering_count = 0;
    int rendering_rate = 0;
    float output_scale = 1;
    std::string console_text = "waiting...";
    std::shared_ptr<RenderEngine> render_engine;
    std::shared_ptr<std::thread> render_thread = nullptr;

    // scene setting
    std::vector<std::string> scene_name;
    static int scene_id = 0;
    static int camera_id = 0;
    SceneLoadObject::get()->GetAllSceneName(scene_name);
    static char** char_scenes_name = (char**)malloc(sizeof(char*) * scene_name.size());
    static char** char_camera_name = (char**)malloc(sizeof(char*) * (scene_name.size() + 1));
    for(int i = 0; i < scene_name.size(); i ++) {
        char_scenes_name[i] = const_cast<char*>(scene_name[i].c_str());
        char_camera_name[i] = const_cast<char*>(scene_name[i].c_str());
    }
    char_camera_name[scene_name.size()] = const_cast<char*>(std::string("custom").c_str());
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if(!is_rendering)
        // parameters setting
        {
            ImGui::Begin("Parameters setting");
            ImGui::SetWindowSize("Parameters setting", ImVec2(400, 800));
            ImGui::SetWindowPos("Parameters setting", ImVec2(0, 0));
            
            {
                // image setting
                ImGui::SeparatorText("Image Setting");
                ImGui::InputInt("Image Width", &data.screen_width);
                // ImGui::SliderInt("Image Width", &data.screen_width, 100, 3840);
                ImGui::InputInt("Image Height", &data.screen_height);
                // ImGui::SliderInt("Image Height", &data.screen_height, 100, 2160);
                ImGui::SliderFloat("Output Scale", &output_scale, 0.5, 5);
                ImGui::ColorEdit3("Background", (float*)&background);

                data.background = vec3(background.x, background.y, background.z);
                data.aspect = double(data.screen_width / data.screen_height);
            }

            {
                // camera setting
                ImGui::SeparatorText("Camera Setting");
                ImGui::Combo("Presets", &camera_id, char_camera_name, scene_name.size() + 1);

                static int fov = data.fov;

                if(camera_id < scene_name.size()) {
                    RenderingParameters camera_data;
                    if(SceneLoadObject::get()->GetCamera(scene_name[camera_id], camera_data)) {
                        fov = camera_data.fov;
                        look_from = ImVec4(camera_data.look_from.x(), camera_data.look_from.y(), camera_data.look_from.z(), 0);
                        look_at = ImVec4(camera_data.look_at.x(), camera_data.look_at.y(), camera_data.look_at.z(), 0);
                        data.aperture = camera_data.aperture;
                        data.focus_distance = camera_data.focus_distance;
                    }
                }

                ImGui::SliderInt("FOV", &fov, 10, 80);
                ImGui::InputFloat3("Camera Position", (float*)&look_from);
                ImGui::InputFloat3("Look At", (float*)&look_at);
                ImGui::InputDouble("Aperture", &data.aperture);
                ImGui::InputDouble("Focus Distance", &data.focus_distance);

                data.fov = fov;
                data.look_from = vec3(look_from.x, look_from.y, look_from.z);
                data.look_at = vec3(look_at.x, look_at.y, look_at.z);
                data.look_up = vec3(0, 1, 0);
            }

            {
                // rendering setting
                ImGui::SeparatorText("Rendering Setting");
                ImGui::SliderInt("Samples per pixel", &data.samples_per_pixel, 5, 100);
                ImGui::SliderInt("rendering depth", &data.rendering_depth, 10, 150);
            }

            {
                // scene setting
                ImGui::SeparatorText("Scene Setting");
                ImGui::Combo("Scene", &scene_id, char_scenes_name, scene_name.size());
                static std::string scene_message;
                SceneLoadObject::get()->GetScene(scene_name[scene_id], scene_message);
                ImGui::Text("%s", scene_message.c_str());
            }

            {
                // render
                ImGui::SeparatorText("Render");
                if(ImGui::Button("Render")) {
                    is_rendering = true;
                    ++ rendering_count;
                }
                ImGui::Text("%s", console_text.c_str());
            }

            {
                // Denoise
                static int sample_radius = 5;

                static float sigma_distance = 2;
                static float sigma_color = 2;
                static float sigma_depth = 2;
                static float sigma_normal = 2;

                static bool open_distance = false;
                static bool open_color = false;
                static bool open_depth = false;
                static bool open_normal = false;

                static int choose_model = 0;
                
                static int denoise_int = 0;
                static std::string denoise_text_ok("denoise ending + ");
                static std::string denoise_text("waiting");

                ImGui::SeparatorText("Denoise Setting");
                ImGui::Combo("model", &choose_model, "guass\0\0");
                ImGui::SliderInt("radius", &sample_radius, 0, 20);
                ImGui::Text("distance:");
                ImGui::Checkbox("o1", &open_distance); ImGui::SameLine(); ImGui::DragFloat("dis_sigma", &sigma_distance, 0.1f, 0.1f, 100.0f);
                ImGui::Text("color:");
                ImGui::Checkbox("o2", &open_color); ImGui::SameLine(); ImGui::DragFloat("col_sigma", &sigma_color, 0.1f, 0.1f, 100.0f);
                ImGui::Text("depth:");
                ImGui::Checkbox("o3", &open_depth); ImGui::SameLine(); ImGui::DragFloat("dep_sigma", &sigma_depth, 0.1f, 0.1f, 100.0f);
                ImGui::Text("normal:");
                ImGui::Checkbox("o4", &open_normal); ImGui::SameLine(); ImGui::DragFloat("nor_sigma", &sigma_normal, 0.1f, 0.1f, 100.0f);
                if(ImGui::Button("Denoise")) {
                    Denoise denoise(sample_radius,
                                    open_distance, open_color, open_depth, open_normal,
                                    sigma_distance, sigma_color, sigma_depth, sigma_normal);
                    denoise.Run(std::string("output/output_png.txt"),       // input image
                                std::string("output/depth_output.png"),     // depth image
                                std::string("output/normal_output.png"),    // normal image
                                std::string("output/output.png"));          // output image
                    is_load_output_image = false;  // show image
                    denoise_text = denoise_text_ok + std::to_string(denoise_int++);
                }
                ImGui::Text(denoise_text.c_str());
            }
            

            ImGui::End();
        } // end if
        else
        {
            ImGui::Begin("Rendering");
            ImGui::SetWindowSize("Rendering", ImVec2(400, 800));
            ImGui::SetWindowPos("Rendering", ImVec2(0, 0));

            if(rendering_count >= 1) {
                render_engine = std::make_shared<RenderEngine>();
                render_engine->LoadRenderData(data);
                render_engine->SetSceneName(scene_name[scene_id]);
                render_thread = std::make_shared<std::thread>(&RenderEngine::Render, render_engine);
                render_thread->detach();
                -- rendering_count;
            }

            int now_processing = render_engine->GetNowProcessing();

            ImGui::ProgressBar((float)now_processing/100.0, ImVec2(0, 0));
            ImGui::Text("rendering");

            RenderEngine::RenderReturnType render_return = render_engine->GetRenderReturn();
            if(render_return == RenderEngine::RenderReturnType::RepeatUse) {
                console_text = "ERROR: the render engine use repeatly!";
            } else if(render_return == RenderEngine::RenderReturnType::SceneError) {
                console_text = "ERROR: scene can't load!";
            } else if(render_return == RenderEngine::RenderReturnType::RenderComplete && now_processing >= 100) {
                is_rendering = false;
                render_thread = nullptr;
                render_engine = nullptr;
                is_load_output_image = false;
                console_text = "render complete!";
            }

            ImGui::End();
        }

        // image show
        {
            ImGui::Begin("Result show");
            ImGui::SetWindowSize("Result show", ImVec2(800, 800));
            ImGui::SetWindowPos("Result show", ImVec2(400, 0));

            if(!is_rendering) {
                if(!is_load_output_image) {
                    LoadImage("output/output.png", textureID);
                    is_load_output_image = true;
                }
                ImGui::Image((void*)textureID, ImVec2(data.screen_width * output_scale, data.screen_height * output_scale), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,0,0,0));
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    free(char_scenes_name);
    free(char_camera_name);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

} // namespace lxrt