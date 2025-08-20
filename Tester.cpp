#include "stdafx.h"
#include "Tester.hpp"
#include "Model.hpp"

namespace vrl
{
	String ToString(const ResourceCategory& category)
	{
		switch (category)
		{
		case ResourceCategory::Required: return U"Required";
		case ResourceCategory::Optional: return U"Optional";
		case ResourceCategory::Custom: return U"Custom";
		default: return U"";
		}
	}

	void Tester::ShowModelItems(const Model& model)
	{
		for (const auto& item : model.getResourceItems())
		{
			Logger << U"--------------------";
			Logger << U"  - Category: " << ToString(item.category);
			Logger << U"  - Path    : " << item.path;
			Logger << U"  - Enabled : " << item.isEnabled;
			Logger << U"  - LineNum : " << item.lineNumber;
		}
		Logger << U"--------------------";
	}
};
