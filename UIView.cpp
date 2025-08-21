#include "stdafx.h"
#include "UIView.hpp"
#include "Model.hpp"

namespace vrl
{
	UIView::UIView(Model& model)
		: m_model(model)
	{
	}

	UIView::Interaction UIView::draw()
	{
		Interaction interaction;

		// Modelにファイルパスが設定されているかをチェック
		const bool isFileLoaded = m_model.getFilePath().has_value();

		// メニューバーの描画
		const auto menuInteraction = drawMainMenuBar(isFileLoaded);
		interaction.exitClicked = menuInteraction.exitClicked;
		interaction.openFileClicked = menuInteraction.openFileClicked;
		interaction.saveFileClicked = menuInteraction.saveFileClicked;
		interaction.revertClicked = menuInteraction.revertClicked;

		// メインウィンドウの描画
		const auto windowInteraction = drawMainWindow(isFileLoaded);
		interaction.registButtonClicked = windowInteraction.registButtonClicked;
		interaction.toggledItemIndex = windowInteraction.toggledItemIndex;
		interaction.erasedItemIndex = windowInteraction.erasedItemIndex;

		return interaction;
	}

	UIView::Interaction UIView::drawMainMenuBar(bool isFileLoaded)
	{
		Interaction interaction;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit", "Esc"))
				{
					interaction.exitClicked = true; // Exitボタンがクリック
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					interaction.openFileClicked = true; // Openボタンがクリック

				}

				if (ImGui::MenuItem("Save", "Ctrl+S", false, isFileLoaded))
				{
					interaction.saveFileClicked = true; // Saveボタンがクリック
				}

				if (ImGui::MenuItem("Revert", "Ctrl+R", false, isFileLoaded))
				{
					interaction.revertClicked = true; // Revertボタンをクリック
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		return interaction;
	}

	UIView::Interaction UIView::drawMainWindow(bool isFileLoaded)
	{
		Interaction interaction;

		// ウィンドウの固定化
		const float menuBarHeight = ImGui::GetFrameHeight();
		ImGui::SetNextWindowPos(ImVec2{ 0, menuBarHeight });
		ImGui::SetNextWindowSize(ImVec2{ (float)Scene::Width(), (float)Scene::Height() - menuBarHeight });

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoCollapse;

		ImGui::Begin("Resource List", nullptr, windowFlags);
		{
			// リソース登録ボタン
			if (ImGui::Button("Regist New Resource"))
			{
				interaction.registButtonClicked = true;
			}
			ImGui::Separator();

			// カテゴリータブ
			if (ImGui::BeginTabBar("CategoryTabs"))
			{
				if (ImGui::BeginTabItem("Required"))
				{
					const auto tableInteraction = drawResourceTable(ResourceCategory::Required);
					if (tableInteraction.toggledItemIndex) interaction.toggledItemIndex = tableInteraction.toggledItemIndex;
					if (tableInteraction.erasedItemIndex) interaction.erasedItemIndex = tableInteraction.erasedItemIndex;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Optional"))
				{
					const auto tableInteraction = drawResourceTable(ResourceCategory::Optional);
					if (tableInteraction.toggledItemIndex) interaction.toggledItemIndex = tableInteraction.toggledItemIndex;
					if (tableInteraction.erasedItemIndex) interaction.erasedItemIndex = tableInteraction.erasedItemIndex;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Custom"))
				{
					const auto tableInteraction = drawResourceTable(ResourceCategory::Custom);
					if (tableInteraction.toggledItemIndex) interaction.toggledItemIndex = tableInteraction.toggledItemIndex;
					if (tableInteraction.erasedItemIndex) interaction.erasedItemIndex = tableInteraction.erasedItemIndex;
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
		return interaction;
	}

	UIView::Interaction UIView::drawResourceTable(ResourceCategory category)
	{
		Interaction interaction;
		if (ImGui::BeginTable("resourceTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{

			// 見出し
			ImGui::TableSetupColumn("Enabled", ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableSetupColumn("Path");
			ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableHeadersRow();

			//Modelからアイテムリストを取得して一行ずつ描画
			for (auto&& [i, item] : Indexed(m_model.getResourceItems()))
			{

				// カテゴリと一致するものだけ
				if (item.category != category)
				{
					continue;
				}

				// チェックボックス
				ImGui::TableNextColumn();

				if (item.category != ResourceCategory::Required)
				{
					bool checked = item.isEnabled;
					if (ImGui::Checkbox((U"##enabled"_s + ToString(i)).toUTF8().c_str(), &checked)) // ImGUIはUTF-8しか理解できないっぽい
					{
						interaction.toggledItemIndex = i;
					}
				}

				// パス
				ImGui::TableNextColumn();
				ImGui::Text(item.path.toUTF8().c_str());

				// 削除
				ImGui::TableNextColumn();
				if (item.category != ResourceCategory::Required)
				{
					if (ImGui::Button((U"Erase##"_s + ToString(i)).toUTF8().c_str()))
					{
						interaction.erasedItemIndex = i;
					}
				}
			}

		}
		ImGui::EndTable();
		return interaction;
	}
};
