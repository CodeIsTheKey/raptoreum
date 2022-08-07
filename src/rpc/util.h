// Copyright (c) 2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_RPC_UTIL_H
#define BITCOIN_RPC_UTIL_H

#include <pubkey.h>
#include <protocol.h>
#include <script/standard.h>
#include <univalue.h>
#include <utilstrencodings.h>

#include <string>
#include <vector>

class CKeyStore;
class CPubKey;
class CScript;
struct InitInterfaces;

extern InitInterfaces* g_rpc_interfaces;

CPubKey HexToPubKey(const std::string& hex_in);
CPubKey AddrToPubKey(CKeyStore* const keystore, const std::string& addr_in);
CScript CreateMultisigRedeemscript(const int required, const std::vector<CPubKey>& pubkeys);

UniValue DescribeAddress(const CTxDestination& dest);

/** Returns given services flags a list of humanly readable (known) network services */
UniValue GetServicesNames(ServiceFlags services);

#endif // BITCOIN_RPC_UTIL_H
