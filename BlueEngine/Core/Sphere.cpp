#include "Sphere.h"

void Sphere::SubDivide(int A, int B, int C, int NumOfDiv, model& SphereModel)
{
        if(NumOfDiv > 0)
        {
            glm::vec3 v1 =glm::normalize(SphereModel.vertices[A].XYZ + SphereModel.vertices[B].XYZ);
            glm::vec3 v2 =glm::normalize(SphereModel.vertices[A].XYZ+SphereModel.vertices[C].XYZ);
            glm::vec3 v3 =glm::normalize(SphereModel.vertices[C].XYZ+SphereModel.vertices[B].XYZ );
        

            int index1 =SphereModel.vertices.size(); 
            SphereModel.vertices.emplace_back(v1,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
            int index2 = SphereModel.vertices.size();
            SphereModel.vertices.emplace_back(v2,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
            int index3 = SphereModel.vertices.size();
            SphereModel.vertices.emplace_back(v3,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
        
            SubDivide(A,index1,index2, NumOfDiv -1, SphereModel);
            SubDivide(C,index2,index3, NumOfDiv -1,SphereModel);
            SubDivide(B,index3,index1, NumOfDiv -1,SphereModel);
            SubDivide(index3,index2,index1, NumOfDiv -1,SphereModel);
        }
        else
        {
            SphereModel.indices.emplace_back(A,B,C);
        }
    
}

void Sphere::CreateSphere(model& SphereModel)
{
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,1.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(-1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,-1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,-1.f), glm::vec3(0.f), glm::vec3(0.6f));

    SubDivide(0,1,2,4, SphereModel);
    SubDivide(0,2,3,4, SphereModel);
    SubDivide(0,3,4,4, SphereModel);
    SubDivide(0,4,1,4, SphereModel);
    SubDivide(5,2,1,4, SphereModel);
    SubDivide(5,3,2,4, SphereModel);
    SubDivide(5,4,3,4, SphereModel);
    SubDivide(5,1,4,4, SphereModel);


    for (Triangle& index : SphereModel.indices)
    {
        glm::vec3 normal = glm::cross(SphereModel.vertices[index.B].XYZ - SphereModel.vertices[index.A].XYZ, SphereModel.vertices[index.C].XYZ - SphereModel.vertices[index.A].XYZ);

        SphereModel.vertices[index.A].Normals += glm::normalize(normal);
        SphereModel.vertices[index.B].Normals += glm::normalize(normal);
        SphereModel.vertices[index.C].Normals += glm::normalize(normal);
    }
    
    SphereModel.Bind();
    

    //Not the best way to scale, but having problems doing this during creation
     
    SphereModel.PlayerScale = glm::vec3(0.5f);
}

void Sphere::Move(model& SphereModel, float deltatime, glm::vec3 RandSpeed)
{
    SphereModel.PlayerPos = SphereModel.PlayerPos + (RandSpeed * deltatime);
}

void sphere_system::Update(unsigned int shaderProgram, component_manager& componentManager)
{
    component_handler<model_component>* modelHandler = componentManager.get_component_handler<model_component>();
    component_handler<matrix_component>* matrixHandler = componentManager.get_component_handler<matrix_component>();
    if (!modelHandler || !matrixHandler) {
        return;
    }

    auto& models = modelHandler->Components;
    auto& matrices = matrixHandler->Components;
    glUseProgram(shaderProgram);

    for (size_t i = 0; i < models.size(); ++i) {
        auto& model = models[i];
        auto& matrix = matrices[i];

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix.ModelMatrix));
        glBindVertexArray(model.VAO);

        glDrawElements(GL_TRIANGLES, model.indices.size() * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void sphere_system::SubDivide(int A, int B, int C, int NumOfDiv, model& SphereModel)
{
    
    if(NumOfDiv > 0)
    {
        glm::vec3 v1 =glm::normalize(SphereModel.vertices[A].XYZ + SphereModel.vertices[B].XYZ);
        glm::vec3 v2 =glm::normalize(SphereModel.vertices[A].XYZ+SphereModel.vertices[C].XYZ);
        glm::vec3 v3 =glm::normalize(SphereModel.vertices[C].XYZ+SphereModel.vertices[B].XYZ );
        

        int index1 =SphereModel.vertices.size(); 
        SphereModel.vertices.emplace_back(v1,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
        int index2 = SphereModel.vertices.size();
        SphereModel.vertices.emplace_back(v2,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
        int index3 = SphereModel.vertices.size();
        SphereModel.vertices.emplace_back(v3,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
        
        SubDivide(A,index1,index2, NumOfDiv -1, SphereModel);
        SubDivide(C,index2,index3, NumOfDiv -1,SphereModel);
        SubDivide(B,index3,index1, NumOfDiv -1,SphereModel);
        SubDivide(index3,index2,index1, NumOfDiv -1,SphereModel);
    }
    else
    {
        SphereModel.indices.emplace_back(A,B,C);
    }
}

void sphere_system:: Draw(component_manager componentManager) 
{

    component_handler<sphere_component> *sphere = componentManager.get_component_handler<sphere_component>();
       component_handler<model_component>* modelHandler = componentManager.get_component_handler<model_component>();


    for (sphere_component element : sphere->Components)
    {
    element.sphere_model.vertices.emplace_back(glm::vec3(0.f,0.f,1.f), glm::vec3(0.f), glm::vec3(0.6f));
    element.sphere_model.vertices.emplace_back(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    element.sphere_model.vertices.emplace_back(glm::vec3(0.f,1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    element.sphere_model.vertices.emplace_back(glm::vec3(-1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    element.sphere_model.vertices.emplace_back(glm::vec3(0.f,-1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    element.sphere_model.vertices.emplace_back(glm::vec3(0.f,0.f,-1.f), glm::vec3(0.f), glm::vec3(0.6f));

    SubDivide(0,1,2,4, element.sphere_model);
    SubDivide(0,2,3,4, element.sphere_model);
    SubDivide(0,3,4,4, element.sphere_model);
    SubDivide(0,4,1,4, element.sphere_model);
    SubDivide(5,2,1,4, element.sphere_model);
    SubDivide(5,3,2,4, element.sphere_model);
    SubDivide(5,4,3,4, element.sphere_model);
    SubDivide(5,1,4,4, element.sphere_model);
    for (Triangle& index : element.sphere_model.indices)
    {
        glm::vec3 normal = glm::cross(element.sphere_model.vertices[index.B].XYZ - element.sphere_model.vertices[index.A].XYZ, element.sphere_model.vertices[index.C].XYZ - element.sphere_model.vertices[index.A].XYZ);

       element.sphere_model.vertices[index.A].Normals += glm::normalize(normal);
       element.sphere_model.vertices[index.B].Normals += glm::normalize(normal);
       element.sphere_model.vertices[index.C].Normals += glm::normalize(normal);}
       
    element.sphere_model.Bind();
    element.sphere_model.PlayerScale = glm::vec3(0.5f);
}
}
