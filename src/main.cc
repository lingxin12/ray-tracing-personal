#include <iostream>

#include "lxrtgui.h"
#include "render_engine.h"

using namespace lxrt;


int main(int argc, char *argv[])
{
    SceneLoadObject::get()->Load();

    LXRTGUI::Main(argc, argv, "./output/output.png");
    
    return 0;
}