#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include "../core/ecs.h"

namespace CHULUBME {

/**
 * @brief Input manager for handling keyboard and mouse input
 */
class InputManager {
public:
    // Key codes
    enum class KeyCode : uint32_t {
        // Keyboard keys
        KEY_UNKNOWN = 0,
        KEY_A = 4,
        KEY_B = 5,
        KEY_C = 6,
        KEY_D = 7,
        KEY_E = 8,
        KEY_F = 9,
        KEY_G = 10,
        KEY_H = 11,
        KEY_I = 12,
        KEY_J = 13,
        KEY_K = 14,
        KEY_L = 15,
        KEY_M = 16,
        KEY_N = 17,
        KEY_O = 18,
        KEY_P = 19,
        KEY_Q = 20,
        KEY_R = 21,
        KEY_S = 22,
        KEY_T = 23,
        KEY_U = 24,
        KEY_V = 25,
        KEY_W = 26,
        KEY_X = 27,
        KEY_Y = 28,
        KEY_Z = 29,
        KEY_1 = 30,
        KEY_2 = 31,
        KEY_3 = 32,
        KEY_4 = 33,
        KEY_5 = 34,
        KEY_6 = 35,
        KEY_7 = 36,
        KEY_8 = 37,
        KEY_9 = 38,
        KEY_0 = 39,
        KEY_RETURN = 40,
        KEY_ESCAPE = 41,
        KEY_BACKSPACE = 42,
        KEY_TAB = 43,
        KEY_SPACE = 44,
        KEY_MINUS = 45,
        KEY_EQUALS = 46,
        KEY_LEFTBRACKET = 47,
        KEY_RIGHTBRACKET = 48,
        KEY_BACKSLASH = 49,
        KEY_SEMICOLON = 51,
        KEY_APOSTROPHE = 52,
        KEY_GRAVE = 53,
        KEY_COMMA = 54,
        KEY_PERIOD = 55,
        KEY_SLASH = 56,
        KEY_CAPSLOCK = 57,
        KEY_F1 = 58,
        KEY_F2 = 59,
        KEY_F3 = 60,
        KEY_F4 = 61,
        KEY_F5 = 62,
        KEY_F6 = 63,
        KEY_F7 = 64,
        KEY_F8 = 65,
        KEY_F9 = 66,
        KEY_F10 = 67,
        KEY_F11 = 68,
        KEY_F12 = 69,
        KEY_PRINTSCREEN = 70,
        KEY_SCROLLLOCK = 71,
        KEY_PAUSE = 72,
        KEY_INSERT = 73,
        KEY_HOME = 74,
        KEY_PAGEUP = 75,
        KEY_DELETE = 76,
        KEY_END = 77,
        KEY_PAGEDOWN = 78,
        KEY_RIGHT = 79,
        KEY_LEFT = 80,
        KEY_DOWN = 81,
        KEY_UP = 82,
        KEY_NUMLOCKCLEAR = 83,
        KEY_KP_DIVIDE = 84,
        KEY_KP_MULTIPLY = 85,
        KEY_KP_MINUS = 86,
        KEY_KP_PLUS = 87,
        KEY_KP_ENTER = 88,
        KEY_KP_1 = 89,
        KEY_KP_2 = 90,
        KEY_KP_3 = 91,
        KEY_KP_4 = 92,
        KEY_KP_5 = 93,
        KEY_KP_6 = 94,
        KEY_KP_7 = 95,
        KEY_KP_8 = 96,
        KEY_KP_9 = 97,
        KEY_KP_0 = 98,
        KEY_KP_PERIOD = 99,
        KEY_LCTRL = 224,
        KEY_LSHIFT = 225,
        KEY_LALT = 226,
        KEY_LGUI = 227,
        KEY_RCTRL = 228,
        KEY_RSHIFT = 229,
        KEY_RALT = 230,
        KEY_RGUI = 231,
        
        // Mouse buttons
        MOUSE_LEFT = 1000,
        MOUSE_MIDDLE = 1001,
        MOUSE_RIGHT = 1002,
        MOUSE_X1 = 1003,
        MOUSE_X2 = 1004
    };
    
    // Key state
    enum class KeyState : uint8_t {
        RELEASED = 0,
        PRESSED = 1,
        HELD = 2
    };
    
    // Mouse state
    struct MouseState {
        int x;
        int y;
        int deltaX;
        int deltaY;
        int scrollX;
        int scrollY;
    };

private:
    // Singleton instance
    static std::unique_ptr<InputManager> s_instance;
    
    // Key states
    std::unordered_map<KeyCode, KeyState> m_keyStates;
    std::unordered_map<KeyCode, KeyState> m_previousKeyStates;
    
    // Mouse state
    MouseState m_mouseState;
    MouseState m_previousMouseState;
    
    // Key callbacks
    std::unordered_map<KeyCode, std::vector<std::function<void(KeyState)>>> m_keyCallbacks;
    
    // Mouse move callbacks
    std::vector<std::function<void(int, int, int, int)>> m_mouseMoveCallbacks;
    
    // Mouse scroll callbacks
    std::vector<std::function<void(int, int)>> m_mouseScrollCallbacks;
    
    // Private constructor for singleton
    InputManager();

public:
    // Get singleton instance
    static InputManager& Instance();
    
    // Destroy singleton instance
    static void DestroyInstance();
    
    // Initialize the input manager
    bool Initialize();
    
    // Shutdown the input manager
    void Shutdown();
    
    // Update the input manager
    void Update();
    
    // Process a key event
    void ProcessKeyEvent(KeyCode key, KeyState state);
    
    // Process a mouse move event
    void ProcessMouseMoveEvent(int x, int y);
    
    // Process a mouse scroll event
    void ProcessMouseScrollEvent(int scrollX, int scrollY);
    
    // Get the state of a key
    KeyState GetKeyState(KeyCode key) const;
    
    // Check if a key is pressed (just pressed this frame)
    bool IsKeyPressed(KeyCode key) const;
    
    // Check if a key is held (pressed and held)
    bool IsKeyHeld(KeyCode key) const;
    
    // Check if a key is released (just released this frame)
    bool IsKeyReleased(KeyCode key) const;
    
    // Get the mouse state
    const MouseState& GetMouseState() const { return m_mouseState; }
    
    // Register a key callback
    void RegisterKeyCallback(KeyCode key, std::function<void(KeyState)> callback);
    
    // Register a mouse move callback
    void RegisterMouseMoveCallback(std::function<void(int, int, int, int)> callback);
    
    // Register a mouse scroll callback
    void RegisterMouseScrollCallback(std::function<void(int, int)> callback);
    
    // Unregister all callbacks
    void UnregisterAllCallbacks();
};

/**
 * @brief Input component for entity-specific input handling
 */
class InputComponent : public Component {
private:
    // Key callbacks
    std::unordered_map<InputManager::KeyCode, std::function<void(InputManager::KeyState)>> m_keyCallbacks;
    
    // Mouse move callback
    std::function<void(int, int, int, int)> m_mouseMoveCallback;
    
    // Mouse scroll callback
    std::function<void(int, int)> m_mouseScrollCallback;
    
    // Whether the component is active
    bool m_active;

public:
    InputComponent();
    ~InputComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Register a key callback
    void RegisterKeyCallback(InputManager::KeyCode key, std::function<void(InputManager::KeyState)> callback);
    
    // Register a mouse move callback
    void RegisterMouseMoveCallback(std::function<void(int, int, int, int)> callback);
    
    // Register a mouse scroll callback
    void RegisterMouseScrollCallback(std::function<void(int, int)> callback);
    
    // Set active state
    void SetActive(bool active);
    
    // Check if the component is active
    bool IsActive() const { return m_active; }
};

} // namespace CHULUBME

