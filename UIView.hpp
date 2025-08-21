# pragma once
# include "Model.hpp"

namespace vrl
{
	class Model;

	class UIView
	{
	public:
		// ユーザー操作の報告書的なもの
		struct Interaction
		{
			bool openFileClicked = false;
			bool saveFileClicked = false;
			bool registButtonClicked = false;
			bool revertClicked = false;
			bool exitClicked = false;

			Optional<size_t> toggledItemIndex;
			Optional<size_t> erasedItemIndex;
		};

		explicit UIView(Model& model);
		[[nodiscard]]
		Interaction draw();
	private:
		[[nodiscard]]
		Interaction drawMainMenuBar();

		[[nodiscard]]
		Interaction drawMainWindow();

		[[nodiscard]]
		Interaction drawResourceTable(ResourceCategory category);

		Model& m_model;
	};
};
