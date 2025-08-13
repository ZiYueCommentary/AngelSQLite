// Copyright 2025 ZiYueCommentary
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sqlite3.h"
#include <unordered_map>
#include <string>

#define EXPORT(x) extern "C" __declspec(dllexport) x __stdcall

class SQLiteDataReader {
public:
    sqlite3_stmt* stmt = nullptr;
    std::unordered_map<std::string, int> columns;

    SQLiteDataReader(sqlite3_stmt* stmt) : stmt(stmt) {
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; ++i) {
            columns[sqlite3_column_name(stmt, i)] = i;
        }
    }
};

EXPORT(bool) SQLiteDataReader_Read(SQLiteDataReader* reader) {
    int rc = sqlite3_step(reader->stmt);
    return rc == SQLITE_ROW;
}

EXPORT(const char*) SQLiteDataReader_GetName(SQLiteDataReader* reader, int column) {
    return sqlite3_column_name(reader->stmt, column);
}

EXPORT(const char*) SQLiteDataReader_GetText(SQLiteDataReader* reader, int column) {
    return reinterpret_cast<const char*>(sqlite3_column_text(reader->stmt, column));
}

EXPORT(float) SQLiteDataReader_GetFloat(SQLiteDataReader* reader, int column) {
    return static_cast<float>(sqlite3_column_double(reader->stmt, column));
}

EXPORT(int) SQLiteDataReader_GetInt(SQLiteDataReader* reader, int column) {
    return sqlite3_column_int(reader->stmt, column);
}

EXPORT(bool) SQLiteDataReader_IsDBNull(SQLiteDataReader* reader, int column) {
    return sqlite3_column_type(reader->stmt, column) == SQLITE_NULL;
}

EXPORT(const char*) SQLiteDataReader_GetTextByName(SQLiteDataReader* reader, const char* name) {
    return reinterpret_cast<const char*>(sqlite3_column_text(reader->stmt, reader->columns[name]));
}

EXPORT(float) SQLiteDataReader_GetFloatByName(SQLiteDataReader* reader, const char* name) {
    return static_cast<float>(sqlite3_column_double(reader->stmt, reader->columns[name]));
}

EXPORT(int) SQLiteDataReader_GetIntByName(SQLiteDataReader* reader, const char* name) {
    return sqlite3_column_int(reader->stmt, reader->columns[name]);
}

EXPORT(bool) SQLiteDataReader_IsDBNullByName(SQLiteDataReader* reader, const char* name) {
    return sqlite3_column_type(reader->stmt, reader->columns[name]) == SQLITE_NULL;
}

EXPORT(void) SQLiteDataReader_Close(SQLiteDataReader* reader) {
    sqlite3_reset(reader->stmt);
    delete reader;
}

class SQLiteConnection {
public:
    sqlite3* db = nullptr;
    bool isOpen = false;

    SQLiteConnection(const char* databasePath) {
        isOpen = sqlite3_open(databasePath, &db) == SQLITE_OK;
    }
};

EXPORT(bool) SQLiteConnection_IsOpen(SQLiteConnection* conn) {
    return conn->isOpen;
}

EXPORT(void) SQLiteConnection_Close(SQLiteConnection* conn) {
    if (conn->isOpen) sqlite3_close(conn->db);
    conn->db = nullptr;
    conn->isOpen = false;
    delete conn;
}

class SQLiteCommand {
public:
    SQLiteConnection* conn = nullptr;
    sqlite3_stmt* stmt = nullptr;

    SQLiteCommand(const char* statement, SQLiteConnection* conn) : conn(conn) {
        if (sqlite3_prepare_v2(conn->db, statement, -1, &stmt, nullptr) != SQLITE_OK) {
            stmt = nullptr;
        }
    }
};

EXPORT(void) SQLiteCommand_BindParameterIntByName(SQLiteCommand* cmd, const char* name, int value) {
    sqlite3_bind_int(cmd->stmt, sqlite3_bind_parameter_index(cmd->stmt, name), value);
}

EXPORT(void) SQLiteCommand_BindParameterInt(SQLiteCommand* cmd, int index, int value) {
    sqlite3_bind_int(cmd->stmt, index, value);
}

EXPORT(void) SQLiteCommand_BindParameterFloatByName(SQLiteCommand* cmd, const char* name, float value) {
    sqlite3_bind_double(cmd->stmt, sqlite3_bind_parameter_index(cmd->stmt, name), value);
}

EXPORT(void) SQLiteCommand_BindParameterFloat(SQLiteCommand* cmd, int index, float value) {
    sqlite3_bind_double(cmd->stmt, index, value);
}

EXPORT(void) SQLiteCommand_BindParameterTextByName(SQLiteCommand* cmd, const char* name, const char* value) {
    sqlite3_bind_text(cmd->stmt, sqlite3_bind_parameter_index(cmd->stmt, name), value, -1, SQLITE_TRANSIENT);
}

EXPORT(void) SQLiteCommand_BindParameterText(SQLiteCommand* cmd, int index, const char* value) {
    sqlite3_bind_text(cmd->stmt, index, value, -1, SQLITE_TRANSIENT);
}

EXPORT(void) SQLiteCommand_BindParameterNullByName(SQLiteCommand* cmd, const char* name) {
    sqlite3_bind_null(cmd->stmt, sqlite3_bind_parameter_index(cmd->stmt, name));
}

EXPORT(void) SQLiteCommand_BindParameterNull(SQLiteCommand* cmd, int index) {
    sqlite3_bind_null(cmd->stmt, index);
}

EXPORT(int) SQLiteCommand_ExecuteNonQuery(SQLiteCommand* cmd) {
    if (!cmd->conn || !SQLiteConnection_IsOpen(cmd->conn) || !cmd->stmt) return -1;

    int rc = sqlite3_step(cmd->stmt);
    int row = sqlite3_changes(cmd->conn->db);
    sqlite3_reset(cmd->stmt);
    if (rc != SQLITE_DONE) return -1;

    return row;
}

EXPORT(SQLiteDataReader*) SQLiteCommand_ExecuteReader(SQLiteCommand* cmd) {
    return new SQLiteDataReader(cmd->stmt);
}

EXPORT(void) SQLiteCommand_Finalize(SQLiteCommand* cmd) {
    sqlite3_finalize(cmd->stmt);
    delete cmd;
}

EXPORT(SQLiteConnection*) ConnectSQLite(const char* databasePath) {
    return new SQLiteConnection(databasePath);
}

EXPORT(SQLiteCommand*) CreateSQLiteCommand(const char* statement, SQLiteConnection* conn) {
    return new SQLiteCommand(statement, conn);
}