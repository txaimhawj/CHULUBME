#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../core/ecs.h"

namespace CHULUBME {

/**
 * @brief Interface for communicating with the blockchain
 */
class BlockchainInterface {
public:
    // Transaction status
    enum class TransactionStatus {
        PENDING,
        CONFIRMED,
        FAILED
    };
    
    // Transaction data
    struct Transaction {
        std::string id;
        std::string type;
        std::string sender;
        std::string recipient;
        double amount;
        std::string data;
        int64_t timestamp;
        std::string signature;
        TransactionStatus status;
    };
    
    // NFT data
    struct NFT {
        std::string id;
        std::string type;
        std::string owner;
        std::string creator;
        std::unordered_map<std::string, std::string> metadata;
        int64_t createdAt;
        double yieldRate;
        int64_t lastYield;
        bool isListed;
        double listPrice;
        int64_t listedAt;
    };
    
    // Wallet data
    struct Wallet {
        std::string address;
        std::string publicKey;
        double balance;
        std::vector<NFT> nfts;
        std::vector<std::string> transactions;
        int64_t createdAt;
        int64_t lastUpdated;
    };

private:
    // Singleton instance
    static std::unique_ptr<BlockchainInterface> s_instance;
    
    // Current wallet
    Wallet m_currentWallet;
    
    // Transaction callbacks
    std::unordered_map<std::string, std::function<void(const Transaction&)>> m_transactionCallbacks;
    
    // Private constructor for singleton
    BlockchainInterface();

public:
    // Get singleton instance
    static BlockchainInterface& Instance();
    
    // Destroy singleton instance
    static void DestroyInstance();
    
    // Initialize the blockchain interface
    bool Initialize();
    
    // Shutdown the blockchain interface
    void Shutdown();
    
    // Update the blockchain interface
    void Update();
    
    // Connect to the blockchain
    bool Connect(const std::string& endpoint);
    
    // Disconnect from the blockchain
    void Disconnect();
    
    // Check if connected to the blockchain
    bool IsConnected() const;
    
    // Create a new wallet
    Wallet CreateWallet();
    
    // Load a wallet from a JSON string
    Wallet LoadWallet(const std::string& jsonData);
    
    // Save a wallet to a JSON string
    std::string SaveWallet(const Wallet& wallet, bool includePrivateKey = false);
    
    // Set the current wallet
    void SetCurrentWallet(const Wallet& wallet);
    
    // Get the current wallet
    const Wallet& GetCurrentWallet() const;
    
    // Get the balance of a wallet
    double GetBalance(const std::string& address);
    
    // Get the NFTs owned by a wallet
    std::vector<NFT> GetNFTs(const std::string& address);
    
    // Get the transactions of a wallet
    std::vector<Transaction> GetTransactions(const std::string& address);
    
    // Create a transaction
    Transaction CreateTransaction(const std::string& type, const std::string& recipient, double amount, const std::string& data);
    
    // Sign a transaction
    bool SignTransaction(Transaction& transaction, const std::string& privateKey);
    
    // Send a transaction
    bool SendTransaction(const Transaction& transaction);
    
    // Get a transaction by ID
    Transaction GetTransaction(const std::string& id);
    
    // Register a transaction callback
    void RegisterTransactionCallback(const std::string& id, std::function<void(const Transaction&)> callback);
    
    // Unregister a transaction callback
    void UnregisterTransactionCallback(const std::string& id);
    
    // Get an NFT by ID
    NFT GetNFT(const std::string& id);
    
    // Create an NFT
    NFT CreateNFT(const std::string& type, const std::string& creator, const std::unordered_map<std::string, std::string>& metadata, double yieldRate = 0.0);
    
    // Transfer an NFT
    bool TransferNFT(const std::string& id, const std::string& recipient);
    
    // List an NFT for sale
    bool ListNFT(const std::string& id, double price);
    
    // Unlist an NFT
    bool UnlistNFT(const std::string& id);
    
    // Buy an NFT
    bool BuyNFT(const std::string& id);
    
    // Calculate yield for an NFT
    double CalculateYield(const std::string& id, double stakedAmount);
    
    // Get listed NFTs
    std::vector<NFT> GetListedNFTs();
    
    // Calculate game reward
    double CalculateGameReward(int64_t matchDuration, int playerRank, double performanceScore, int activePlayerCount);
};

/**
 * @brief Wallet component for entities that interact with the blockchain
 */
class WalletComponent : public Component {
private:
    // Wallet data
    BlockchainInterface::Wallet m_wallet;
    
    // Whether the wallet is loaded
    bool m_loaded;
    
    // Transaction callbacks
    std::unordered_map<std::string, std::function<void(const BlockchainInterface::Transaction&)>> m_transactionCallbacks;

public:
    WalletComponent();
    ~WalletComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set wallet
    void SetWallet(const BlockchainInterface::Wallet& wallet);
    
    // Get wallet
    const BlockchainInterface::Wallet& GetWallet() const;
    
    // Check if wallet is loaded
    bool IsLoaded() const;
    
    // Create a transaction
    BlockchainInterface::Transaction CreateTransaction(const std::string& type, const std::string& recipient, double amount, const std::string& data);
    
    // Send a transaction
    bool SendTransaction(const BlockchainInterface::Transaction& transaction);
    
    // Register a transaction callback
    void RegisterTransactionCallback(const std::string& id, std::function<void(const BlockchainInterface::Transaction&)> callback);
    
    // Unregister a transaction callback
    void UnregisterTransactionCallback(const std::string& id);
    
    // Update the wallet
    void Update();
};

/**
 * @brief NFT component for entities that represent NFTs
 */
class NFTComponent : public Component {
private:
    // NFT data
    BlockchainInterface::NFT m_nft;
    
    // Whether the NFT is loaded
    bool m_loaded;
    
    // Owner entity
    Entity m_owner;

public:
    NFTComponent();
    ~NFTComponent() override;
    
    // Initialize the component
    void Initialize() override;
    
    // Finalize the component
    void Finalize() override;
    
    // Set NFT
    void SetNFT(const BlockchainInterface::NFT& nft);
    
    // Get NFT
    const BlockchainInterface::NFT& GetNFT() const;
    
    // Check if NFT is loaded
    bool IsLoaded() const;
    
    // Set owner entity
    void SetOwner(Entity owner);
    
    // Get owner entity
    Entity GetOwner() const;
    
    // Transfer the NFT
    bool Transfer(const std::string& recipient);
    
    // List the NFT for sale
    bool List(double price);
    
    // Unlist the NFT
    bool Unlist();
    
    // Calculate yield
    double CalculateYield(double stakedAmount);
    
    // Update the NFT
    void Update();
};

} // namespace CHULUBME

