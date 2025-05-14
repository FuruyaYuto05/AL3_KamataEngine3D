#include "MapChipField.h"
#include <fstream>
#include <map>
#include <sstream>
using namespace KamataEngine;


namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::LoadMapChipCsv(const std::string& filePath) { 
    
	ResetMapChipData();
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	//マップチップcsv	
	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
		
		std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}


	}

}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex]; 
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); 
}
