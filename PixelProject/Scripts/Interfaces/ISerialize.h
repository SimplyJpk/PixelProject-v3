#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

#include "Utility/Console.h"
#include "Utility/FileIO.h"

enum class SaveTypes
{
	XML,
	Binary,
	Json
};

class ISerialize
{
 public:
	ISerialize() = default;
	virtual ~ISerialize() = default;

	static inline std::string SAVE_TYPE_NAME[] = { ".xml", "", ".json" };

	bool StartSave(const std::string& path = "");

	bool StartLoad(bool on_fail_save = false, const std::string& path = "");

	virtual std::string FilePath() = 0;
	virtual SaveTypes SaveType() = 0;

 protected:
	virtual void Save(cereal::JSONOutputArchive out_archive)
	{
		NotImplemented("[ISerialize::Save] JSON not implemented");
	}

	virtual void Save(cereal::BinaryOutputArchive out_archive)
	{
		NotImplemented("[ISerialize::Save] Binary not implemented");
	}

	virtual void Save(cereal::XMLOutputArchive out_archive)
	{
		NotImplemented("[ISerialize::Save] XML not implemented");
	}

	virtual void Load(cereal::JSONInputArchive in_archive)
	{
		NotImplemented("[ISerialize::Load] JSON not implemented");
	}

	virtual void Load(cereal::BinaryInputArchive in_archive)
	{
		NotImplemented("[ISerialize::Load] Binary not implemented");
	}

	virtual void Load(cereal::XMLInputArchive in_archive)
	{
		NotImplemented("[ISerialize::Load] XML not implemented");
	}

 private:
	std::string get_final_path(const std::string& path);

	static void NotImplemented(const std::string& message = "Not implemented")
	{
		Console::PrintError(message);
		// TODO : (James) 11/11/20 - Should throw exception here
	}
};
