#pragma once

namespace vrl
{
	// リソースの種類を定義
	enum class ResourceCategory
	{
		Required, // 必須リソース(これがないとSiv3Dが動かない)
		Optional, // オプションリソース(容量削減のために使わないなら消してもいい)
		Custom,  // カスタムリソース(用意したリソース)
	};

	// リソース一行一行が持つ情報を定義
	struct ResourceItem
	{
		FilePath path; // ファイルパス
		ResourceCategory category; // リソースの種類
		bool isEnabled; // 有効かどうか
		int32 lineNumber; // リソースが定義されている行番号
	};

	void WriteResourcesInCategory(TextWriter& writer, const Array<ResourceItem>& items, ResourceCategory category, const String& sectionComment);

	class Model
	{
	public:
		void loadFile(const FilePath& filePath);
		void saveFile() const;
		void addItem(const FilePath& path);
		void revertChanges();

		void toggleItemEnabled(size_t index);
		void removeItem(size_t index);

		// TODO: Undo,Redo機能。コマンドパターンを使わなきゃなのでちょっと面倒


		[[nodiscard]]
		const Array<ResourceItem>& getResourceItems() const;

		[[nodiscard]]
		const Optional<FilePath>& getFilePath() const;

	private:
		bool writeItemsToFile(const FilePath& filePath, const Array<ResourceItem>& items) const;


		Optional<FilePath> m_filePath; // ファイルパス
		Array<ResourceItem> m_items; // リソースアイテムの配列
		Array<ResourceItem> m_backupItems; // バックアップ
	};
}
