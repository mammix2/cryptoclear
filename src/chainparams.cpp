// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t genTime = 1507122108;

void MineGenesis(CBlock genesis){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
    	++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
	newhash = genesis.GetHash();
	if(newhash < besthash){
	    besthash=newhash;
	    printf("New best: %s\n", newhash.GetHex().c_str());
	}
    }
    printf("Found Genesis, Nonce: %ld, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x21;
        pchMessageStart[3] = 0x46;
        vAlertPubKey = ParseHex("045192f30c92a11684739b8aa934f64fdfc690b9ef93904f9326db72cbe12d76dc23402f0ab92939a0850e19dd851feb0c5648913087c8fab3616eace1b029e4b3");
        nDefaultPort = 8895;
        nRPCPort = 8896;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //

        const char* pszTimestamp = "BBC News: Theresa May battles a sore throat and prankster in conference speech.";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, genTime, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = genTime;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 4048505;

        hashGenesisBlock = genesis.GetHash();
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        assert(hashGenesisBlock == uint256("0x00000aad5c5d97f37c02dc24f5fec9d2a45c5265f7d69b9f22d596f45a639d7f"));
        assert(genesis.hashMerkleRoot == uint256("0xc4db2593f7a96c3cba7ca5bc183599dbdce715221844c1cbc768e826321211b0"));

        vSeeds.push_back(CDNSSeedData("cclr001.ffptech.com",  "cclr001.ffptech.com"));
        vSeeds.push_back(CDNSSeedData("node01.cryptoclear.com",  "node01.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node02.cryptoclear.com",  "node02.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node03.cryptoclear.com",  "node03.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node04.cryptoclear.com",  "node04.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node05.cryptoclear.com",  "node05.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node06.cryptoclear.com",  "node06.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node07.cryptoclear.com",  "node07.cryptoclear.com"));
        vSeeds.push_back(CDNSSeedData("node08.cryptoclear.com",  "node08.cryptoclear.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 28);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 132);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 171);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 100000000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xb7;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xd5;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("0495c366b3dc2986b8669858743cecdcb4f0749c6c23cc1e94d77a1a5b289a2f81f3613273a2aa8c8b2ebd19caab8785492ba98c33d9336472f627f5963b65e14d");
        nDefaultPort = 18895;
        nRPCPort = 18896;
        strDataDir = "testnet";
        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 994399;
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000002f1191e8389e02391041f8977e321de038a449fcc3d6493482db320fff"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 97);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x41;
        pchMessageStart[1] = 0x32;
        pchMessageStart[2] = 0x23;
        pchMessageStart[3] = 0x14;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = genTime;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 2133278;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 28895;
        strDataDir = "regtest";
//        MineGenesis(genesis); // mine the genesis block only, disable after solved
        assert(hashGenesisBlock == uint256("0x000002f4561c03258b561e9b19d7a6eca8496208804065b873da9f156e809523"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
