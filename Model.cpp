#include "stdafx.h"
#include "Model.hpp"
#include "ResourceParser.hpp"

namespace vrl
{
	void Model::loadFile(const FilePath& path)
	{
		m_filePath = path;
		m_items.clear();

		// ファイルの読み込み
		const vrl::ResourceParser parser;
		m_items = parser.parse(path);
	}

	const Array<vrl::ResourceItem>& Model::getResourceItems() const
	{
		return m_items;
	}

	const Optional<FilePath>& Model::getFilePath() const
	{
		return m_filePath;
	}
}
