# CHULUBME Hero Creation Workflow

This document outlines the process for creating and testing heroes in the CHULUBME MOBA game.

## Overview

The CHULUBME hero system is built on an Entity Component System (ECS) architecture, which allows for flexible and modular hero design. Heroes are composed of various components, including:

- **HeroComponent**: Core hero stats and abilities
- **TransformComponent**: Position and orientation in the game world
- **MeshRendererComponent**: Visual representation
- **InputComponent**: Player input handling
- **NFTComponent**: Blockchain-based skin ownership

## Hero Creation Process

### 1. Using the Hero Editor

The Hero Editor provides a graphical interface for creating and editing heroes:

```cpp
// Create a new hero editor
HeroEditor editor(engine.GetEntityManager());
editor.Initialize();

// Create a new hero
Entity hero = editor.CreateNewHero();

// Set hero properties
editor.SetHeroName("New Hero");
editor.SetHeroDescription("A powerful new champion");
editor.SetHeroRole("Fighter");

// Set hero stats
HeroStats stats;
stats.health = 600;
stats.mana = 300;
stats.attackDamage = 65;
// ... set other stats

editor.SetHeroStats(stats);

// Save the hero
hero = editor.SaveHero();
// Create a new ability
Entity ability = editor.CreateNewAbility();

// Set ability properties
AbilityData abilityData;
abilityData.name = "Powerful Strike";
abilityData.description = "Deals heavy damage to a single target";
abilityData.targetType = AbilityTargetType::UNIT;
abilityData.range = 300;
abilityData.manaCost = 50;
abilityData.cooldown = 8;
abilityData.baseDamage = 100;
abilityData.damageScaling = 0.7;
abilityData.effectTypes = {AbilityEffectType::DAMAGE};

editor.SetAbilityData(abilityData);

// Save the ability
ability = editor.SaveAbility();

// Add the ability to the hero
editor.LoadHero(hero);
editor.LoadAbility(ability);
editor.AddAbilityToHero();
// Save heroes to a file
editor.SaveHeroesToFile("heroes.json");

// Load heroes from a file
editor.LoadHeroesFromFile("heroes.json");
// Create a test environment
TestEnvironment testEnv(engine.GetEntityManager());
testEnv.Initialize();

// Load a hero for testing
Entity testHero = testEnv.LoadHero("Axe Master");

// Simulate hero actions
testEnv.SimulateAbilityUse(testHero, 0, targetEntity); // Use first ability
testEnv.SimulateMovement(testHero, x, y, z); // Move hero
testEnv.SimulateCombat(testHero, enemyEntity); // Test combat
// Get the hero's wallet component
WalletComponent* wallet = hero.GetComponent<WalletComponent>();

// Check if the player owns a specific skin NFT
BlockchainInterface::NFT skinNFT = BlockchainInterface::Instance().GetNFT("skin_123");
if (skinNFT.owner == wallet->GetWallet().address) {
    // Apply the skin to the hero
    hero.GetComponent<HeroComponent>()->SetSkin(skinNFT.id, skinNFT.metadata["name"]);
}


