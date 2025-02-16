#include <iostream>
#include <memory>

#include <tinyxml2/tinyxml2.h>

#include <IL/il.h>
#include <string>

#include "scene/Scene.h"
#include "window/Window.h"
#include "window/cameras/FpsCamera.h"
#include "window/cameras/ExplorerCamera.h"

using std::cerr, std::endl, std::cout;
using std::unique_ptr, std::string;
using tinyxml2::XMLDocument, tinyxml2::XMLNode, tinyxml2::XML_SUCCESS;
using engine::scene::Scene;
using engine::window::Window;
using engine::window::cameras::FpsCamera;
using engine::window::cameras::ExplorerCamera;

unique_ptr<XMLDocument> ReadSceneFile(const char*);

/**
 * Ponto de entrada do programa.
 *
 * Responsável pela inicialização das bibliotecas necessárias ao funcionamento do mesmo,
 * pela criação de uma janela para desenhar, e pela leitura inicial do ficheiro XML da cena.
 */

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "Engine requer exatamente 1 argumento, o ficheiro de configuração XML." << endl;
        cerr << "Uso ./engine <file.xml>" << endl;
        return -1;
    }

    // Initialize GLFW
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW! Exiting..." << endl;
        exit(-1);
    }

    // Initialize DevIL
    ilInit();

	// Setup the window and OpenGL Context
	// This must happen before we attempt to parse the scene file
	//  because the scene parsing attempts to use OpenGL functions to
	//  upload vertex data to the graphics card!
    Window window("CG@DI-UM", 1280, 720);
    window.CreateWindow();

    // Find out if the scene file exists
    auto root = ReadSceneFile(argv[1]);
    if(root == nullptr) {
        return -1;
    }

    // Load the scene
    window.SetScene(nullptr, string(argv[1]));
    window.ReloadScene();

    // Start main loop
    window.MainLoop();

    // Free up resources
    delete window.GetCamera();
    delete window.GetScene();
    glfwTerminate();

    return 0;
}

unique_ptr<XMLDocument> ReadSceneFile(const char* filename) {
    auto document = unique_ptr<XMLDocument>(new XMLDocument);

    if(document->LoadFile(filename) != XML_SUCCESS) {
        cerr << "Ocorreu um erro na leitura do ficheiro de configuração XML fornecido." << endl;
        return nullptr;
    }

    return document;
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
