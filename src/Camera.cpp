#include "Camera.h"

using namespace std;
using namespace glm;

namespace basicgraphics{

const float X_SCALE = 0.005;
const float Y_SCALE = 0.005;

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float viewAngle, float aspectRatio, float near, float far) : position(position), viewAngle(viewAngle), aspectRatio(aspectRatio), nearClip(near), farClip(far), mouseDown(false), lastMousePos(vec2(0.0)), lastTime(0), currentFrameTime(0) {
    this->forward = normalize(forward);
    this->up = normalize(up);
}

void Camera::lookAt(glm::vec3 lookAtPoint) {
    //TODO: update the camera variables to make it look at the lookAtPoint
    
}

glm::mat4 Camera::getView() const {
    //TODO: return the camera's view matrix. You may not use the glm::lookAt function!
    // Hint: remember when calculating the u,v,n axes that the up vector may not be orthogonal
    //       to the forward look vector.
    return mat4(1.0); // Fix me
}

glm::mat4 Camera::getProjection() const {
    //TODO: Return the perspective projection matrix. You may not use the glm::perspective function!
    return mat4(1.0); // Fix me!
}

void Camera::onAnalogChange(const VRAnalogEvent &event) {
    if (event.getName() == "FrameStart") {
        lastTime = currentFrameTime;
        currentFrameTime = event.getValue();
        move();
    }
}
    
void Camera::onButtonDown(const VRButtonEvent &event) {
    string name = event.getName();
    if (name == "MouseBtnLeft_Down") {
        mouseDown = true;
        yaw = 0;
        pitch = 0;
        startForward = forward;
    }
}

void Camera::onButtonUp(const VRButtonEvent &event) {
    string name = event.getName();
    if(name == "MouseBtnLeft_Up"){
        mouseDown = false;
    }
}

void Camera::move(){
    float deltaTime = currentFrameTime - lastTime;
    
    //TODO: (Optional) based on which keys are held down (ASWD) adjust the position to move the camera forward/backward or left/right. Move at a constant speed independent of the framerate.

    
    
}

void Camera::onCursorMove(const VRCursorEvent &event) {
    if (mouseDown){
        vec2 dxy = vec2(event.getPos()[0], event.getPos()[1]) - lastMousePos;
        if(length(dxy) != 0.0){
            yaw += dxy.x * X_SCALE;
            pitch += dxy.y * Y_SCALE;
            if (pitch > 90.0f){
                pitch = 90.0f;
            }
            else if (pitch < -90.0f){
                pitch = -90.0f;
            }
            
            vec3 right = cross(up, -startForward);
            mat4 rotation = rotate(mat4(1.0), pitch, right) * rotate(mat4(1.0), yaw, cross(-startForward, right));
            forward = (rotation * vec4(startForward, 1.0f)).xyz();
        }
    }
    lastMousePos = vec2(event.getPos()[0], event.getPos()[1]);
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Camera::getPos() const {
    return position;
}

void Camera::setViewAngle(float viewAngle) {
    this->viewAngle = viewAngle;
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Camera::setNearClip(float nearClip) {
    this->nearClip = nearClip;
}

void Camera::setFarClip(float farClip) {
    this->farClip = farClip;
}

}//namespace
