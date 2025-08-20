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
		// ここで例外がスローされるが、操作はできるので一旦無視
		// 0x00007FF9082E7F7A で例外がスロー wil::ResultException (メモリの場所 0x000000477CDFE290)。

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
