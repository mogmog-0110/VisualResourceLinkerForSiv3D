#pragma once
# include "Model.hpp"

namespace vrl
{
	class ResourceParser
	{
	public:
		[[nodiscard]]
		Array<ResourceItem> parse(const FilePath& filePath) const;
	private:
		String extractPath(const String& line) const;
	};
};
