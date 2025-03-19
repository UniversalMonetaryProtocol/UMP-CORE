// Copyright (c) 2025 The Universal Monetary Protocol Developers
// Distributed under the MIT software license.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// Genesis Block Creation Function
//-----------------------------------------------------------------------------

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript &genesisOutputScript,
                                 uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                                 int32_t nVersion, const CAmount &genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    // Embed the timestamp message into the coinbase script.
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
        << std::vector<unsigned char>((const unsigned char *)pszTimestamp,
                                      (const unsigned char *)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    // Use the provided public key (uncompressed, starting with "04")
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

// Helper overload that uses our custom genesis parameters.
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                                 int32_t nVersion, const CAmount &genesisReward)
{
    // New genesis block message.
    const char *pszTimestamp = "March 17 2025 May the luck of the irish be with you always";
    // Provided public key in uncompressed format.
    const CScript genesisOutputScript =
        CScript() << ParseHex("04E222ED9925AEC7C1B19D24C59E4CBAF7ECB2F89DCE6A9A904877F5EF91244635B6C0157BBF4C29116B792FB1B12B4935CD9EDD8C663C3B95647FA209DF58105C")
                  << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

//-----------------------------------------------------------------------------
// Main Network Parameters
//-----------------------------------------------------------------------------

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // Set consensus parameters.
        consensus.nSubsidyHalvingInterval = 500000; // Halve every 500,000 blocks.
        consensus.nPowTargetTimespan = 60;          // Retarget every 1 minute.
        consensus.nPowTargetSpacing = 60;           // 1 minute block time.
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;

        // Block reward is 1 coin.
        const CAmount genesisReward = 1 * COIN;

        // Custom proof-of-work limit.
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // Unique network message start string.
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        // RPC port for main net.
        nDefaultPort = 4332;
        nPruneAfterHeight = 1000;

        // Create the genesis block using provided main net parameters.
        genesis = CreateGenesisBlock(1742232311, 2000090602, 0x1e0ffff0, 1, genesisReward);
        consensus.hashGenesisBlock = genesis.GetHash();

        // Verify genesis block values.
        assert(genesis.hashMerkleRoot ==
               uint256S("0x0910b8c51236d0adb3444604cfcb019f657ea358ade9baeda804d54ca9582362"));
        assert(consensus.hashGenesisBlock ==
               uint256S("0xa545728341a1fba7c2deaa9ccfd86de38e9ad20f30ea3fcbe2d5215d1c079bde"));

        // Set default nodes (DNS seeds).
        vSeeds.clear();
        vSeeds.emplace_back("seed1", "18.224.250.194", false);
        vSeeds.emplace_back("seed2", "18.189.127.19", false);
        vSeeds.emplace_back("seed3", "3.136.34.227", false);
        vSeeds.emplace_back("seed4", "3.146.90.153", false);
        vSeeds.emplace_back("seed5", "3.146.108.5", false);
        vFixedSeeds.clear();

        // Base58 prefixes.
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 68);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // No checkpoints for this new fork.
        checkpointData = (CCheckpointData){ { } };
        chainTxData = ChainTxData{0, 0, 0};
    }
};

//-----------------------------------------------------------------------------
// Test Network Parameters
//-----------------------------------------------------------------------------

class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

        // Set consensus parameters.
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.nPowTargetTimespan = 60;
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;

        // Block reward is 1 coin.
        const CAmount genesisReward = 1 * COIN;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // Unique testnet message start.
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        // RPC port for testnet.
        nDefaultPort = 14332;
        nPruneAfterHeight = 1000;

        // Create the testnet genesis block.
        genesis = CreateGenesisBlock(1742232311, 143674, 0x1e0ffff0, 1, genesisReward);
        consensus.hashGenesisBlock = genesis.GetHash();

        // Verify testnet genesis values.
        assert(genesis.hashMerkleRoot ==
               uint256S("0x0910b8c51236d0adb3444604cfcb019f657ea358ade9baeda804d54ca9582362"));
        assert(consensus.hashGenesisBlock ==
               uint256S("0x51f7e90f4db1b1518d4647ae767727124217a6cb416e66f374bd568284198a3a"));

        // Set default nodes.
        vSeeds.clear();
        vSeeds.emplace_back("seed1", "18.224.250.194", false);
        vSeeds.emplace_back("seed2", "18.189.127.19", false);
        vSeeds.emplace_back("seed3", "3.136.34.227", false);
        vSeeds.emplace_back("seed4", "3.146.90.153", false);
        vSeeds.emplace_back("seed5", "3.146.108.5", false);
        vFixedSeeds.clear();

        // Base58 prefixes.
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        checkpointData = (CCheckpointData){ { } };
        chainTxData = ChainTxData{0, 0, 0};
    }
};

//-----------------------------------------------------------------------------
// Regression Test Network Parameters
//-----------------------------------------------------------------------------

class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";

        // Set consensus parameters.
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.nPowTargetTimespan = 60;
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;

        // Block reward is 1 coin.
        const CAmount genesisReward = 1 * COIN;

        // For regtest, use an easier difficulty.
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // Unique regtest message start.
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        // RPC port for regtest.
        nDefaultPort = 14333;
        nPruneAfterHeight = 1000;

        // Create the regtest genesis block.
        genesis = CreateGenesisBlock(1742232311, 0, 0x207fffff, 1, genesisReward);
        consensus.hashGenesisBlock = genesis.GetHash();

        // Verify regtest genesis values.
        assert(genesis.hashMerkleRoot ==
               uint256S("0x0910b8c51236d0adb3444604cfcb019f657ea358ade9baeda804d54ca9582362"));
        assert(consensus.hashGenesisBlock ==
               uint256S("0x0e1e868e9635e703f6ab4ff09d26ba13715f93fe73312cff2f67bbf4a6ee52cb"));

        // For regtest, leave seed nodes empty.
        vSeeds.clear();
        vFixedSeeds.clear();

        // Base58 prefixes.
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        checkpointData = (CCheckpointData){ { } };
        chainTxData = ChainTxData{0, 0, 0};
    }

    // Method to update BIP9 parameters for regtest.
    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};

//-----------------------------------------------------------------------------
// Global Chain Parameter Handling
//-----------------------------------------------------------------------------

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

CChainParams &Params(const std::string &chain)
{
    SelectParams(chain);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string &chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string &network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

// Global function to update regtest BIP9 parameters.
void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    // We know that in regtest mode, globalChainParams is a CRegTestParams.
    static_cast<CRegTestParams *>(globalChainParams.get())->UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
