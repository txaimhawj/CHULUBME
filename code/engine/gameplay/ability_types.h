#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../core/ecs.h"

namespace CHULUBME {

// Forward declarations
class HeroComponent;

/**
 * @brief Ability target type
 */
enum class AbilityTargetType {
    NONE,           // No target required (self-cast)
    UNIT,           // Single unit target
    DIRECTION,      // Direction target
    AREA,           // Area target
    LOCATION        // Location target
};

/**
 * @brief Ability effect type
 */
enum class AbilityEffectType {
    DAMAGE,         // Deal damage
    HEAL,           // Heal
    BUFF,           // Apply buff
    DEBUFF,         // Apply debuff
    CROWD_CONTROL,  // Apply crowd control
    UTILITY,        // Utility effect
    MOVEMENT        // Movement effect
};

/**
 * @brief Ability data
 */
struct AbilityData {
    // Basic info
    std::string name;
    std::string description;
    std::string icon;
    
    // Targeting
    AbilityTargetType targetType;
    float range;
    float areaRadius;
    
    // Costs and cooldown
    float manaCost;
    float cooldown;
    
    // Scaling
    float baseDamage;
    float damageScaling;
    float baseHeal;
    float healScaling;
    
    // Effects
    std::vector<AbilityEffectType> effectTypes;
    float effectDuration;
    
    // Constructor with default values
    AbilityData();
};

/**
 * @brief Base ability component
 */
class AbilityComponent : public Component {
protected:
    // Ability data
    AbilityData m_data;
    
    // Owner entity
    Entity m_owner;
    
    // Current state
    bool m_active;
    int m_level;
    
    // Callback for ability execution
    std::function<void(Entity, Entity, const std::vector<Entity>&)> m_executeCallback;

public:
    AbilityComponent();
    virtual ~AbilityComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set ability data
    void SetData(const AbilityData& data) { m_data = data; }
    
    // Get ability data
    const AbilityData& GetData() const { return m_data; }
    
    // Set owner entity
    void SetOwner(Entity owner) { m_owner = owner; }
    
    // Get owner entity
    Entity GetOwner() const { return m_owner; }
    
    // Set active state
    void SetActive(bool active) { m_active = active; }
    
    // Check if ability is active
    bool IsActive() const { return m_active; }
    
    // Set level
    void SetLevel(int level) { m_level = level; }
    
    // Get level
    int GetLevel() const { return m_level; }
    
    // Set execute callback
    void SetExecuteCallback(std::function<void(Entity, Entity, const std::vector<Entity>&)> callback) { m_executeCallback = callback; }
    
    // Execute the ability
    virtual bool Execute(Entity target, const std::vector<Entity>& additionalTargets = {});
    
    // Get mana cost at current level
    virtual float GetManaCost() const;
    
    // Get cooldown at current level
    virtual float GetCooldown() const;
    
    // Get damage at current level
    virtual float GetDamage(const HeroComponent* owner) const;
    
    // Get heal amount at current level
    virtual float GetHeal(const HeroComponent* owner) const;
    
    // Update the ability
    virtual void Update(float deltaTime);
};

/**
 * @brief Targeted ability component
 */
class TargetedAbilityComponent : public AbilityComponent {
private:
    // Additional data for targeted abilities
    float m_castTime;
    bool m_isCasting;
    float m_castProgress;
    Entity m_currentTarget;

public:
    TargetedAbilityComponent();
    ~TargetedAbilityComponent() override;
    
    // Set cast time
    void SetCastTime(float castTime) { m_castTime = castTime; }
    
    // Get cast time
    float GetCastTime() const { return m_castTime; }
    
    // Check if ability is casting
    bool IsCasting() const { return m_isCasting; }
    
    // Get cast progress
    float GetCastProgress() const { return m_castProgress; }
    
    // Get current target
    Entity GetCurrentTarget() const { return m_currentTarget; }
    
    // Execute the ability
    bool Execute(Entity target, const std::vector<Entity>& additionalTargets = {}) override;
    
    // Cancel casting
    void CancelCast();
    
    // Update the ability
    void Update(float deltaTime) override;
};

/**
 * @brief Area of effect ability component
 */
class AreaAbilityComponent : public AbilityComponent {
private:
    // Additional data for area abilities
    float m_castTime;
    bool m_isCasting;
    float m_castProgress;
    float m_targetX;
    float m_targetY;
    float m_targetZ;

public:
    AreaAbilityComponent();
    ~AreaAbilityComponent() override;
    
    // Set cast time
    void SetCastTime(float castTime) { m_castTime = castTime; }
    
    // Get cast time
    float GetCastTime() const { return m_castTime; }
    
    // Check if ability is casting
    bool IsCasting() const { return m_isCasting; }
    
    // Get cast progress
    float GetCastProgress() const { return m_castProgress; }
    
    // Get target location
    void GetTargetLocation(float& x, float& y, float& z) const { x = m_targetX; y = m_targetY; z = m_targetZ; }
    
    // Execute the ability
    bool Execute(Entity target, const std::vector<Entity>& additionalTargets = {}) override;
    
    // Execute the ability at a location
    bool ExecuteAtLocation(float x, float y, float z);
    
    // Cancel casting
    void CancelCast();
    
    // Update the ability
    void Update(float deltaTime) override;
};

/**
 * @brief Passive ability component
 */
class PassiveAbilityComponent : public AbilityComponent {
private:
    // Additional data for passive abilities
    bool m_isToggleable;
    bool m_isToggled;

public:
    PassiveAbilityComponent();
    ~PassiveAbilityComponent() override;
    
    // Set toggleable
    void SetToggleable(bool toggleable) { m_isToggleable = toggleable; }
    
    // Check if ability is toggleable
    bool IsToggleable() const { return m_isToggleable; }
    
    // Set toggled state
    void SetToggled(bool toggled);
    
    // Check if ability is toggled
    bool IsToggled() const { return m_isToggled; }
    
    // Execute the ability (toggle for toggleable passives)
    bool Execute(Entity target, const std::vector<Entity>& additionalTargets = {}) override;
    
    // Update the ability
    void Update(float deltaTime) override;
};

/**
 * @brief Ability system for managing abilities
 */
class AbilitySystem : public System {
private:
    // Ability templates
    std::unordered_map<std::string, std::shared_ptr<AbilityComponent>> m_abilityTemplates;
    
    // Active abilities
    std::vector<Entity> m_activeAbilities;

public:
    AbilitySystem(EntityManager* manager);
    ~AbilitySystem() override;
    
    // Initialize the system
    void Initialize() override;
    
    // Update the system
    void Update(float deltaTime) override;
    
    // Called when an entity is added to this system
    void OnEntityAdded(Entity entity) override;
    
    // Called when an entity is removed from this system
    void OnEntityRemoved(Entity entity) override;
    
    // Register an ability template
    void RegisterAbilityTemplate(const std::string& name, std::shared_ptr<AbilityComponent> abilityTemplate);
    
    // Get an ability template
    std::shared_ptr<AbilityComponent> GetAbilityTemplate(const std::string& name) const;
    
    // Create an ability from a template
    Entity CreateAbility(const std::string& templateName, Entity owner);
    
    // Create a custom ability
    Entity CreateCustomAbility(const AbilityData& data, Entity owner);
    
    // Get active abilities
    const std::vector<Entity>& GetActiveAbilities() const { return m_activeAbilities; }
    
    // Load ability templates from a JSON file
    bool LoadAbilityTemplatesFromFile(const std::string& filename);
    
    // Save ability templates to a JSON file
    bool SaveAbilityTemplatesToFile(const std::string& filename) const;
};

} // namespace CHULUBME

