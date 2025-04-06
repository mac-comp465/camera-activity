#include "App.h"

#include <config/VRDataIndex.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

using namespace basicgraphics;
using namespace std;
using namespace glm;

App::App(int argc, char** argv) : VRApp(argc, argv)
{
    camera.reset(new Camera(vec3(-5, 0.6f, 0), vec3(0, 0, -1), vec3(0, 1, 0), glm::radians(45.0f), 1.0f, 0.01f, 100.0f));
}

App::~App()
{
	shutdown();
}

void App::onAnalogChange(const VRAnalogEvent &event) {
    camera->onAnalogChange(event);
}

void App::onButtonDown(const VRButtonEvent &event) {
    camera->onButtonDown(event);
}

void App::onButtonUp(const VRButtonEvent &event) {
    camera->onButtonUp(event);
}

void App::onCursorMove(const VRCursorEvent &event) {
    camera->onCursorMove(event);
}

void App::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}
		#endif
        
        // Setup the projection matrix so that things are rendered in perspective
        GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
        GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");
        camera.reset(new Camera(vec3(-5, 0.6f, 0), vec3(0, 0, -1), vec3(0, 1, 0), glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f));
        camera->lookAt(vec3(0.0));


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

        // This loads the dragon model from a file and initializes an instance of the model class to store it
        modelMesh.reset(new Model("spot_triangulated_reduced.obj", 1.0, vec4(1.0)));
        
        cowTexture = Texture::create2DTextureFromFile("spot_texture.png");
        
        
        // Initialize the texture environment map
        // Order should be:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front)
        // -Z (back)
        string textureFiles[] = {"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"};
        environmentMap = Texture::createCubeMapFromFiles(textureFiles, false, 1);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        environmentMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        skyBox.reset(new Skybox(environmentMap));
        
        
        // set the light position to roughly match up with the sun position in the skybox texture
        float radius = 10.0;
        lightPosition = vec4(-1.7*radius, 0.3*radius, -1.0*radius, 1.0);
        drawLightVector = false;
    }
}

void App::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = camera->getView();
    //std::cout<<glm::to_string(view)<<std::endl;
    glm::mat4 model(1.0);
    
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = camera->getProjection();
    
    
    // Tell opengl that we want to use the shader
    _shader.use();
    _shader.setUniform("view_mat", view);
    _shader.setUniform("projection_mat", projection);
    _shader.setUniform("model_mat", model);
    
    vec3 eyePosition = camera->getPos();
    _shader.setUniform("eye_world", eyePosition);
    
    vec3 ambientReflectionCoeff(0.7, 0.7, 0.7);
    vec3 diffuseReflectionCoeff(0.9, 0.9, 0.9);
    vec3 specularReflectionCoeff(0.9, 0.9, 0.9);
    
    // Properties of the light source (the "I" terms in the equations discussed in class)
    // These values are for a white light so the r,g,b intensities are all the same
    vec3 ambientLightIntensity(0.3, 0.3, 0.3);
    vec3 diffuseLightIntensity(0.7, 0.7, 0.7);
    vec3 specularLightIntensity(0.9, 0.9, 0.9);
    
    _shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    _shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    _shader.setUniform("specularReflectionCoeff", specularReflectionCoeff);
    _shader.setUniform("shinyness", 20.0f);

    _shader.setUniform("lightPosition", lightPosition);
    _shader.setUniform("ambientLightIntensity", ambientLightIntensity);
    _shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    _shader.setUniform("specularLightIntensity", specularLightIntensity);
    
    cowTexture->bind(1);
    _shader.setUniform("textureSampler", 1);
 
    // Draw the model
    modelMesh->draw(_shader);
    
    // Draw the skybox. Should be the last thing to draw
    skyBox->draw(view, projection);
    

}

void App::reloadShaders()
{
    _shader.compileShader("phong.vert", GLSLShader::VERTEX);
    _shader.compileShader("phong.frag", GLSLShader::FRAGMENT);
    _shader.link();
    _shader.use();
}
