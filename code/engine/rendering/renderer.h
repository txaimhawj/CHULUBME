#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../core/ecs.h"

namespace CHULUBME {

// Forward declarations
class Shader;
class Texture;
class Mesh;
class Material;
class Camera;

/**
 * @brief Transform component for positioning entities in 3D space
 */
class TransformComponent : public Component {
private:
    // Position, rotation, and scale
    float m_position[3];
    float m_rotation[3];
    float m_scale[3];
    
    // Parent transform
    Entity m_parent;
    
    // Child transforms
    std::vector<Entity> m_children;
    
    // World transformation matrix
    float m_worldMatrix[16];
    
    // Whether the world matrix needs to be recalculated
    bool m_dirty;

public:
    TransformComponent();
    ~TransformComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set position
    void SetPosition(float x, float y, float z);
    
    // Get position
    const float* GetPosition() const { return m_position; }
    
    // Set rotation (Euler angles in degrees)
    void SetRotation(float x, float y, float z);
    
    // Get rotation
    const float* GetRotation() const { return m_rotation; }
    
    // Set scale
    void SetScale(float x, float y, float z);
    
    // Get scale
    const float* GetScale() const { return m_scale; }
    
    // Set parent transform
    void SetParent(Entity parent);
    
    // Get parent transform
    Entity GetParent() const { return m_parent; }
    
    // Add child transform
    void AddChild(Entity child);
    
    // Remove child transform
    void RemoveChild(Entity child);
    
    // Get child transforms
    const std::vector<Entity>& GetChildren() const { return m_children; }
    
    // Get world transformation matrix
    const float* GetWorldMatrix();
    
    // Mark the transform as dirty (needs recalculation)
    void SetDirty() { m_dirty = true; }
    
    // Check if the transform is dirty
    bool IsDirty() const { return m_dirty; }
};

/**
 * @brief Camera component for rendering the scene
 */
class CameraComponent : public Component {
private:
    // Camera properties
    float m_fieldOfView;
    float m_nearPlane;
    float m_farPlane;
    float m_aspectRatio;
    
    // View and projection matrices
    float m_viewMatrix[16];
    float m_projectionMatrix[16];
    
    // Whether the matrices need to be recalculated
    bool m_dirty;
    
    // Whether this is the main camera
    bool m_main;

public:
    CameraComponent();
    ~CameraComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set field of view (in degrees)
    void SetFieldOfView(float fov);
    
    // Get field of view
    float GetFieldOfView() const { return m_fieldOfView; }
    
    // Set near plane distance
    void SetNearPlane(float nearPlane);
    
    // Get near plane distance
    float GetNearPlane() const { return m_nearPlane; }
    
    // Set far plane distance
    void SetFarPlane(float farPlane);
    
    // Get far plane distance
    float GetFarPlane() const { return m_farPlane; }
    
    // Set aspect ratio
    void SetAspectRatio(float aspectRatio);
    
    // Get aspect ratio
    float GetAspectRatio() const { return m_aspectRatio; }
    
    // Get view matrix
    const float* GetViewMatrix(const TransformComponent* transform);
    
    // Get projection matrix
    const float* GetProjectionMatrix();
    
    // Set as main camera
    void SetMain(bool main) { m_main = main; }
    
    // Check if this is the main camera
    bool IsMain() const { return m_main; }
    
    // Mark the camera as dirty (needs recalculation)
    void SetDirty() { m_dirty = true; }
    
    // Check if the camera is dirty
    bool IsDirty() const { return m_dirty; }
};

/**
 * @brief Mesh renderer component for rendering 3D meshes
 */
class MeshRendererComponent : public Component {
private:
    // Mesh to render
    std::shared_ptr<Mesh> m_mesh;
    
    // Material to use for rendering
    std::shared_ptr<Material> m_material;
    
    // Whether the mesh renderer is visible
    bool m_visible;

public:
    MeshRendererComponent();
    ~MeshRendererComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set mesh
    void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
    
    // Get mesh
    std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
    
    // Set material
    void SetMaterial(std::shared_ptr<Material> material) { m_material = material; }
    
    // Get material
    std::shared_ptr<Material> GetMaterial() const { return m_material; }
    
    // Set visibility
    void SetVisible(bool visible) { m_visible = visible; }
    
    // Check if the mesh renderer is visible
    bool IsVisible() const { return m_visible; }
};

/**
 * @brief Render system for rendering entities with mesh renderer components
 */
class RenderSystem : public System {
private:
    // Main camera
    Entity m_mainCamera;
    
    // Render queue
    struct RenderQueueItem {
        Entity entity;
        MeshRendererComponent* meshRenderer;
        TransformComponent* transform;
    };
    
    std::vector<RenderQueueItem> m_renderQueue;
    
    // Shaders
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    
    // Textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    
    // Meshes
    std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
    
    // Materials
    std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;

public:
    RenderSystem(EntityManager* manager);
    ~RenderSystem() override;
    
    // Initialize the system
    void Initialize() override;
    
    // Update the system
    void Update(float deltaTime) override;
    
    // Render the system
    void Render() override;
    
    // Called when an entity is added to this system
    void OnEntityAdded(Entity entity) override;
    
    // Called when an entity is removed from this system
    void OnEntityRemoved(Entity entity) override;
    
    // Set main camera
    void SetMainCamera(Entity camera) { m_mainCamera = camera; }
    
    // Get main camera
    Entity GetMainCamera() const { return m_mainCamera; }
    
    // Load a shader
    std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    
    // Get a shader
    std::shared_ptr<Shader> GetShader(const std::string& name) const;
    
    // Load a texture
    std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
    
    // Get a texture
    std::shared_ptr<Texture> GetTexture(const std::string& name) const;
    
    // Load a mesh
    std::shared_ptr<Mesh> LoadMesh(const std::string& name, const std::string& path);
    
    // Get a mesh
    std::shared_ptr<Mesh> GetMesh(const std::string& name) const;
    
    // Create a material
    std::shared_ptr<Material> CreateMaterial(const std::string& name, std::shared_ptr<Shader> shader);
    
    // Get a material
    std::shared_ptr<Material> GetMaterial(const std::string& name) const;
};

} // namespace CHULUBME

