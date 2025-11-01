#include <application.hxx>

int main(int argc, char* argv[]){
    vec::Application* app = new vec::Application("MyApp", VK_MAKE_VERSION(1, 0, 0), VK_FALSE);
    app->running();
    delete app;
    return 0;
}
