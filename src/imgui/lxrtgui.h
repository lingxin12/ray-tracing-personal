// #pragma once
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>

#include "render_engine.h"

namespace lxrt {

class LXRTGUI {
public:
    LXRTGUI() { }

    static int Main(int argc, char* argv[], const std::string& output_path);

private:
    static GLuint GenerateImageID() {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        float border_color[] = {0.35f, 0.35f, 0.35f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return textureID;
    }

    static void LoadImage(const std::string& path, GLuint textureID) {
        // stbi_set_flip_vertically_on_load(true);
        int nWidth = 0, nHeight = 0, nChannel = 0;
        unsigned char* result = stbi_load(path.c_str(), &nWidth, &nHeight, &nChannel, 0);

        if (result)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, result);
        }
        else
        {
            printf("Failed to load texture");
        }
        stbi_image_free(result);
    }
};

} // namespace lxrt