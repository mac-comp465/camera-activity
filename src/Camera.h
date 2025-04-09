#ifndef Camera_h
#define Camera_h

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <memory>

#include <api/MinVR.h>
using namespace MinVR;

namespace basicgraphics{
    
class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float viewAngle, float aspectRatio, float near, float far);
    virtual glm::mat4 getView() const;
    virtual glm::mat4 getProjection() const;
    virtual void onAnalogChange(const VRAnalogEvent &state);
    virtual void onButtonDown(const VRButtonEvent &state);
    virtual void onButtonUp(const VRButtonEvent &state);
    virtual void onCursorMove(const VRCursorEvent &state);
    void setPosition(glm::vec3 position);
    glm::vec3 getPos() const;
    void lookAt(glm::vec3 lookAtPoint);
    void setViewAngle(float viewAngle);
    void setAspectRatio(float aspectRatio);
    void setNearClip(float nearClip);
    void setFarClip(float farClip);
    void move();
    

protected:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    
    float viewAngle;
    float aspectRatio;
    float nearClip;
    float farClip;
    
    bool mouseDown;
    glm::vec2 lastMousePos;
    float yaw;
    float pitch;
    glm::vec3 startForward; //Position of the forward vector when the mouse is first pressed.
    
    float lastTime;
    float currentFrameTime;
};
}

#endif
