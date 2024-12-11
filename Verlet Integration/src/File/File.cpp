#include <fstream>
#include <sstream>
#include "File.h"
#include "Core/Log.h"

namespace Verlet
{
	namespace FileLoader
	{
		std::string GetFilePath(std::string filePath)
		{
			std::string toReplace = "res:";

			std::size_t pos = filePath.find(toReplace);
			if (pos == std::string::npos) return filePath;
			filePath.replace(pos, toReplace.length(), CONF_RESOURCE_LOCATION);

			return filePath;
		}

		// Calls GetFilePath
		File GetFile(std::string filePath)
		{
			// Convert res: to resource location
			std::string FilePath = Verlet::FileLoader::GetFilePath(filePath);

			std::ifstream stream(FilePath);
			std::stringstream strStream;
			strStream << stream.rdbuf();
			std::string fileContents = strStream.str();
			stream.close();

			if (fileContents.empty())
			{
				VT_FATAL("Failed to load file from filepath (or the file is empty): \"{0}\"", FilePath.c_str());
				exit(-1);
			}

			File file;
			file.FilePath = FilePath;
			file.FileContents = fileContents;

			return file;
		}
	}
}