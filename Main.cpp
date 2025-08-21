# include "stdafx.h"
# include "Model.hpp"
# include "UIView.hpp"
# include "Controller.hpp"

# ifdef _DEBUG
# include "Tester.hpp"
# endif

void Main()
{
	Addon::Register<DearImGuiAddon>(U"ImGui");

	// 見た目の設定(何がいいのだろうか)
	// モダンなダークテーマ
	//ImGui::StyleColorsDark();

	// 明るいライトテーマ
	ImGui::StyleColorsLight();

	vrl::Model model;
	vrl::UIView view{ model };
	vrl::Controller controller{ model };

	System::SetTerminationTriggers(UserAction::CloseButtonClicked); // こちらの操作で終了できるように

#ifdef _DEBUG
	//model.loadFile(U"Resource.rc");
	//vrl::Tester::ShowModelItems(model);
#endif
	while (System::Update())
	{
		if (controller.exitRequested || KeyEscape.down())
		{
			controller.exitRequested = false;

			const String title = U"Confirm Exit";
			const String message = U"Are you sure you want to exit?";

			// 確認メッセージボックスを表示
			if (System::MessageBoxYesNo(title, message) == MessageBoxResult::Yes)
			{
				System::Exit();
			}
		}

		const auto interaction = view.draw();
		controller.handleInput(interaction);

		// ドラッグ&ドロップ処理
		if (DragDrop::HasNewFilePaths())
		{
			for (const auto& droppedFile : DragDrop::GetDroppedFilePaths())
			{
				model.addItem(droppedFile.path);
			}
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
