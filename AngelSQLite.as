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

const int STDCALL = 1;

int library = LoadLibrary("AngelSQLite.dll");

void OnInitialize() {
    print("AngelSQLite plugin v0.0.1 - under development!");

    RegisterLibraryObject("SQLiteConnection");
    RegisterLibraryObject("SQLiteCommand");
    RegisterLibraryObject("SQLiteDataReader");

    // SQLiteConnection
    RegisterLibraryMethod("SQLiteConnection", "bool IsOpen()", GetProcAddress(library, "_SQLiteConnection_IsOpen@4"), STDCALL);
    RegisterLibraryMethod("SQLiteConnection", "void Close()", GetProcAddress(library, "_SQLiteConnection_Close@4"), STDCALL);
    
    // SQLiteCommand
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(int, int)", GetProcAddress(library, "_SQLiteCommand_BindParameterInt@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(int, float)", GetProcAddress(library, "_SQLiteCommand_BindParameterFloat@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(int, const char)", GetProcAddress(library, "_SQLiteCommand_BindParameterText@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(const char, int)", GetProcAddress(library, "_SQLiteCommand_BindParameterIntByName@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(const char, float)", GetProcAddress(library, "_SQLiteCommand_BindParameterFloatByName@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameter(const char, const char)", GetProcAddress(library, "_SQLiteCommand_BindParameterTextByName@12"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameterNull(int)", GetProcAddress(library, "_SQLiteCommand_BindParameterNull@8"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void BindParameterNull(const char)", GetProcAddress(library, "_SQLiteCommand_BindParameterNullByName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "int ExecuteNonQuery()", GetProcAddress(library, "_SQLiteCommand_ExecuteNonQuery@4"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "SQLiteDataReader ExecuteReader()", GetProcAddress(library, "_SQLiteCommand_ExecuteReader@4"), STDCALL);
    RegisterLibraryMethod("SQLiteCommand", "void Finalize()", GetProcAddress(library, "_SQLiteCommand_Finalize@4"), STDCALL);

    // SQLiteDataReader
    RegisterLibraryMethod("SQLiteDataReader", "bool Read()", GetProcAddress(library, "_SQLiteDataReader_Read@4"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "const char GetName(int)", GetProcAddress(library, "_SQLiteDataReader_GetName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "int GetInt(int)", GetProcAddress(library, "_SQLiteDataReader_GetInt@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "float GetFloat(int)", GetProcAddress(library, "_SQLiteDataReader_GetFloat@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "const char GetText(int)", GetProcAddress(library, "_SQLiteDataReader_GetText@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "bool IsDBNull(int)", GetProcAddress(library, "_SQLiteDataReader_IsDBNull@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "int GetInt(const char)", GetProcAddress(library, "_SQLiteDataReader_GetIntByName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "float GetFloat(const char)", GetProcAddress(library, "_SQLiteDataReader_GetFloatByName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "const char GetText(const char)", GetProcAddress(library, "_SQLiteDataReader_GetTextByName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "bool IsDBNull(const char)", GetProcAddress(library, "_SQLiteDataReader_IsDBNullByName@8"), STDCALL);
    RegisterLibraryMethod("SQLiteDataReader", "void Close()", GetProcAddress(library, "_SQLiteDataReader_Close@4"), STDCALL);

    // Global functions
    RegisterLibraryFunction("SQLiteConnection ConnectSQLite(const char)", GetProcAddress(library, "_ConnectSQLite@4"), STDCALL);
    RegisterLibraryFunction("SQLiteCommand CreateSQLiteCommand(const char, SQLiteConnection)", GetProcAddress(library, "_CreateSQLiteCommand@8"), STDCALL);
}