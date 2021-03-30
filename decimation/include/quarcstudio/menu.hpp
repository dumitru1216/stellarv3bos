/////////////////////////////////////////////////////
//                                                   
//    Generated with QuarcStudio. (ex. ImGui Editor) 
//    QuarcStudio is designed with love by pers0na2. 
//    All rights to QuarcStudio belong to pers0na2.  
//                                                   
/////////////////////////////////////////////////////


#ifndef QUARCMENU
#define QUARCMENU

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_USE using namespace ImGui; 
#include  "../imgui/imgui_internal.h"

#include "checkbox.hpp"
#include "slider.hpp"
#include "combo.hpp"
#include "tab.hpp"
#include "subtab.hpp"
#include "button.hpp"
#include "childs.hpp"

namespace QGUI 
{

	void init_styles(ImGuiStyle& style)
	{
 		style.WindowPadding = ImVec2(0,0);
 		style.FramePadding = ImVec2(4,3);
 		style.ItemSpacing = ImVec2(8,4);
 		style.ItemInnerSpacing = ImVec2(4,4);
 		style.IndentSpacing = 21;
 		style.ScrollbarSize = 1;
 		style.GrabMinSize = 1;
 		style.WindowBorderSize = 0;
 		style.ChildBorderSize = 1;
 		style.PopupBorderSize = 1;
 		style.FrameBorderSize = 1;
 		style.WindowRounding = 0;
 		style.ChildRounding = 0;
 		style.FrameRounding = 0;
 		style.PopupRounding = 0;
 		style.ScrollbarRounding = 0;
 		style.GrabRounding = 0;
 		style.SelectableTextAlign = ImVec2(0.5f,0.5f);

 		style.Colors[ImGuiCol_Text] = ImVec4(0.900000f,0.900000f,0.900000f,1.000000f);
 		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.600000f,0.600000f,0.600000f,1.000000f);
 		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.044118f,0.044117f,0.044117f,1.000000f);
 		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.000000f,0.000000f,0.000000f,0.000000f);
 		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.110000f,0.110000f,0.140000f,0.920000f);
 		style.Colors[ImGuiCol_Border] = ImVec4(0.500000f,0.500000f,0.500000f,0.500000f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.000000f,0.000000f,0.000000f,0.000000f);
 		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.430000f,0.430000f,0.430000f,0.390000f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.470000f,0.470000f,0.690000f,0.400000f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.420000f,0.410000f,0.640000f,0.690000f);
 		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.270000f,0.270000f,0.540000f,0.830000f);
 		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.320000f,0.320000f,0.630000f,0.870000f);
 		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.400000f,0.400000f,0.800000f,0.200000f);
 		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.400000f,0.400000f,0.550000f,0.800000f);
 		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.200000f,0.250000f,0.300000f,0.600000f);
 		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.400000f,0.400000f,0.800000f,0.300000f);
 		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.400000f,0.400000f,0.800000f,0.400000f);
 		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.410000f,0.390000f,0.800000f,0.600000f);
 		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.900000f,0.900000f,0.900000f,0.500000f);
 		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.000000f,1.000000f,1.000000f,0.300000f);
 		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.410000f,0.390000f,0.800000f,0.600000f);
 		style.Colors[ImGuiCol_Button] = ImVec4(0.350000f,0.400000f,0.610000f,0.620000f);
 		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.400000f,0.480000f,0.710000f,0.790000f);
 		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.460000f,0.540000f,0.800000f,1.000000f);
 		style.Colors[ImGuiCol_Header] = ImVec4(0.400000f,0.400000f,0.900000f,0.450000f);
 		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.450000f,0.450000f,0.900000f,0.800000f);
 		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.530000f,0.530000f,0.870000f,0.800000f);
 		style.Colors[ImGuiCol_Separator] = ImVec4(0.500000f,0.500000f,0.500000f,0.600000f);
 		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.600000f,0.600000f,0.700000f,1.000000f);
 		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.700000f,0.700000f,0.900000f,1.000000f);
 		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.000000f,1.000000f,1.000000f,0.160000f);
 		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.780000f,0.820000f,1.000000f,0.600000f);
 		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.780000f,0.820000f,1.000000f,0.900000f);
 		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.000000f,0.000000f,1.000000f,0.350000f);
 		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.000000f,1.000000f,0.000000f,0.900000f);
	}

	void render_menu()
	{
	
	}

}

#endif