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
		// ImGuiの入出力情報にアクセス
		ImGuiIO& io = ImGui::GetIO();

		// Ctrlキーが押されている状態で
		if (io.KeyCtrl)
		{
			// Oキー
			if (ImGui::IsKeyPressed(ImGuiKey_O))
			{
				openFile();
			}
			// Sキー
			if (ImGui::IsKeyPressed(ImGuiKey_S))
			{
				m_model.saveFile();
			}
			// Rキー
			if (ImGui::IsKeyPressed(ImGuiKey_R))
			{
				m_model.revertChanges();
			}
		}

		if (interaction.exitClicked)
		{
			exitRequested = true;
		}
		if (interaction.openFileClicked)
		{
			openFile();
		}

		if (interaction.saveFileClicked)
		{
			m_model.saveFile();
		}

		if (interaction.revertClicked)
		{
			m_model.revertChanges();
		}

		if (interaction.registButtonClicked)
		{
			if (not m_model.getFilePath())
			{
				System::MessageBoxOK(U"Error", U"Please open a Resource.rc file before registering new resources.", MessageBoxStyle::Error);
				return;
			}
			openResource();
		}

		if (interaction.toggledItemIndex)
		{
			m_model.toggleItemEnabled(*interaction.toggledItemIndex);
		}

		if (interaction.erasedItemIndex)
		{
			m_model.removeItem(*interaction.erasedItemIndex);
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

	void Controller::openResource()
	{
		// 複数ファイルを選択できるダイアログを開く
		if (const auto paths = Dialog::OpenFiles({ FileFilter::AllFiles() }))
		{
			// 選ばれたすべてのファイルについて、Modelに登録を依頼します
			for (const auto& path : paths)
			{
				m_model.addItem(path);
			}
		}
	}
};
