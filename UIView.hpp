#pragma once

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
		};

		explicit UIView(Model& model);
		[[nodiscard]]
		Interaction draw();
	private:
		Model& m_model;
	};
};
