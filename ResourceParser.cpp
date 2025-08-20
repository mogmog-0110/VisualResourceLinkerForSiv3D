# include "stdafx.h"
# include "ResourceParser.hpp"

namespace vrl
{
	Array<ResourceItem> ResourceParser::parse(const FilePath& filePath) const
	{
		Array<ResourceItem> items;
		TextReader reader{ filePath };

		if (not reader)
		{
			return items; // ファイルが開けなかった場合は空の配列を返す
		}


		// 初期化
		String line;
		int32 lineNumber = 1;
		ResourceCategory currentCategory = ResourceCategory::Required;
		bool inExampleBlock = false;

		while (reader.readLine(line))
		{
			// 各行の先頭と末尾の空白を取り除く
			const String trimmedLine = line.trimmed();

			// /*で始まり*/で終わる行は読み飛ばす
			if (inExampleBlock)
			{
				if (trimmedLine.starts_with(U"*/"))
				{
					inExampleBlock = false; // サンプルブロックの終了
				}
				++lineNumber; // サンプルブロック内は行番号を進める
				continue; // サンプルブロック内は無視
			}

			// カテゴリーを判定
			if (trimmedLine.contains(U"Siv3D Engine Optional Resources"))
			{
				currentCategory = ResourceCategory::Optional; // オプションリソースの開始
			}
			else if (trimmedLine.contains(U"Siv3D App Resources"))
			{
				currentCategory = ResourceCategory::Custom; // 必須リソースの開始
			}
			else if (trimmedLine.starts_with(U"/* examples"))
			{
				inExampleBlock = true; // サンプルブロックの開始
			}
			else if (trimmedLine.starts_with(U"*/"))
			{
				inExampleBlock = false; // サンプルブロックの終了
			}

			// リソース解析
			bool isCommentedOut = trimmedLine.starts_with(U"//");
			String targetLine = isCommentedOut ? trimmedLine.substr(2).trimmed() : trimmedLine;

			if (targetLine.starts_with(U"Resource("))
			{
				const size_t startPos = targetLine.indexOf(U'(');
				const size_t endPos = targetLine.lastIndexOf(U')');

				// String::npos -> 探している文字が見つからない 
				if (startPos != String::npos && endPos != String::npos && startPos < endPos)
				{
					const FilePath path = targetLine.substr(startPos + 1, endPos - startPos - 1);

					items.push_back(ResourceItem{
							.path = path,
							.category = currentCategory,
							.isEnabled = (not isCommentedOut),
							.lineNumber = lineNumber

					});
				}
			}
			++lineNumber;
		}
		return items;
	}

}

