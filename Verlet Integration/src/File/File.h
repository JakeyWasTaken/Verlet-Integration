#pragma once
#include <iostream>
#include "Config.h"

namespace Verlet
{
	struct File
	{
		std::string FilePath;
		std::string FileContents;
	};

	namespace FileLoader
	{
		std::string GetFilePath(std::string filePath);

		// Calls GetFilePath
		File GetFile(std::string filePath);
	}
}

