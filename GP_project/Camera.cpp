#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;

        //TODO - Update the rest of camera parameters
        this->cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraUp, cameraFrontDirection));
        
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection); //dynamic matrix
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction)
        {
        case MOVE_FORWARD:
            if ((cameraPosition + cameraFrontDirection * speed).y > 0.5) // check so you cant go below the map
            { 
                cameraPosition += cameraFrontDirection * speed;
                cameraTarget += cameraFrontDirection * speed;
            }
            break;
        case MOVE_BACKWARD:
            if ((cameraPosition - cameraFrontDirection * speed).y > 0.5) // check so you cant go below the map
            {
                cameraPosition -= cameraFrontDirection * speed;
                cameraTarget -= cameraFrontDirection * speed;
            }

            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            cameraTarget -= cameraRightDirection * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            cameraTarget += cameraRightDirection * speed;
            break;
        }
    }


    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    float scenePitch = 0.0f;
    float sceneYaw = 0.0f;
    void Camera::rotate(float pitch, float yaw) {
        scenePitch += pitch;
        sceneYaw += yaw;

        cameraTarget.x = cameraPosition.x + sin(glm::radians(sceneYaw));
        cameraTarget.z = cameraPosition.z - cos(glm::radians(sceneYaw));
        cameraTarget.y = cameraPosition.y + sin(glm::radians(scenePitch));
        cameraTarget.x = cameraPosition.x + sin(glm::radians(sceneYaw)) * cos(glm::radians(scenePitch));
        cameraTarget.z = cameraPosition.z - cos(glm::radians(sceneYaw)) * cos(glm::radians(scenePitch));

        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }
}