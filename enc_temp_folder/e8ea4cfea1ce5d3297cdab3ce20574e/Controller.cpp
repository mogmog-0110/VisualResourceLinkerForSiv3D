# include "stdafx.h"
# include "Controller.hpp"
# include "Model.hpp"

namespace vrl
{

	Controller::Controller(Model& model)
		: m_model{ model }
	{
	}

	void Controller::handleInput(const UIView::Interaction& interaction)
	{
		if (interaction.openFileClicked)
		{
			openFile();
		}

	}

	void Controller::openFile()
	{
		// ここで
		const Array<FileFilter> filters =
		{
			{ U"リソースファイル (*.rc)", { U"rc" } }, // リソースのみを選べるようにフィルター
			FileFilter::AllFiles()
		};

		// 作成したフィルターを使って、ダイアログを開く
		if (const auto path = Dialog::OpenFile(filters))
		{
			m_model.loadFile(*path);
		}
	}
};
