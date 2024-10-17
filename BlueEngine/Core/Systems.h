#pragma once
#include <set>
#include <unordered_map>

#include "Components.h"

 inline void SubDivide(int A, int B, int C, int NumOfDiv, mesh_component& SphereModel)
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


inline void CreateSphere(mesh_component& TempMesh)
{
    TempMesh.vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f), glm::vec3(0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f), glm::vec3(0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f), glm::vec3(0.6f));

    // Subdivide the sphere
    int NumOfDiv = 4;
    SubDivide(0, 1, 2, NumOfDiv, TempMesh);
    SubDivide(0, 2, 3, NumOfDiv, TempMesh);
    SubDivide(0, 3, 4, NumOfDiv, TempMesh);
    SubDivide(0, 4, 1, NumOfDiv, TempMesh);
    SubDivide(5, 2, 1, NumOfDiv, TempMesh);
    SubDivide(5, 3, 2, NumOfDiv, TempMesh);
    SubDivide(5, 4, 3, NumOfDiv, TempMesh);
    SubDivide(5, 1, 4, NumOfDiv, TempMesh);

    // Calculate normals
    for (Triangle& index : TempMesh.indices) {
        glm::vec3 normal = glm::cross(
            TempMesh.vertices[index.B].XYZ - TempMesh.vertices[index.A].XYZ,TempMesh.vertices[index.C].XYZ - TempMesh.vertices[index.A].XYZ);
        TempMesh.vertices[index.A].Normals += glm::normalize(normal);
        TempMesh.vertices[index.B].Normals += glm::normalize(normal);
        TempMesh.vertices[index.C].Normals += glm::normalize(normal);
    }

     TempMesh.bind();
}

 inline void CreatePlane(mesh_component& TempMesh)
{
    /*element.vertices.clear();
    element.indices.clear();*/
        
    TempMesh.vertices.emplace_back(glm::vec3(5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(-5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    TempMesh.vertices.emplace_back(glm::vec3(-5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));

        
    TempMesh.indices.emplace_back(0,1,3);
    TempMesh.indices.emplace_back(1,2,3);
        
    for (Triangle& index :  TempMesh.indices)
    {
        glm::vec3 normal = glm::cross(TempMesh.vertices[index.B].XYZ - TempMesh.vertices[index.A].XYZ, TempMesh.vertices[index.C].XYZ - TempMesh.vertices[index.A].XYZ);

        TempMesh.vertices[index.A].Normals += glm::normalize(normal);
        TempMesh.vertices[index.B].Normals += glm::normalize(normal);
        TempMesh.vertices[index.C].Normals += glm::normalize(normal);
    }
     TempMesh.bind();
}

class Systems
{
public:
    virtual ~Systems() = default;
    virtual void Draw( component_manager& componentManager ) {};
   virtual void Update(unsigned int ShaderProgram, component_manager& componentManager) = 0;
   std::pmr::set<Entity> mEntities;


};


struct I_system_handler
{
    virtual ~I_system_handler() = default;
};

template <typename T> 
struct system_handler : public I_system_handler
{
    std::vector<T> Systems;
    std::map<int,int> index;  
};

class SystemManager : public I_system_handler
{
public:
    ~SystemManager()
    {
        for (auto& [key, handler] : m_systems_map)
        {
            delete handler;
        }
    }
    std::map<std::string, I_system_handler*> m_systems_map;
    template <typename T>
    void AddSystem(unsigned int id)
    {
        if (m_systems_map.contains(typeid(T).name()) == false)
        {
            m_systems_map[typeid(T).name()] = new system_handler<T>();
        }
        system_handler<T> *handler = static_cast<system_handler<T>*>(m_systems_map[typeid(T).name()]);
        handler->Systems.emplace_back();
        handler->index[id] = handler->Systems.size() - 1;
    }

    template <typename T>
    void draw_meshes(component_manager& componentManager)
    {
        std::string system_name = typeid(T).name();
        if (m_systems_map.find(system_name) == m_systems_map.end())
        {
            return;
        }
        system_handler<T>* handler = static_cast<system_handler<T>*>(m_systems_map[system_name]);
        for (T& system : handler->Systems)
        {
            system.Draw(componentManager); //unsure what to have here
        }
    }

    
    template <typename T>
    void UpdateSystems(unsigned int ShaderSystem, component_manager& componentManager)
    {
        std::string system_name = typeid(T).name();
        if (m_systems_map.find(system_name) == m_systems_map.end())
        {
            return;
        }
        system_handler<T>* handler = static_cast<system_handler<T>*>(m_systems_map[system_name]);
        for (T& system : handler->Systems)
        {
            system.Update(ShaderSystem,  componentManager); //unsure what to have here
        }
    }
};

struct movementSystem : public Systems
{
    /*position_component *position;*/
    transform_component *transform;
    movement_component *movement;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        transform->PlayerPos += movement->Velocity * movement->speed;
    }

};

struct HealthSystem : public Systems
{
    health_component *health;
    attack_component *attack;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {  
        component_handler<health_component> *health = componentManager.get_component_handler<health_component>();
        component_handler<attack_component> *attack = componentManager.get_component_handler<attack_component>();
        /*for (health_component &element : health->Components)
        {
            for (attack_component &element2 : attack->Components)
            {
                if (element2.Attack == true)
                {
                    element.Health -= 10;
                    element2.Attack = false;
                }
            }
        }*/
        
    }
};

struct matrix_system : public Systems
{
   // glm::mat4 _modelMatrix = glm::mat4(1.f);
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override {
        auto* matrix_handler = componentManager.get_component_handler<matrix_component>();
        auto* transform_handler = componentManager.get_component_handler<transform_component>();

        // Ensure both handlers are valid
        if (!matrix_handler || !transform_handler) {
            return;
        }

        // Assuming a one-to-one correspondence between matrix and transform components
        auto& matrices = matrix_handler->Components;
        auto& transforms = transform_handler->Components;

        size_t min_size = std::min(matrices.size(), transforms.size());

        for (size_t i = 0; i < min_size; ++i) {
            auto& matrix = matrices[i];
            auto& transform = transforms[i];

            matrix.ModelMatrix = glm::mat4(1.f);
            matrix.ModelMatrix = glm::translate(matrix.ModelMatrix, transform.PlayerPos);
            matrix.ModelMatrix = glm::rotate(matrix.ModelMatrix, transform.Rotation.x, glm::vec3(1.f, 0.f, 0.f));
            matrix.ModelMatrix = glm::rotate(matrix.ModelMatrix, transform.Rotation.y, glm::vec3(0.f, 1.f, 0.f));
            matrix.ModelMatrix = glm::rotate(matrix.ModelMatrix, transform.Rotation.z, glm::vec3(0.f, 0.f, 1.f));
            matrix.ModelMatrix = glm::scale(matrix.ModelMatrix, transform.Scale);
        }
    }
    
};


struct render_system : Systems
{
    std::unordered_map<std::string, mesh_component> MeshMap;
    void CreateMeshes()
    {
        MeshMap["Sphere"];
        CreateSphere(MeshMap["Sphere"]);
        MeshMap["Plane"];
        CreatePlane(MeshMap["Plane"]);
    }
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        component_handler<model_component> *model_handler = componentManager.get_component_handler<model_component>();
        component_handler<matrix_component> *matrix_handler = componentManager.get_component_handler<matrix_component>();

        if (!model_handler || !matrix_handler) {
            return;
        }

        auto& models = model_handler->Components;
        auto& matrices = matrix_handler->Components;

        glUseProgram(ShaderProgram);

        // Ensure the sizes match or handle mismatches appropriately
        //size_t min_size = std::min(models.size(), matrices.size());

        for (size_t i = 0; i < models.size(); ++i) {
            const auto& mesh = MeshMap[models[i].MeshName];
            const auto& matrix = matrices[i];

            int modelLoc = glGetUniformLocation(ShaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix.ModelMatrix));
            glBindVertexArray(mesh.VAO);

            glDrawElements(GL_TRIANGLES, mesh.indices.size() * 3, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        }
    }
};
  
    


struct plane_system : public Systems
{
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
    }
    

   
void Draw ( component_manager& componentManager) override
{

        std::cout<<"Drawing Plane"<<std::endl;
        
        mesh_component TempMesh;
        /*element.vertices.clear();
        element.indices.clear();*/
        
        TempMesh.vertices.emplace_back(glm::vec3(5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
        TempMesh.vertices.emplace_back(glm::vec3(5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
        TempMesh.vertices.emplace_back(glm::vec3(-5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
        TempMesh.vertices.emplace_back(glm::vec3(-5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));

        
        TempMesh.indices.emplace_back(0,1,3);
        TempMesh.indices.emplace_back(1,2,3);
        
    for (Triangle& index :  TempMesh.indices)
    {
        glm::vec3 normal = glm::cross(TempMesh.vertices[index.B].XYZ - TempMesh.vertices[index.A].XYZ, TempMesh.vertices[index.C].XYZ - TempMesh.vertices[index.A].XYZ);

        TempMesh.vertices[index.A].Normals += glm::normalize(normal);
        TempMesh.vertices[index.B].Normals += glm::normalize(normal);
        TempMesh.vertices[index.C].Normals += glm::normalize(normal);
    }

        
        glGenVertexArrays(1, &TempMesh.VAO);
        glBindVertexArray(TempMesh.VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &TempMesh.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TempMesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, TempMesh.vertices.size() * sizeof(Vertex), TempMesh.vertices.data(), GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        glGenBuffers(1, &TempMesh.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TempMesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, TempMesh.indices.size() * sizeof(Triangle), TempMesh.indices.data(), GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Unbind the VBO (optional)
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbind the VAO (optional)
        glBindVertexArray(0);
    
}
};


struct cube_system : public Systems
{
    cube_component *cube;
    
};

struct test_system final : public Systems
{
   
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        component_handler<test_component> *test = componentManager.get_component_handler<test_component>();
        for (test_component &element : test->Components)
        {
            element.TestString = "Hello World";
            std::cout<<element.TestString<<std::endl;
            
        }
    }
    void TestVoid()
    {
        std::cout<<"Hello void"<<std::endl;
    }
    
};


