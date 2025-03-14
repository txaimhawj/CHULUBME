#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../core/ecs.h"
#include "ability_types.h"

namespace CHULUBME {

// Forward declarations
class AbilityComponent;

/**
 * @brief Stats for a hero
 */
struct HeroStats {
    // Base stats
    float health;
    float mana;
    float attackDamage;
    float abilityPower;
    float armor;
    float magicResist;
    float attackSpeed;
    float movementSpeed;
    float healthRegen;
    float manaRegen;
    float critChance;
    float critDamage;
    float lifeSteal;
    float cooldownReduction;
    
    // Per-level stat growth
    float healthPerLevel;
    float manaPerLevel;
    float attackDamagePerLevel;
    float abilityPowerPerLevel;
    float armorPerLevel;
    float magicResistPerLevel;
    float attackSpeedPerLevel;
    
    // Constructor with default values
    HeroStats();
};

/**
 * @brief Hero component for MOBA heroes
 */
class HeroComponent : public Component {
private:
    // Hero data
    std::string m_name;
    std::string m_description;
    std::string m_role; // "Tank", "Fighter", "Mage", "Assassin", "Support", "Marksman"
    
    // Hero stats
    HeroStats m_baseStats;
    HeroStats m_currentStats;
    
    // Hero level
    int m_level;
    int m_experience;
    
    // Abilities
    std::vector<Entity> m_abilities;
    
    // Current state
    float m_currentHealth;
    float m_currentMana;
    bool m_alive;
    
    // Cooldowns and effects
    std::unordered_map<std::string, float> m_cooldowns;
    std::vector<std::pair<std::string, float>> m_statusEffects;
    
    // Skin/NFT data
    std::string m_skinId;
    std::string m_skinName;
    
    // Blockchain wallet entity
    Entity m_wallet;

public:
    HeroComponent();
    ~HeroComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set hero name
    void SetName(const std::string& name) { m_name = name; }
    
    // Get hero name
    const std::string& GetName() const { return m_name; }
    
    // Set hero description
    void SetDescription(const std::string& description) { m_description = description; }
    
    // Get hero description
    const std::string& GetDescription() const { return m_description; }
    
    // Set hero role
    void SetRole(const std::string& role) { m_role = role; }
    
    // Get hero role
    const std::string& GetRole() const { return m_role; }
    
    // Set base stats
    void SetBaseStats(const HeroStats& stats) { m_baseStats = stats; }
    
    // Get base stats
    const HeroStats& GetBaseStats() const { return m_baseStats; }
    
    // Get current stats
    const HeroStats& GetCurrentStats() const { return m_currentStats; }
    
    // Set level
    void SetLevel(int level);
    
    // Get level
    int GetLevel() const { return m_level; }
    
    // Add experience
    void AddExperience(int experience);
    
    // Get experience
    int GetExperience() const { return m_experience; }
    
    // Add ability
    void AddAbility(Entity ability);
    
    // Remove ability
    void RemoveAbility(Entity ability);
    
    // Get abilities
    const std::vector<Entity>& GetAbilities() const { return m_abilities; }
    
    // Set current health
    void SetCurrentHealth(float health);
    
    // Get current health
    float GetCurrentHealth() const { return m_currentHealth; }
    
    // Set current mana
    void SetCurrentMana(float mana);
    
    // Get current mana
    float GetCurrentMana() const { return m_currentMana; }
    
    // Check if hero is alive
    bool IsAlive() const { return m_alive; }
    
    // Take damage
    float TakeDamage(float damage, bool isMagical = false);
    
    // Heal
    float Heal(float amount);
    
    // Use mana
    bool UseMana(float amount);
    
    // Restore mana
    float RestoreMana(float amount);
    
    // Set cooldown
    void SetCooldown(const std::string& ability, float duration);
    
    // Get cooldown
    float GetCooldown(const std::string& ability) const;
    
    // Check if ability is on cooldown
    bool IsOnCooldown(const std::string& ability) const;
    
    // Add status effect
    void AddStatusEffect(const std::string& effect, float duration);
    
    // Remove status effect
    void RemoveStatusEffect(const std::string& effect);
    
    // Check if has status effect
    bool HasStatusEffect(const std::string& effect) const;
    
    // Set skin
    void SetSkin(const std::string& skinId, const std::string& skinName);
    
    // Get skin ID
    const std::string& GetSkinId() const { return m_skinId; }
    
    // Get skin name
    const std::string& GetSkinName() const { return m_skinName; }
    
    // Set wallet entity
    void SetWallet(Entity wallet) { m_wallet = wallet; }
    
    // Get wallet entity
    Entity GetWallet() const { return m_wallet; }
    
    // Update the hero
    void Update(float deltaTime);
    
    // Reset the hero (full health/mana, clear cooldowns and effects)
    void Reset();
};

/**
 * @brief Hero system for managing heroes
 */
class HeroSystem : public System {
private:
    // Hero templates
    std::unordered_map<std::string, HeroComponent> m_heroTemplates;
    
    // Active heroes
    std::vector<Entity> m_activeHeroes;
    
    // Hero factory methods
    Entity CreateHeroFromTemplate(const std::string& templateName);

public:
    HeroSystem(EntityManager* manager);
    ~HeroSystem() override;
    
    // Initialize the system
    void Initialize() override;
    
    // Update the system
    void Update(float deltaTime) override;
    
    // Called when an entity is added to this system
    void OnEntityAdded(Entity entity) override;
    
    // Called when an entity is removed from this system
    void OnEntityRemoved(Entity entity) override;
    
    // Register a hero template
    void RegisterHeroTemplate(const std::string& name, const HeroComponent& heroTemplate);
    
    // Get a hero template
    const HeroComponent* GetHeroTemplate(const std::string& name) const;
    
    // Create a hero from a template
    Entity CreateHero(const std::string& templateName);
    
    // Create a custom hero
    Entity CreateCustomHero(const std::string& name, const std::string& description, const std::string& role, const HeroStats& stats);
    
    // Get active heroes
    const std::vector<Entity>& GetActiveHeroes() const { return m_activeHeroes; }
    
    // Load hero templates from a JSON file
    bool LoadHeroTemplatesFromFile(const std::string& filename);
    
    // Save hero templates to a JSON file
    bool SaveHeroTemplatesToFile(const std::string& filename) const;
};

} // namespace CHULUBME

