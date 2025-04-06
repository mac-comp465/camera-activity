#ifndef APP_H
#define APP_H

#include <api/MinVR.h>
using namespace MinVR;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#ifdef _WIN32
#include "GL/glew.h"
#include "GL/wglew.h"
#elif (!defined(__APPLE__))
#include "GL/glxew.h"
#endif

// OpenGL Headers
#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include <BasicGraphics.h>

#include "Skybox.h"
#include "Camera.h"


class App : public VRApp {
public:
    
    /** The constructor passes argc, argv, and a MinVR config file on to VRApp.
     */
	App(int argc, char** argv);
    virtual ~App();

    
    /** USER INTERFACE CALLBACKS **/
    virtual void onAnalogChange(const VRAnalogEvent &state);
    virtual void onButtonDown(const VRButtonEvent &state);
    virtual void onButtonUp(const VRButtonEvent &state);
	virtual void onCursorMove(const VRCursorEvent &state);
    
    
    /** RENDERING CALLBACKS **/
    virtual void onRenderGraphicsScene(const VRGraphicsState& state);
    virtual void onRenderGraphicsContext(const VRGraphicsState& state);
    
private:
    
    virtual void reloadShaders();
    basicgraphics::GLSLProgram _shader;

    std::shared_ptr<basicgraphics::Texture> environmentMap;
    std::shared_ptr<basicgraphics::Skybox> skyBox;
    
    std::shared_ptr<basicgraphics::Texture> cowTexture;
    
    std::unique_ptr<basicgraphics::Model> modelMesh;
    std::shared_ptr<basicgraphics::Camera> camera;
    
    glm::vec4 lightPosition;
    bool drawLightVector;
    
};


#endif //APP_H
