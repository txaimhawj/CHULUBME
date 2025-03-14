#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace CHULUBME {

/**
 * @brief Base allocator interface
 */
class Allocator {
public:
    virtual ~Allocator() = default;
    
    // Allocate memory of specified size with optional alignment
    virtual void* Allocate(size_t size, size_t alignment = 0) = 0;
    
    // Free previously allocated memory
    virtual void Free(void* ptr) = 0;
    
    // Get the total size of allocated memory
    virtual size_t GetAllocatedSize() const = 0;
    
    // Get the total size of the memory pool
    virtual size_t GetTotalSize() const = 0;
};

/**
 * @brief Linear allocator - allocates memory linearly, can only free all memory at once
 */
class LinearAllocator : public Allocator {
private:
    uint8_t* m_memory;
    size_t m_size;
    size_t m_offset;
    bool m_ownsMemory;

public:
    // Create a linear allocator with a specified size
    LinearAllocator(size_t size);
    
    // Create a linear allocator with pre-allocated memory
    LinearAllocator(void* memory, size_t size);
    
    // Destructor
    ~LinearAllocator() override;
    
    // Allocate memory
    void* Allocate(size_t size, size_t alignment = 0) override;
    
    // Free all memory (individual frees are not supported)
    void Free(void* ptr) override;
    
    // Reset the allocator (free all memory)
    void Reset();
    
    // Get the total size of allocated memory
    size_t GetAllocatedSize() const override { return m_offset; }
    
    // Get the total size of the memory pool
    size_t GetTotalSize() const override { return m_size; }
};

/**
 * @brief Pool allocator - allocates fixed-size blocks of memory
 */
class PoolAllocator : public Allocator {
private:
    uint8_t* m_memory;
    size_t m_blockSize;
    size_t m_blockCount;
    size_t m_freeBlocks;
    void* m_freeList;
    bool m_ownsMemory;

public:
    // Create a pool allocator with specified block size and count
    PoolAllocator(size_t blockSize, size_t blockCount);
    
    // Create a pool allocator with pre-allocated memory
    PoolAllocator(void* memory, size_t blockSize, size_t blockCount);
    
    // Destructor
    ~PoolAllocator() override;
    
    // Allocate a block of memory
    void* Allocate(size_t size, size_t alignment = 0) override;
    
    // Free a block of memory
    void Free(void* ptr) override;
    
    // Get the total size of allocated memory
    size_t GetAllocatedSize() const override { return (m_blockCount - m_freeBlocks) * m_blockSize; }
    
    // Get the total size of the memory pool
    size_t GetTotalSize() const override { return m_blockCount * m_blockSize; }
};

/**
 * @brief Stack allocator - allocates memory in a stack-like fashion
 */
class StackAllocator : public Allocator {
private:
    struct Marker {
        size_t offset;
        size_t adjustment;
    };
    
    uint8_t* m_memory;
    size_t m_size;
    size_t m_offset;
    bool m_ownsMemory;

public:
    // Create a stack allocator with a specified size
    StackAllocator(size_t size);
    
    // Create a stack allocator with pre-allocated memory
    StackAllocator(void* memory, size_t size);
    
    // Destructor
    ~StackAllocator() override;
    
    // Allocate memory
    void* Allocate(size_t size, size_t alignment = 0) override;
    
    // Free memory (must be the most recently allocated block)
    void Free(void* ptr) override;
    
    // Get a marker for the current position
    Marker GetMarker() const;
    
    // Free all memory up to a marker
    void FreeToMarker(Marker marker);
    
    // Reset the allocator (free all memory)
    void Reset();
    
    // Get the total size of allocated memory
    size_t GetAllocatedSize() const override { return m_offset; }
    
    // Get the total size of the memory pool
    size_t GetTotalSize() const override { return m_size; }
};

/**
 * @brief Memory manager - manages different allocators for different purposes
 */
class MemoryManager {
public:
    // Allocator types
    enum class AllocatorType {
        Default,    // Default allocator (system malloc/free)
        Linear,     // Linear allocator
        Pool,       // Pool allocator
        Stack       // Stack allocator
    };

private:
    // Singleton instance
    static std::unique_ptr<MemoryManager> s_instance;
    
    // Allocators
    std::unordered_map<AllocatorType, std::unique_ptr<Allocator>> m_allocators;
    
    // Mutex for thread safety
    std::mutex m_mutex;
    
    // Private constructor for singleton
    MemoryManager();

public:
    // Get singleton instance
    static MemoryManager& Instance();
    
    // Destroy singleton instance
    static void DestroyInstance();
    
    // Initialize the memory manager
    bool Initialize();
    
    // Shutdown the memory manager
    void Shutdown();
    
    // Allocate memory using a specific allocator
    void* Allocate(AllocatorType type, size_t size, size_t alignment = 0);
    
    // Free memory allocated with a specific allocator
    void Free(AllocatorType type, void* ptr);
    
    // Get an allocator
    Allocator* GetAllocator(AllocatorType type);
    
    // Register a custom allocator
    void RegisterAllocator(AllocatorType type, std::unique_ptr<Allocator> allocator);
    
    // Get memory usage statistics
    struct MemoryStats {
        size_t totalAllocated;
        size_t totalReserved;
        std::unordered_map<AllocatorType, size_t> allocatorUsage;
    };
    
    MemoryStats GetMemoryStats() const;
};

// Smart pointer that uses a specific allocator
template<typename T, MemoryManager::AllocatorType Type = MemoryManager::AllocatorType::Default>
class AllocatedPtr {
private:
    T* m_ptr;

public:
    // Constructor
    AllocatedPtr() : m_ptr(nullptr) {}
    
    // Constructor with initialization
    template<typename... Args>
    AllocatedPtr(Args&&... args) {
        m_ptr = new (MemoryManager::Instance().Allocate(Type, sizeof(T))) T(std::forward<Args>(args)...);
    }
    
    // Destructor
    ~AllocatedPtr() {
        if (m_ptr) {
            m_ptr->~T();
            MemoryManager::Instance().Free(Type, m_ptr);
        }
    }
    
    // Move constructor
    AllocatedPtr(AllocatedPtr&& other) noexcept : m_ptr(other.m_ptr) {
        other.m_ptr = nullptr;
    }
    
    // Move assignment
    AllocatedPtr& operator=(AllocatedPtr&& other) noexcept {
        if (this != &other) {
            if (m_ptr) {
                m_ptr->~T();
                MemoryManager::Instance().Free(Type, m_ptr);
            }
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }
    
    // Disable copy constructor and assignment
    AllocatedPtr(const AllocatedPtr&) = delete;
    AllocatedPtr& operator=(const AllocatedPtr&) = delete;
    
    // Dereference operators
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    
    // Get the raw pointer
    T* Get() const { return m_ptr; }
    
    // Check if pointer is valid
    explicit operator bool() const { return m_ptr != nullptr; }
    
    // Reset the pointer
    void Reset() {
        if (m_ptr) {
            m_ptr->~T();
            MemoryManager::Instance().Free(Type, m_ptr);
            m_ptr = nullptr;
        }
    }
    
    // Create a new instance
    template<typename... Args>
    void Reset(Args&&... args) {
        Reset();
        m_ptr = new (MemoryManager::Instance().Allocate(Type, sizeof(T))) T(std::forward<Args>(args)...);
    }
};

// Factory function to create an allocated pointer
template<typename T, MemoryManager::AllocatorType Type = MemoryManager::AllocatorType::Default, typename... Args>
AllocatedPtr<T, Type> MakeAllocated(Args&&... args) {
    return AllocatedPtr<T, Type>(std::forward<Args>(args)...);
}

} // namespace CHULUBME

