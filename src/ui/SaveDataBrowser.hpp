#pragma once

#include <string>
#include <vector>
#include "save_load.hpp"
#include "Screen.hpp"

namespace lif {

class SaveDataBrowser {
	long idnum = 0;
	lif::SaveData loadedData;

protected:
	struct SaveFile {
		std::string displayName;
		std::string path;
		int level = 0;
		int nPlayers = 0;
	};

	SaveDataBrowser();

	lif::ui::Screen::Callback _createLoadCallback(const std::string& file);
	lif::ui::Screen::Callback _createDeleteCallback(const std::string& file) const;
	std::string _newUniqueId();

public:
	std::vector<SaveFile> browseSaveData(std::string saveDataPath) const;
	const lif::SaveData& getLoadedData() const { return loadedData; }
};

}
