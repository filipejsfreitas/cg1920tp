#include <iostream>

#include <tinyxml2/tinyxml2.h>

#include "Scene.h"
#include "window/Window.h"
#include "window/cameras/FpsCamera.h"

using std::cerr, std::endl, std::cout;
using tinyxml2::XMLDocument, tinyxml2::XMLNode, tinyxml2::XML_SUCCESS;
using engine::Scene;
using engine::window::Window;
using engine::window::cameras::FpsCamera;

XMLNode* ReadSceneFile(const char*);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "Engine requer exatamente 1 argumento, o ficheiro de configuração XML." << endl;
        cerr << "Uso ./engine <file.xml>" << endl;
        return -1;
    }

    // Initialize GLFW
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW! Exitting..." << endl;
        exit(-1);
    }

	// Setup the window and OpenGL Context
	// This must happen before we attempt to parse the scene file
	//  because the scene parsing attempts to use OpenGL functions to
	//  upload vertex data to the graphics card!
    Window window("CG@DI-UM", 1280, 720);
    window.CreateWindow();

    // Find out if the scene file exists
    XMLNode* root = ReadSceneFile(argv[1]);
    if(root == nullptr) {
        return -1;
    }

    // Parse the scene file
    Scene scene;
    if(!scene.ParseXml(root)) {
        cerr << "Failed to parse Scene's XML!" << endl;
        return -1;
    }

    // Delete the XMLDocument as its no longer needed
    delete root->GetDocument();

    // Set the window parameters
    // window.SetCamera(new StaticCamera(glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0)));
    window.SetCamera(new FpsCamera);
    window.SetScene(&scene);

    // Start main loop
    window.MainLoop();

    // Free up resources
    delete window.GetCamera();
    glfwTerminate();

    return 0;
}

XMLNode* ReadSceneFile(const char* filename) {
    XMLDocument* document = new XMLDocument;

    if(document->LoadFile(filename) != XML_SUCCESS) {
        cerr << "Ocorreu um erro na leitura do ficheiro de configuração XML fornecido." << endl;
        return nullptr;
    }

    XMLNode* root = document->FirstChild();

    if(strcmp("scene", root->Value()) != 0) {
        cerr << "Esperado valor 'scene', obtido '" << root->Value() << "'" << endl;
        return nullptr;
    }

    return root;
}

// Código específico para Windows:
// Em portáteis com placas gráficas integradas e dedicadas, este código dá hints às drivers
// para que estas escolham a placa gráfica dedicada para executar a nossa engine.
// Não tem qualquer efeito em Linux ou macOS, e portanto, não é incluido nos executáveis gerados
// para essas plataformas.
#ifdef WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
