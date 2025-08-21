#pragma once
#include "UIView.hpp"

namespace vrl
{
	class Model;

	class Controller
	{
	public:
		explicit Controller(Model& model);
		void handleInput(const UIView::Interaction& interaction);
	private:
		void openFile();
		void openResource();
		Model& m_model;
	};
};
