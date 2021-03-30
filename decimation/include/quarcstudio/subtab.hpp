/////////////////////////////////////////////////////
//                                                   
//    Generated with QuarcStudio. (ex. ImGui Editor) 
//    QuarcStudio is designed with love by pers0na2. 
//    All rights to QuarcStudio belong to pers0na2.  
//                                                   
/////////////////////////////////////////////////////


#ifndef QUARCSUBTAB
#define QUARCSUBTAB

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_USE using namespace ImGui; 
#include  "../imgui/imgui_internal.h"
namespace QGUI 
{

	bool subtab(const char* label, bool selected)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize({165,25}, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		if (selected)
		{
			window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 10, bb.Min.y + 4), ImVec2(bb.Max.x + -5, bb.Max.y + -4), ImColor(56,55,53,255),0, 15); 
			window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 165, bb.Min.y + 4), ImVec2(bb.Max.x + -5, bb.Max.y + -4), ImColor(255,179,0,255),0, 15); 
		}


		if(selected)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255/ 255.f , 254/ 255.f ,254/ 255.f ,255/ 255.f ));
			ImGui::RenderText(ImVec2(bb.Min.x + (165 / 2 - label_size.x / 2), bb.Min.y + (25 / 2 - label_size.y / 2)), label);
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(213/ 255.f , 213/ 255.f ,213/ 255.f ,255/ 255.f ));
			ImGui::RenderText(ImVec2(bb.Min.x + (165 / 2 - label_size.x / 2), bb.Min.y + (25 / 2 - label_size.y / 2)), label);
			ImGui::PopStyleColor();
		}

		return pressed;
	}

}

#endif