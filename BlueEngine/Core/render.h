#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Model.h"
#include "plane.h"
#include "Sphere.h"
#include "Collision.h"
#include "Entity.h"
#include "Systems.h"
#include "Components.h"


Sphere sphere;
Collision coll;
Plane Plane;

Camera camera;

entity_manager entityManager;
component_manager componentManager;
SystemManager systemManager;

bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere);



struct Render {
bool inside;
    bool isMovingForward;

    

    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {
         
        model SphereModel0, SphereModel1, SphereModel2, SphereModel3, SphereModel4;
        
        Entity Sphere0 = entityManager.create_entity();
        if (Sphere0.ID == -1){std::cerr << "Failed to create entity." << std::endl;}
        Entity TestEntity = entityManager.create_entity();
        if (TestEntity.ID == -1){std::cerr << "Failed to create test entity." << std::endl;}

        systemManager.AddSystem<test_system>(TestEntity.ID);
        
        systemManager.AddSystem<sphere_system>(Sphere0.ID);
        systemManager.AddSystem<matrix_component>(Sphere0.ID);
        
       componentManager.add_component<test_component>(TestEntity.ID);
        
        componentManager.add_component<sphere_component>(Sphere0.ID);
        componentManager.add_component<transform_component>(Sphere0.ID);
        componentManager.add_component<matrix_component>(Sphere0.ID);

        

        auto* sphereHandler = static_cast<component_handler<sphere_component>*>(componentManager.component_map[typeid(sphere_component).name()]);
        sphereHandler->Components[sphereHandler->index[Sphere0.ID]].radius = 0.5f;
        
        model floorPlane, ZWallP, ZWallN, XWallP, XWallN;
        std::vector<model*> models = { &floorPlane, &ZWallP, &ZWallN, &XWallP, &XWallN };
        
        std::vector<model*> sphere_models;
        
        sphere_models.emplace_back(&SphereModel0);
        sphere_models.emplace_back(&SphereModel1);
        sphere_models.emplace_back(&SphereModel2);
        sphere_models.emplace_back(&SphereModel3);
        sphere_models.emplace_back(&SphereModel4);
     

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection;


    systemManager.draw_meshes<sphere_system>(componentManager);

        Plane.CreateFloor(floorPlane);
        Plane.CreateFloor(ZWallP);
        Plane.CreateFloor(ZWallN);
        Plane.CreateFloor(XWallP);
        Plane.CreateFloor(XWallN);
        
        sphere.CreateSphere(SphereModel0);
      sphere.CreateSphere(SphereModel1);
      sphere.CreateSphere(SphereModel2);
      sphere.CreateSphere(SphereModel3);
        sphere.CreateSphere(SphereModel4);

        


        floorPlane.PlayerPos = glm::vec3(0.f,0.f,0.f);
       
        ZWallN.PlayerPos= glm::vec3(0.f, 0, -4.5f);
        ZWallN.PlayerRotation = glm::vec3(90.f,0.f,0.f);
        ZWallN.PlayerScale = glm::vec3(1.f,1.f,0.1f);
        
        ZWallP.PlayerPos= glm::vec3(0.f, 0.f, 4.5f);
        ZWallP.PlayerRotation = glm::vec3(-90.f,0.f,0.f);
        ZWallP.PlayerScale = glm::vec3(1.f,1.f,0.1f);
        
        XWallN.PlayerPos= glm::vec3(-4.5f, 0.f, 0.f);
        XWallN.PlayerRotation = glm::vec3(0.f,0.f,-90.f);
        XWallN.PlayerScale = glm::vec3(0.1f,1.f,1.f);
        
        XWallP.PlayerPos= glm::vec3(4.5f, 0.f, 0.f);
        XWallP.PlayerRotation = glm::vec3(0.f,0.f,90.f);
        XWallP.PlayerScale = glm::vec3(0.1f,1.f,1.f);

        SphereModel0.PlayerPos = glm::vec3(-4.f,0.1f,0.3f);
        SphereModel1.PlayerPos = glm::vec3(-3.f,0.1f,0.2f);
        SphereModel2.PlayerPos = glm::vec3(0.f,0.1f,-1.f);
        SphereModel3.PlayerPos = glm::vec3(3.f,0.1f,0.f);
        SphereModel4.PlayerPos = glm::vec3(1.f,0.1f,-1.f);
        
        while (!glfwWindowShouldClose(window))
            {
          

            coll.SphereSphereCollision(sphere_models);
           coll.SphereBoxCollision(sphere_models,models);

            
           sphere.Move(SphereModel0, deltaTime, SphereModel0.Velocity);
           sphere.Move(SphereModel1, deltaTime, SphereModel1.Velocity);
           sphere.Move(SphereModel2, deltaTime, SphereModel2.Velocity);
           sphere.Move(SphereModel3, deltaTime, SphereModel3.Velocity);
            sphere.Move(SphereModel4, deltaTime, SphereModel4.Velocity);
            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ProsessInput(window, deltaTime, SphereModel0);
            

            systemManager.UpdateSystems<sphere_system>(shaderProgram, componentManager);
            systemManager.UpdateSystems<matrix_system>(shaderProgram, componentManager);
            systemManager.UpdateSystems<test_system>(shaderProgram, componentManager);
            
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            camera.tick(shaderProgram);

       
            glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
            glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

            int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,10,0)));

            glLineWidth(3);

          
            for (model* element : sphere_models)
            {
                element->CalculateMatrix();
                element->CalculateBoundingBox();
                element->DrawMesh(shaderProgram);

            }
            for (model* element : models)
            {
                element->CalculateMatrix();
                element->CalculateBoundingBox();
                element->DrawMesh(shaderProgram);

            }
            
            glfwSwapBuffers(window);
            glfwPollEvents();
            }
    }
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw   += xoffset;
    camera.pitch += yoffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.cameraFront = glm::normalize(direction);
}


void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere) {

    glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(camera.cameraFront.x, 0.0f, camera.cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime ;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.cameraPos += cameraSpeed * cameraFrontXZ;
        /*Player.PlayerPos +=  cameraSpeed * cameraFrontXZ;*/}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       { camera.cameraPos -= cameraSpeed * cameraFrontXZ;
        /*Player.PlayerPos -=  cameraSpeed * cameraFrontXZ;*/}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    /*Player.PlayerPos -=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    /*Player.PlayerPos +=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.cameraPos += cameraSpeed * camera.cameraUp; // Move camera up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.cameraPos -= cameraSpeed * camera.cameraUp;

    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        std::cout<<"move forward"<<std::endl;
        sphere.Velocity = glm::vec3(8.f,0.f,0.f);
}

}
