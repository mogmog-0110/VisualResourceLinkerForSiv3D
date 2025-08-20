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

		// メインメニューバーの表示
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Resource"))
				{
					interaction.openFileClicked = true; // Open Resourceボタンがクリック

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// メインウィンドウの描画
		if (ImGui::Begin("Resource List"))
		{
			if (ImGui::BeginTabBar("CategoryTabs"))
			{
				if (ImGui::BeginTabItem("Required Resource"))
				{
					// TODO: 必須リソースの描画
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Optional Resource"))
				{
					// TODO: オプションリソースの描画
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Custom Resource"))
				{
					// TODO: カスタムリソースの描画
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		return interaction;
	}
};
