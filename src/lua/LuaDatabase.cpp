//
// Created by Fredy on 28/10/2021.
//

#include "LuaDatabase.h"

MYSQLOO_LUA_FUNCTION(connect) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    if (database->m_tableReference == 0) {
        LUA->Push(-1);
        database->m_tableReference = LUA->ReferenceCreate();
    }
    database->m_database->connect();
    return 0;
}

MYSQLOO_LUA_FUNCTION(escape) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    unsigned int nQueryLength;
    const char *sQuery = LUA->GetString(2, &nQueryLength);
    auto escaped = database->m_database->escape(std::string(sQuery, nQueryLength));
    LUA->PushString(escaped.c_str(), escaped.size());
    return 1;
}

MYSQLOO_LUA_FUNCTION(setCharacterSet) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->CheckType(2, GarrysMod::Lua::Type::String);
    const char *charset = LUA->GetString(2);
    bool success = database->m_database->setCharacterSet(charset);
    LUA->PushBool(success);
    LUA->PushString("");
    return 2;
}

MYSQLOO_LUA_FUNCTION(setSSLSettings) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    SSLSettings sslSettings;
    if (LUA->IsType(2, GarrysMod::Lua::Type::String)) {
        sslSettings.key = LUA->GetString(2);
    }
    if (LUA->IsType(3, GarrysMod::Lua::Type::String)) {
        sslSettings.cert = LUA->GetString(3);
    }
    if (LUA->IsType(4, GarrysMod::Lua::Type::String)) {
        sslSettings.ca = LUA->GetString(4);
    }
    if (LUA->IsType(5, GarrysMod::Lua::Type::String)) {
        sslSettings.capath = LUA->GetString(5);
    }
    if (LUA->IsType(6, GarrysMod::Lua::Type::String)) {
        sslSettings.cipher = LUA->GetString(6);
    }
    database->m_database->setSSLSettings(sslSettings);
    return 0;
}

MYSQLOO_LUA_FUNCTION(disconnect) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    bool wait = false;
    if (LUA->IsType(2, GarrysMod::Lua::Type::Bool)) {
        wait = LUA->GetBool(2);
    }
    database->m_database->disconnect(wait);
    return 0;
}

MYSQLOO_LUA_FUNCTION(status) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushNumber(database->m_database->m_status);
    return 1;
}

MYSQLOO_LUA_FUNCTION(serverVersion) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushNumber(database->m_database->serverVersion());
    return 1;
}

MYSQLOO_LUA_FUNCTION(serverInfo) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushString(database->m_database->serverInfo().c_str());
    return 1;
}

MYSQLOO_LUA_FUNCTION(hostInfo) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushString(database->m_database->hostInfo().c_str());
    return 1;
}

MYSQLOO_LUA_FUNCTION(setAutoReconnect) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->CheckType(2, GarrysMod::Lua::Type::Bool);
    database->m_database->setAutoReconnect(LUA->GetBool(2));
    return 0;
}

MYSQLOO_LUA_FUNCTION(setMultiStatements) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->CheckType(2, GarrysMod::Lua::Type::Bool);
    database->m_database->setMultiStatements(LUA->GetBool(2));
    return 0;
}

MYSQLOO_LUA_FUNCTION(setCachePreparedStatements) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->CheckType(2, GarrysMod::Lua::Type::Bool);
    database->m_database->setCachePreparedStatements(LUA->GetBool(2));
    return 0;
}

MYSQLOO_LUA_FUNCTION(abortAllQueries) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    auto abortedQueries = database->m_database->abortAllQueries();
    for (auto pair: abortedQueries) {
        //TODO:
        //query->onQueryDataFinished(LUA, data);
    }
    LUA->PushNumber((double) abortedQueries.size());
    return 1;
}

MYSQLOO_LUA_FUNCTION(queueSize) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushNumber((double) database->m_database->queueSize());
    return 1;
}

MYSQLOO_LUA_FUNCTION(ping) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    LUA->PushBool(database->m_database->ping());
    return 1;
}

MYSQLOO_LUA_FUNCTION(wait) {
    auto database = getLuaObject<LuaDatabase>(LUA, LuaObject::TYPE_DATABASE);
    database->m_database->wait();
    return 0;
}

void LuaDatabase::createMetaTable(ILuaBase *LUA) {
    LuaObject::TYPE_DATABASE = LUA->CreateMetaTable("MySQLOO Database");
    LuaObject::addMetaTableFunctions(LUA);

    LUA->PushCFunction(connect);
    LUA->SetField(-2, "connect");

    LUA->PushCFunction(escape);
    LUA->SetField(-2, "escape");

    LUA->PushCFunction(setCharacterSet);
    LUA->SetField(-2, "setCharacterSet");

    LUA->PushCFunction(setSSLSettings);
    LUA->SetField(-2, "setSSLSettings");

    LUA->PushCFunction(disconnect);
    LUA->SetField(-2, "disconnect");

    LUA->PushCFunction(status);
    LUA->SetField(-2, "status");

    LUA->PushCFunction(serverVersion);
    LUA->SetField(-2, "serverVersion");

    LUA->PushCFunction(serverInfo);
    LUA->SetField(-2, "serverInfo");

    LUA->PushCFunction(hostInfo);
    LUA->SetField(-2, "hostInfo");

    LUA->PushCFunction(setAutoReconnect);
    LUA->SetField(-2, "setAutoReconnect");

    LUA->PushCFunction(setMultiStatements);
    LUA->SetField(-2, "setMultiStatements");

    LUA->PushCFunction(setCachePreparedStatements);
    LUA->SetField(-2, "setCachePreparedStatements");

    LUA->PushCFunction(abortAllQueries);
    LUA->SetField(-2, "abortAllQueries");

    LUA->PushCFunction(queueSize);
    LUA->SetField(-2, "queueSize");

    LUA->PushCFunction(ping);
    LUA->SetField(-2, "ping");

    LUA->PushCFunction(wait);
    LUA->SetField(-2, "wait");
}

void LuaDatabase::think(ILuaBase *lua) {
    LuaObject::think(lua);
}