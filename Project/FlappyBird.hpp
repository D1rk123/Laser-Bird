#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H
#define _USE_MATH_DEFINES
#include "GameObject.hpp"
#include "Mesh.hpp"
#include "glm/ext.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

class FlappyBird : public GameObject
{
    float gravity;
    float outOfScreenSpring;
    float flyVelocity;
    float maxVelocity;
    float jumpSpeed;

    public:
        FlappyBird() : GameObject(){
            gravity = 9.81f;
            outOfScreenSpring = 20.0f;
            flyVelocity = 10.0f;
            maxVelocity = 25.0f;
            jumpSpeed = 10.0f;
        }

        void setFlyVelocity(float vel) {
            flyVelocity = vel;
        }

        float getFlyVelocity() {
            return flyVelocity;
        }

        void startFlying() {
            setVelocity(glm::vec3(flyVelocity,0.0f,0.0f));
            orientation[3][1] = 2;
        }

        void jump() {
            increaseVelocityY(jumpSpeed);
        }

        void increaseFallVelocity(float seconds, float height) {
            increaseVelocityY(-seconds*gravity);
            if (orientation[3][1] >= 0.7*height) {
                increaseVelocityY(-seconds*outOfScreenSpring*(orientation[3][1] - 0.7*height));
            }
        }

        bool touchingGround(float height) {
            float radiusFlappy = mesh->getEllipsoid().radius;
            return (orientation[3][1] < -height+radiusFlappy);
        }

        void update(float elapsedTime, float height) {
            // update flymovement
            increaseFallVelocity(elapsedTime, height);
            glm::vec3 position = glm::vec3(orientation[3][0], orientation[3][1], orientation[3][2]);
            glm::vec3 update = velocity*elapsedTime;
            if(velocity[1] > maxVelocity) {
                velocity[1] = maxVelocity;
            } else if (velocity[1] < -maxVelocity) {
                velocity[1] = -maxVelocity;
            }

            // Find rotation matrix to make flappybird face in direction of movement
            glm::vec3 dir = glm::normalize(velocity);
            glm::vec3 z = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 cross = glm::cross(dir,z);
            glm::mat4 flightRotation = glm::mat4(glm::vec4(dir, 0.0f), glm::vec4(z,0.0f), glm::vec4(cross, 0.0f), glm::vec4(0,0,0,1.0f));

            orientation = glm::translate(position + update) * flightRotation;
        }

    protected:
    private:
};

#endif // FLAPPYBIRD_H
