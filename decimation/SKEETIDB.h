#pragma once
#include "gui/Elements.h"
#include "include/imgui/imgui.h"

ImGuiStyle* style = &ImGui::GetStyle();

// =====================================
// - Custom controls
// =====================================

#define InsertSpacer(x1) style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0); ImGui::BeginChild(x1, ImVec2(210.f, 18.f), false); {} ImGui::EndChild(); style->Colors[ImGuiCol_ChildBg] = ImColor(49, 49, 49, 255);
#define InsertGroupboxSpacer(x1) style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0); ImGui::BeginChild(x1, ImVec2(210.f, 9.f), false); {} ImGui::EndChild(); style->Colors[ImGuiCol_ChildBg] = ImColor(49, 49, 49, 255);
#define InsertGroupboxTitle(x1) ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(11.f); ImGui::GroupBoxTitle(x1);
#define InsertGroupBoxLeft(x1,x2) ImGui::NewLine(); ImGui::SameLine(19.f); ImGui::BeginGroupBox(x1, ImVec2(258.f, x2), true);
#define InsertGroupBoxCentered(x1,x2) ImGui::NewLine(); ImGui::SameLine(19.f); ImGui::BeginGroupBox(x1, ImVec2(535.f, x2), true);
#define InsertGroupBoxRight(x1,x2) ImGui::NewLine(); ImGui::SameLine(10.f); ImGui::BeginGroupBox(x1, ImVec2(258.f, x2), true);
#define InsertEndGroupBoxLeft(x1,x2) ImGui::EndGroupBox(); ImGui::SameLine(19.f); ImGui::BeginGroupBoxScroll(x1, x2, ImVec2(258.f, 11.f), true); ImGui::EndGroupBoxScroll();
#define InsertEndGroupBoxCentered(x1,x2) ImGui::EndGroupBox(); ImGui::SameLine(19.f); ImGui::BeginGroupBoxScroll(x1, x2, ImVec2(526.f, 11.f), true); ImGui::EndGroupBoxScroll();
#define InsertEndGroupBoxRight(x1,x2) ImGui::EndGroupBox(); ImGui::SameLine(10.f); ImGui::BeginGroupBoxScroll(x1, x2, ImVec2(258.f, 11.f), true); ImGui::EndGroupBoxScroll();
#define InsertGroupBoxTop(x1,x2) ImGui::NewLine(); ImGui::SameLine(19.f); ImGui::BeginGroupBox(x1, x2, true);
#define InsertEndGroupBoxTop(x1,x2,x3) ImGui::EndGroupBox(); ImGui::SameLine(19.f); ImGui::BeginGroupBoxScroll(x1, x2, x3, true); ImGui::EndGroupBoxScroll();

// =====================================
// - Default controls
// =====================================

#define InsertCheckbox(x1,x2) ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(19.f); ImGui::Checkbox(x1, &x2);
#define InsertButton(x1) ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f); ImGui::PushItemWidth(158.f); ImGui::Button(x1);
#define InsertSlider(x1,x2,x3,x4,x5) ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f); ImGui::PushItemWidth(158.f); ImGui::SliderFloat(x1, &x2, x3, x4, x5); ImGui::PopItemWidth();
#define InsertSliderWithoutText(x1,x2,x3,x4,x5) ImGui::Spacing(); ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f); ImGui::PushItemWidth(159.f); ImGui::SliderFloat(x1, &x2, x3, x4, x5); ImGui::PopItemWidth();
#define InsertSliderInt(x1,x2,x3,x4,x5) ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f); ImGui::PushItemWidth(158.f); ImGui::SliderInt(x1, &x2, x3, x4, x5); ImGui::PopItemWidth();
#define InsertSliderIntWithoutText(x1,x2,x3,x4,x5) ImGui::Spacing(); ImGui::Spacing(); ImGui::NewLine(); ImGui::SameLine(42.f); ImGui::PushItemWidth(159.f); ImGui::SliderInt(x1, &x2, x3, x4, x5); ImGui::PopItemWidth();
#define InsertCombo(x1,x2,x3) ImGui::PushItemWidth(158.f); ImGui::SingleSelect(x1, &x2, x3); ImGui::PopItemWidth();
#define InsertComboWithoutText(x1,x2,x3) ImGui::PushItemWidth(158.f); ImGui::SingleSelect(x1, &x2, x3); ImGui::PopItemWidth();
#define InsertMultiCombo(x1,x2,x3,x4) ImGui::PushItemWidth(158.f); ImGui::MultiSelect(x1, x2, x3, x4); ImGui::PopItemWidth();
#define InsertMultiComboWithoutText(x1,x2,x3,x4) ImGui::PushItemWidth(158.f); ImGui::MultiSelect(x1, &x2, x3, x4); ImGui::PopItemWidth();
#define InsertKeyBind(x1,x2) ImGui::Keybind(x1, &x2);
#define InsertKeyBindStyle(x1,x2,x3) ImGui::Keybind(x1, &x2, &x3);