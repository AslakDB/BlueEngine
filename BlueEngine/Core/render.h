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
render_system renderSystem;

bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM);



struct Render {
bool inside;
    bool isMovingForward;

    

    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {
        
        renderSystem.CreateMeshes();

        std::vector<glm::vec3> EnemyPos;
        std::vector<Entity> Enemies;

        model SphereModel0, SphereModel1, SphereModel2, SphereModel3, SphereModel4;
        
        Entity Player = entityManager.create_entity();
        if (Player.ID == -1){std::cerr << "Failed to create entity." << std::endl;}

        
        Entity Enemy0 = entityManager.create_entity();
        Entity Enemy1 = entityManager.create_entity();
        Entity Enemy2 = entityManager.create_entity();
        Entity Enemy3 = entityManager.create_entity();
        Entity Enemy4 = entityManager.create_entity();
        Entity Enemy5 = entityManager.create_entity();
        
        
        Entity TestEntity = entityManager.create_entity();
        if (TestEntity.ID == -1){std::cerr << "Failed to create test entity." << std::endl;}

        Entity Plane0 = entityManager.create_entity();
        if (Plane0.ID == -1){std::cerr << "Failed to create plane entity." << std::endl;}

       
        systemManager.AddSystem<matrix_system>();
        //systemManager.AddSystem<plane_system>(Plane0.ID);
        systemManager.AddSystem<render_system>();
        systemManager.AddSystem<movementSystem>();

       // systemManager.AddSystem<test_system>(TestEntity.ID);
       
       // systemManager.AddSystem<sphere_system>(Sphere0.ID);
        //systemManager.AddSystem<matrix_system>(Sphere0.ID);
        //systemManager.AddSystem<render_system>(Sphere0.ID);
        /*systemManager.AddSystem<sphere_system>(Sphere1.ID);
        systemManager.AddSystem<matrix_system>(Sphere1.ID);*/
        
        // Add components
        componentManager.add_component<model_component>(Player.ID);
        componentManager.add_component<transform_component>(Player.ID);
        componentManager.add_component<movement_component>(Player.ID);
        componentManager.add_component<mesh_component>(Player.ID);
        componentManager.add_component<sphere_component>(Player.ID);
        componentManager.add_component<matrix_component>(Player.ID);
        componentManager.getComponent<model_component>(Player.ID).MeshName = "Sphere";
        componentManager.getComponent<model_component>(Player.ID).colour = glm::vec3(1,0,0);
       // componentManager.getComponent<movement_component>(Sphere0.ID).Velocity = glm::vec3(2, 0, 0) ;
        
        componentManager.add_component<plane_component>(Plane0.ID);
       // componentManager.add_component<mesh_component>(Plane0.ID);
        componentManager.add_component<transform_component>(Plane0.ID);
        componentManager.add_component<matrix_component>(Plane0.ID);
        componentManager.add_component<model_component>(Plane0.ID);
        componentManager.getComponent<transform_component>(Plane0.ID).Scale = glm::vec3(1, 1, 10);
        componentManager.getComponent<transform_component>(Plane0.ID).PlayerPos = glm::vec3(1, 0, -40);
        
        componentManager.add_component<test_component>(TestEntity.ID);
        
        
        

        componentManager.getComponent<transform_component>(Player.ID).PlayerPos = glm::vec3(0,0 , 2);
        componentManager.getComponent<transform_component>(Player.ID).Scale =  glm::vec3(0.5, 0.5,0.5);
        
        componentManager.add_component<model_component>(Enemy0.ID);
        componentManager.add_component<sphere_component>(Enemy0.ID);
        componentManager.add_component<transform_component>(Enemy0.ID);
        componentManager.add_component<matrix_component>(Enemy0.ID);
        componentManager.getComponent<model_component>(Enemy0.ID).MeshName = "Sphere";
        
        componentManager.getComponent<transform_component>(Enemy0.ID).Scale =  glm::vec3(0.5, 0.5,0.5);
     

        EnemyPos.emplace_back(componentManager.getComponent<transform_component>(Enemy0.ID).PlayerPos);


        // Setup sphere model radius
        auto* sphereHandler = static_cast<component_handler<sphere_component>*>(componentManager.component_map[typeid(sphere_component).name()]);
        sphereHandler->Components[sphereHandler->index[Player.ID]].radius = 0.5f;
        
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

        
       /*systemManager.draw_meshes<plane_system>(componentManager);
        systemManager.draw_meshes<sphere_system>(componentManager);*/
        
        /*Plane.CreateFloor(floorPlane);
        Plane.CreateFloor(ZWallP);
        Plane.CreateFloor(ZWallN);
        Plane.CreateFloor(XWallP);
        Plane.CreateFloor(XWallN);*/
        
        /*sphere.CreateSphere(SphereModel0);
      sphere.CreateSphere(SphereModel1);
      sphere.CreateSphere(SphereModel2);
      sphere.CreateSphere(SphereModel3);
        sphere.CreateSphere(SphereModel4);*/
       //  componentManager.getComponent<movement_component>(Player.ID).Velocity = glm::vec3(1, 0, 0);
        while (!glfwWindowShouldClose(window))
            {

           
            
            if(coll.SphereSphereCollision(componentManager.getComponent<transform_component>(Player.ID).PlayerPos ,EnemyPos) == true)
            {
                componentManager.getComponent<movement_component>(Player.ID).Velocity *= glm::vec3(-1);
                
            }
                        
            
                
                //coll.SphereBoxCollision(sphere_models,models);

            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ProsessInput(window, deltaTime, componentManager.getComponent<movement_component>(Player.ID));

            
           

            
            
            //systemManager.UpdateSystems<model_system>(shaderProgram,componentManager);
            
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            camera.tick(shaderProgram);

       
            glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
            glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

            int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,10,0)));

            glLineWidth(3);
            


            //systemManager.UpdateSystems<render_system>(shaderProgram, componentManager);
            renderSystem.Update(shaderProgram, componentManager, deltaTime);
            systemManager.UpdateSystems<matrix_system>(shaderProgram, componentManager, deltaTime);
            
            systemManager.UpdateSystems<movementSystem>(shaderProgram,componentManager, deltaTime);
            
            
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


void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM)
{
    glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(camera.cameraFront.x, 0.0f, camera.cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime ;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.cameraPos += cameraSpeed * cameraFrontXZ;
    }
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
            //sphere.Velocity = glm::vec3(8.f,0.f,0.f);
        }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        MVM.Velocity -= glm::vec3(0, 0, 2) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        MVM.Velocity += glm::vec3(0, 0, 2) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        MVM.Velocity -= glm::vec3(2, 0, 0) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        MVM.Velocity += glm::vec3(2, 0,0 ) * deltaTime;
    } else {
        MVM.Velocity = glm::vec3(0, 0, 0);
    }
}

