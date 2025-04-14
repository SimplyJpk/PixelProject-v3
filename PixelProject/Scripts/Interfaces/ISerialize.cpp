#include "ISerialize.h"

using namespace cereal;

bool ISerialize::StartSave(const std::string& path)
{
	const auto saveType = SaveType();
	const auto finalPath = get_final_path(path);

	std::ofstream os(finalPath);

	switch (saveType) {
	case SaveTypes::XML:
		Save(XMLOutputArchive(os));
		return true;
	case SaveTypes::Binary:
		Save(BinaryOutputArchive(os));
		return true;
	case SaveTypes::Json:
		Save(JSONOutputArchive(os));
		return true;
	}
	return false;
}

bool ISerialize::StartLoad(const bool on_fail_save, const std::string& path)
{
	const auto saveType = SaveType();
	const auto finalPath = get_final_path(path);

	std::ifstream fs(finalPath);
	if (fs.is_open()) {
		switch (saveType) {
		case SaveTypes::XML:
			Load(XMLInputArchive(fs));
			return true;
		case SaveTypes::Binary:
			Load(BinaryInputArchive(fs));
			return true;
		case SaveTypes::Json:
			Load(JSONInputArchive(fs));
			return true;
		}
	}
	else if (on_fail_save) {
		return StartSave(path);
	}
	return false;
}

std::string ISerialize::get_final_path(const std::string& path)
{
	std::string finalPath = path;
	if (finalPath.empty())
		finalPath = FilePath();

	std::string resultPath = PixelProject::IO::GetBasePath() + "data/" + finalPath + SAVE_TYPE_NAME[static_cast<int>(
		SaveType())];
	return resultPath;
};
