#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "hero_system.h"
#include "ability_types.h"

namespace CHULUBME {

/**
 * @brief Hero editor for creating and editing heroes
 */
class HeroEditor {
private:
    // Entity manager
    EntityManager* m_entityManager;
    
    // Hero system
    HeroSystem* m_heroSystem;
    
    // Ability system
    AbilitySystem* m_abilitySystem;
    
    // Current hero being edited
    Entity m_currentHero;
    
    // Current ability being edited
    Entity m_currentAbility;
    
    // Temporary hero data for editing
    std::string m_heroName;
    std::string m_heroDescription;
    std::string m_heroRole;
    HeroStats m_heroStats;
    
    // Temporary ability data for editing
    AbilityData m_abilityData;
    
    // Callback for when a hero is created or updated
    std::function<void(Entity)> m_heroCallback;
    
    // Callback for when an ability is created or updated
    std::function<void(Entity)> m_abilityCallback;

public:
    HeroEditor(EntityManager* entityManager);
    ~HeroEditor();
    
    // Initialize the editor
    bool Initialize();
    
    // Shutdown the editor
    void Shutdown();
    
    // Set hero callback
    void SetHeroCallback(std::function<void(Entity)> callback) { m_heroCallback = callback; }
    
    // Set ability callback
    void SetAbilityCallback(std::function<void(Entity)> callback) { m_abilityCallback = callback; }
    
    // Create a new hero
    Entity CreateNewHero();
    
    // Load an existing hero for editing
    bool LoadHero(Entity hero);
    
    // Save the current hero
    Entity SaveHero();
    
    // Create a new ability
    Entity CreateNewAbility();
    
    // Load an existing ability for editing
    bool LoadAbility(Entity ability);
    
    // Save the current ability
    Entity SaveAbility();
    
    // Add the current ability to the current hero
    bool AddAbilityToHero();
    
    // Remove an ability from the current hero
    bool RemoveAbilityFromHero(Entity ability);
    
    // Get the current hero
    Entity GetCurrentHero() const { return m_currentHero; }
    
    // Get the current ability
    Entity GetCurrentAbility() const { return m_currentAbility; }
    
    // Get hero name
    const std::string& GetHeroName() const { return m_heroName; }
    
    // Set hero name
    void SetHeroName(const std::string& name) { m_heroName = name; }
    
    // Get hero description
    const std::string& GetHeroDescription() const { return m_heroDescription; }
    
    // Set hero description
    void SetHeroDescription(const std::string& description) { m_heroDescription = description; }
    
    // Get hero role
    const std::string& GetHeroRole() const { return m_heroRole; }
    
    // Set hero role
    void SetHeroRole(const std::string& role) { m_heroRole = role; }
    
    // Get hero stats
    const HeroStats& GetHeroStats() const { return m_heroStats; }
    
    // Set hero stats
    void SetHeroStats(const HeroStats& stats) { m_heroStats = stats; }
    
    // Get ability data
    const AbilityData& GetAbilityData() const { return m_abilityData; }
    
    // Set ability data
    void SetAbilityData(const AbilityData& data) { m_abilityData = data; }
    
    // Load heroes from a JSON file
    bool LoadHeroesFromFile(const std::string& filename);
    
    // Save heroes to a JSON file
    bool SaveHeroesToFile(const std::string& filename) const;
    
    // Load abilities from a JSON file
    bool LoadAbilitiesFromFile(const std::string& filename);
    
    // Save abilities to a JSON file
    bool SaveAbilitiesToFile(const std::string& filename) const;
    
    // Render the editor UI
    void RenderUI();
};

} // namespace CHULUBME

