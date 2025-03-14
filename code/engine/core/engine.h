#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "ecs.h"

namespace CHULUBME {

/**
 * @brief Main engine class that manages the game loop and subsystems
 */
class Engine {
private:
    // Singleton instance
    static std::unique_ptr<Engine> s_instance;
    
    // Entity manager
    std::unique_ptr<EntityManager> m_entityManager;
    
    // Game loop timing
    std::chrono::steady_clock::time_point m_lastFrameTime;
    float m_deltaTime;
    float m_fixedTimeStep;
    float m_timeAccumulator;
    
    // Engine state
    bool m_initialized;
    bool m_running;
    
    // Private constructor for singleton
    Engine();

public:
    // Get singleton instance
    static Engine& Instance();
    
    // Destroy singleton instance
    static void DestroyInstance();
    
    // Initialize the engine
    bool Initialize();
    
    // Shutdown the engine
    void Shutdown();
    
    // Run the main game loop
    void Run();
    
    // Stop the main game loop
    void Stop();
    
    // Update the engine for one frame
    void Update();
    
    // Fixed update at a consistent time step
    void FixedUpdate();
    
    // Render the current frame
    void Render();
    
    // Get the entity manager
    EntityManager* GetEntityManager() const { return m_entityManager.get(); }
    
    // Get the delta time between frames
    float GetDeltaTime() const { return m_deltaTime; }
    
    // Set the fixed time step for physics and other systems
    void SetFixedTimeStep(float timeStep) { m_fixedTimeStep = timeStep; }
    
    // Get the fixed time step
    float GetFixedTimeStep() const { return m_fixedTimeStep; }
    
    // Check if engine is initialized
    bool IsInitialized() const { return m_initialized; }
    
    // Check if engine is running
    bool IsRunning() const { return m_running; }
};

} // namespace CHULUBME

