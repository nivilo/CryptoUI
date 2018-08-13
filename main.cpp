#include "mainwindow.h"
#include "stream.h"
#include "connectwindow.h"
#include <QApplication>
#include <QObject>
#include <tr1/unordered_map>
#include "common_util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::tr1::unordered_map<std::string, int> pd;
    pd["XBT-USD"] = 0;
    pd["BTC-USDT"] = 0;
    pd["BTC-USD"] = 0;
    pd["USDT-BTC"] = 0;

    pd["ETH-USD"] = 1;
    pd["USD-ETH"] = 1;
    pd["ETH-USDT"] = 1;
    pd["USDT-ETH"] = 1;
    pd["ETH-USDT"] = 1;

    stream s;
    ConnectWindow cw;
    MainWindow w(&cw, pd);

    QObject::connect(&cw, SIGNAL(DisconnectAll()), &s, SLOT(OnDisconnectALLRequest()));
    QObject::connect(&cw, SIGNAL(MimasSocketConnect()), &s, SLOT(OnMimasConnectRequest()));
    QObject::connect(&cw, SIGNAL(LimitSocketConnect()), &s, SLOT(OnLimitConnectRequest()));
    QObject::connect(&cw, SIGNAL(SelfDefineSocketConnect(std::string)), &s, SLOT(OnSelfDefineConnectRequest(std::string)));

    QObject::connect(&s, SIGNAL(SocketDisconnectDone()), &w, SLOT(OnDisconnectDone()));
    QObject::connect(&s, SIGNAL(MimasConnectDone()), &w, SLOT(OnMimasConnectDone()));
    QObject::connect(&s, SIGNAL(LimitConnectDone()), &w, SLOT(OnLimitConnectDone()));
    QObject::connect(&s, SIGNAL(SelfDefineConnectDone()), &w, SLOT(OnSelfDefineConnectDone()));

    QObject::connect(&s, SIGNAL(SocketRecvStarted()), &w, SLOT(OnSocketRecvStarted()));
    QObject::connect(&w, SIGNAL(MainWindowDisconnect()), &s, SLOT(OnDisconnectALLRequest()));

    QObject::connect(&w, SIGNAL(StartClicked()), &s, SLOT(Start()));
    QObject::connect(&s, SIGNAL(DataUpdate(std::string)), &w, SLOT(marketUpdate(std::string)));

    w.show();

    return a.exec();
}
