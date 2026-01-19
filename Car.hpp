#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    class Car
    {
    private:
        BasicShape bodyMain;              
        BasicShape bodyHood;          
        BasicShape bodyTrunk;               
        BasicShape bodyRoof;          
        BasicShape windowFront;    
        BasicShape windowRear;          
        BasicShape windowsLeft;           
        BasicShape windowsRight;      
        BasicShape wheels;              
        BasicShape wheelRims;         
        BasicShape headlights;           
        BasicShape taillights;            
        BasicShape indicators;        
        BasicShape seats;                
        BasicShape steeringWheel;        
        BasicShape dashboard;            
        BasicShape interiorPanels;       
        BasicShape floorMat;            
        BasicShape gearShift;          
        BasicShape mirrors;
        BasicShape doorHandles;          
        BasicShape grille;              
        BasicShape bumpers;           
        float baseY = 0.0f;
        float steer = 0.0f;           
        float turnRate = 80.0f;        

        BasicShape driverDoor;   
        glm::vec3 driverDoorHingeLocal = { 0,0,0 };      
        float driverDoorAngle = 0.0f;       
        float driverDoorTargetAngle = 0.0f;        
        float driverDoorOpenAngle = -70.0f;    
        float throttle = 0.0f;   

        float speed = 0.0f;      
        float maxForwardSpeed = 10.0f;
        float maxReverseSpeed = 4.0f;
        float acceleration = 12.0f;        
        float friction = 10.0f;       
        glm::vec3 position;
        float rotation;               
        float length = 4.5f;     
        float width = 1.9f;      
        float height = 1.4f;          
        float groundClearance = 0.2f; 
        glm::vec3 bodyColor;
        glm::vec3 interiorColor;

        bool driverDoorOpen = false;

        void buildExterior();
        void buildInterior();
        void buildWheels();
        void buildLights();
        void buildDetails();

        void buildDriverDoor();

    public:
        Car();
        ~Car() = default;
        float bodyScale = 1.0f;  
        float bodyRot = 0.0f;  

        

        BasicShape bodyShape;
        bool useBodyShape = false;
        

                void create(glm::vec3 pos, float rotationY = 0.0f,
                glm::vec3 bodyCol = { 0.7f, 0.1f, 0.1f },                  
                glm::vec3 interiorCol = { 0.15f, 0.12f, 0.1f }); 
                void render(const glm::mat4& viewProj);

                glm::vec3 getDriverSeatPosition() const;

                glm::vec3 getDriverViewDirection() const;

                bool isNearDriverDoor(glm::vec3 playerPos) const;

                void getCollisionBounds(float& minX, float& maxX, float& minZ, float& maxZ) const;

                void setSteer(float s); 
                void setPosition(const glm::vec3& p) { position = p; }
                void stopMovement() { speed = 0.0f; throttle = 0.0f; steer = 0.0f; }

        float getGroundClearance() const { return groundClearance; }

        glm::vec3 getPosition() const { return position; }
        float getRotation() const { return rotation; }


        void update(float dt);
        void setDriverDoorOpen(bool open);
        void setThrottle(float t);        
        float getSpeed() const { return speed; }
    };
}