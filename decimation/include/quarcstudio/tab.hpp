/////////////////////////////////////////////////////
//                                                   
//    Generated with QuarcStudio. (ex. ImGui Editor) 
//    QuarcStudio is designed with love by pers0na2. 
//    All rights to QuarcStudio belong to pers0na2.  
//                                                   
/////////////////////////////////////////////////////


#ifndef QUARCTAB
#define QUARCTAB

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_USE using namespace ImGui; 
#include  "../imgui/imgui_internal.h"

namespace QGUI 
{

	bool tab(const char* label,const char* icon, bool selected)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize({10+label_size.x + 20,43+0}, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		if(selected)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255 / 255.f, 188 / 255.f, 12 / 255.f, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + 4 + (style.ItemInnerSpacing.x), bb.Min.y + style.FramePadding.y + 16), icon);
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255 / 255.f, 188 / 255.f, 12 / 255.f, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + 24 + (style.ItemInnerSpacing.x), bb.Min.y + style.FramePadding.y + 14), label); 
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f));
			ImGui::RenderText(ImVec2(bb.Min.x + 4 + (style.ItemInnerSpacing.x), bb.Min.y + style.FramePadding.y + 16), icon);
			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229/ 255.f , 229/ 255.f ,229/ 255.f ,255/ 255.f ));
			ImGui::RenderText(ImVec2(bb.Min.x + 24 + (style.ItemInnerSpacing.x), bb.Min.y + style.FramePadding.y + 14), label);
			ImGui::PopStyleColor();
		}
		return pressed;
	}

}

#endif