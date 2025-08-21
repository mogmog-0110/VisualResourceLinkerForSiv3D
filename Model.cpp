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

		// 念のためバックアップ
		m_backupItems = m_items;
		const DateTime now = DateTime::Now();
		const String timestamp = now.format(U"yyyy-MM-dd_HH-mm-ss");
		const FilePath backupFilePath = path + U"." + timestamp + U".bak";
		writeItemsToFile(backupFilePath, m_backupItems);
	}

	void Model::saveFile() const
	{
		if (not m_filePath)
		{
			System::MessageBoxOK(U"Save Error", U"No file path has been specified for saving.", MessageBoxStyle::Warning);
			return;
		}

		if (writeItemsToFile(*m_filePath, m_items))
		{
			// 成功
			const String message = U"File saved successfully to:\n\n" + *m_filePath;
			System::MessageBoxOK(U"Save Successful", message, MessageBoxStyle::Info);
		}
		else
		{
			// 失敗
			const String message = U"Failed to save the file.\nPlease check if the file is read-only\nor currently in use by another program.";
			System::MessageBoxOK(U"Save Error", message, MessageBoxStyle::Error);
		}
	}

	void Model::revertChanges()
	{
		// 現在のリストを、バックアップした時の状態に戻す
		const String title = U"Confirm Revert";
		const String message = U"Are you sure you want to discard all current changes?\n\n"
			U"This action cannot be undone.";

		// Yes/Noのメッセージボックスを表示
		if (System::MessageBoxYesNo(title, message) == MessageBoxResult::Yes)
		{
			// YesならRevert
			m_items = m_backupItems;
		}
	}

	void Model::addItem(const FilePath& path)
	{
		// プロジェクト基準の相対パスに変換
		const FilePath relativePath = FileSystem::RelativePath(path);

		if (relativePath.isEmpty())
		{
			const String title = U"Invalid File Path";
			const String message = U"The provided file path is invalid or is located outside the project directory.\n\n"
				U"Please ensure the file is within the project folder or a subfolder.";

			System::MessageBoxOK(title, message, MessageBoxStyle::Warning);
			return; // パスの変換に失敗したか、そもそもパスが空か
		}

		// 重複チェック
		for (const auto& item : m_items)
		{
			if (item.path == relativePath)
			{
				return;
			}
		}

		//ResourceItemの作成
		m_items.push_back(ResourceItem{
				.path = relativePath,
				.category = ResourceCategory::Custom, // 新規追加は自動的にカスタム
				.isEnabled = true,
				.lineNumber = 999 // 新規追加の目印
			});
	}

	void Model::toggleItemEnabled(size_t index)
	{
		if (index < m_items.size())
		{
			m_items[index].isEnabled = (not m_items[index].isEnabled);
		}
	}

	void Model::removeItem(size_t index)
	{
		if (index < m_items.size())
		{
			m_items.remove_at(index);
		}
	}

	const Array<vrl::ResourceItem>& Model::getResourceItems() const
	{
		return m_items;
	}

	const Optional<FilePath>& Model::getFilePath() const
	{
		return m_filePath;
	}

	bool Model::writeItemsToFile(const FilePath& filePath, const Array<ResourceItem>& items) const
	{
		TextWriter writer{ filePath };
		if (not writer)
		{
			// ファイルが開けなかった場合は false
			return false;
		}

		writer.writeln(U"# include <Siv3D/Windows/Resource.hpp>");
		writer.writeln(U"");
		writer.writeln(U"DefineResource(100, ICON, icon.ico)");

		// カテゴリごとに書き出す
		WriteResourcesInCategory(writer, items, ResourceCategory::Required, U"Siv3D Engine Resources (DO NOT REMOVE)");
		WriteResourcesInCategory(writer, items, ResourceCategory::Optional, U"Siv3D Engine Optional Resources");
		WriteResourcesInCategory(writer, items, ResourceCategory::Custom, U"Siv3D App Resources");

		return true; // 保存に成功したら trueを返す
	}

	// リソースファイルをSiv3Dが正しく読み込めるようにするための補助関数
	void WriteResourcesInCategory(TextWriter& writer, const Array<ResourceItem>& items, ResourceCategory category, const String& sectionComment)
	{
		writer.writeln(U"");
		writer.writeln(U"//////////////////////////////////////////////////////");
		writer.writeln(U"//");
		writer.writeln(U"//\t" + sectionComment);
		writer.writeln(U"//");
		writer.writeln(U"//////////////////////////////////////////////////////");

		for (const auto& item : items)
		{
			if (item.category == category)
			{
				// Requiredリソースはユーザーが変更できないので、常に有効
				const bool isEnabled = (category == ResourceCategory::Required) ? true : item.isEnabled;
				const String prefix = (isEnabled ? U"" : U"//");
				const String line = prefix + U"Resource(" + item.path + U")";
				writer.writeln(line);
			}
		}
	}
}


