// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2014-2020 The Dash Core developers
// Copyright (c) 2020-2022 The Raptoreum developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/raptoreum-config.h>
#endif

#include <chainparams.h>
#include <interfaces/node.h>
#include <qt/raptoreum.h>
#include <qt/test/apptests.h>
#include <qt/test/rpcnestedtests.h>
#include <util.h>
#include <qt/test/uritests.h>
#include <qt/test/compattests.h>
#include <qt/test/trafficgraphdatatests.h>

#if defined(ENABLE_WALLET)
#include <qt/test/wallettests.h>
#if defined(ENABLE_BIP70)
#include <qt/test/paymentservertests.h>
#endif
#endif

#include <QApplication>
#include <QObject>
#include <QTest>

#include <openssl/ssl.h>

#if defined(QT_STATICPLUGIN)
#include <QtPlugin>
#if defined(QT_QPA_PLATFORM_MINIMAL)
Q_IMPORT_PLUGIN(QMinimalIntegrationPlugin);
#endif
#if defined(QT_QPA_PLATFORM_XCB)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_WINDOWS)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_COCOA)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_ANDROID)
Q_IMPORT_PLUGIN(QAndroidPlatformIntegrationPlugin)
#endif
#endif

extern void noui_connect();

// This is all you need to run all the tests
int main(int argc, char *argv[])
{
    SetupEnvironment();
    SetupNetworking();
    SelectParams(CBaseChainParams::REGTEST);
    noui_connect();
    ClearDatadirCache();
    fs::path pathTemp = fs::temp_directory_path() / strprintf("test_raptoreum-qt_%lu_%i", (unsigned long)GetTime(), (int)GetRand(100000));
    fs::create_directories(pathTemp);
    gArgs.ForceSetArg("-datadir", pathTemp.string());
    auto node = interfaces::MakeNode();

    bool fInvalid = false;

    // Prefer the "minimal" platform for the test instead of the normal default
    // platform ("xcb", "windows", or "cocoa") so tests can't unintentionally
    // interfere with any background GUIs and don't require extra resources.
    #if defined(WIN32)
        _putenv_s("QT_QPA_PLATFORM", "minimal");
    #else
        setenv("QT_QPA_PLATFORM", "minimal", 0);
    #endif

    // Don't remove this, it's needed to access
    // QApplication:: and QCoreApplication:: in the tests
    BitcoinApplication app(*node);
    app.setApplicationName("RTM-Qt-test");

    AppTests app_tests(app);
    if (QTest::qExec(&app_tests) != 0) {
        fInvalid = true;
    }
    URITests test1;
    if (QTest::qExec(&test1) != 0) {
        fInvalid = true;
    }
    RPCNestedTests test2;
    if (QTest::qExec(&test2) != 0) {
        fInvalid = true;
    }
    CompatTests test3;
    if (QTest::qExec(&test3) != 0) {
        fInvalid = true;
    }
#if defined(ENABLE_WALLET)
    WalletTests test4;
    if (QTest::qExec(&test4) != 0) {
        fInvalid = true;
    }
#if defined(ENABLE_BIP70)
    PaymentServerTests test5;
    if (QTest::qExec(&test5) != 0) {
        fInvalid = true;
    }
#endif
#endif
    TrafficGraphDataTests test6;
    if (QTest::qExec(&test6) != 0) {
        fInvalid = true;
    }

    return fInvalid;
}
