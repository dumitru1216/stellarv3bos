/////////////////////////////////////////////////////
//                                                   
//    Generated with QuarcStudio. (ex. ImGui Editor) 
//    QuarcStudio is designed with love by pers0na2. 
//    All rights to QuarcStudio belong to pers0na2.  
//                                                   
/////////////////////////////////////////////////////


#ifndef QUARCCHECKBOX
#define QUARCCHECKBOX

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_USE using namespace ImGui; 
#include  "../imgui/imgui_internal.h"

namespace QGUI 
{

	bool checkbox(const char* label, bool* v)
	{
		IM_USE;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const float square_sz = ImGui::GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect total_bb(pos, pos + ImVec2(square_sz + (style.ItemInnerSpacing.x + 0+4),0+label_size.y +style.FramePadding.y * 2));
		ItemSize(total_bb, style.FramePadding.y);
		ItemAdd(total_bb, id);

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		if (pressed)
			*v = !(*v);

			window->DrawList->AddRectFilled(ImVec2(total_bb.Min.x + 3, total_bb.Min.y + 4), ImVec2(total_bb.Min.x + 25, total_bb.Min.y + 16), ImColor(53,53,53,255),6, 15); 

		if (*v)
		{
			window->DrawList->AddCircleFilled(ImVec2(total_bb.Min.x + 18, total_bb.Min.y + 10),7, ImColor(255,179,0,255),32); 
		}
		else
		{
			window->DrawList->AddCircleFilled(ImVec2(total_bb.Min.x + 10, total_bb.Min.y + 10),7, ImColor(207,156,35,255),32); 
		}

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(229/ 255.f , 229/ 255.f ,229/ 255.f ,255/ 255.f ));
		ImGui::RenderText(ImVec2(total_bb.Min.x + style.ItemInnerSpacing.x +  25, total_bb.Min.y + style.FramePadding.y + 0), label);
		ImGui::PopStyleColor();
		return pressed;
	}

}

#endif