#include "SaveDataManager.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "platform/CCFileUtils.h"

using namespace rapidjson;
using namespace cocos2d;

static const std::string SAVE_PATH = FileUtils::getInstance()->getWritablePath() + "save_data.json";

void SaveDataManager::saveGame(int board[6][5], int currentPlayer) {
    Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // Save board
    rapidjson::Value boardArray(rapidjson::kArrayType);
    for (int i = 0; i < 6; ++i) {
        rapidjson::Value row(rapidjson::kArrayType);
        for (int j = 0; j < 5; ++j) {
            row.PushBack(board[i][j], allocator);
        }
        boardArray.PushBack(row, allocator);
    }

    doc.AddMember("board", boardArray, allocator);
    doc.AddMember("currentPlayer", currentPlayer, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), SAVE_PATH);
}

bool SaveDataManager::loadGame(int board[6][5], int& currentPlayer) {
    if (!FileUtils::getInstance()->isFileExist(SAVE_PATH)) return false;

    std::string jsonData = FileUtils::getInstance()->getStringFromFile(SAVE_PATH);
    Document doc;
    doc.Parse(jsonData.c_str());

    if (!doc.IsObject()) return false;

    // Load board
    const auto& boardArray = doc["board"];
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 5; ++j)
            board[i][j] = boardArray[i][j].GetInt();

    currentPlayer = doc["currentPlayer"].GetInt();

    return true;
}

void SaveDataManager::clearGame() {
    FileUtils::getInstance()->removeFile(SAVE_PATH);
}
