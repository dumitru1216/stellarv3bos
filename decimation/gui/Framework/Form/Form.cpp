	#include "../gui.h"
#include "../form/form.h"
#include "../backdrop.hpp"

#pragma warning(disable : 4018)

std::map<int, float> HoverAnimation;
std::map<int, float> HoverTimer;

std::map<int, float> SelectAnimation;
std::map<int, float> SelectTimer;

bool TitleBarHovered;

bool SHOULD_DRAW;
std::string FormName;

bool GUI::Form::BeginWindow( std::string name ) {

	if( InputHelper::Pressed( VK_INSERT ) ) {
		if( ctx->animation <= 1.0f ) {
			if( ctx->FocusedID != 0 )
				ctx->FocusedID = 0;

			if( ctx->DropdownInfo.HashedID != 0 )
				ctx->DropdownInfo.HashedID = ctx->DropdownInfo.Open = 0;

			if( ctx->ColorPickerInfo.HashedID != 0 )
				ctx->ColorPickerInfo.HashedID = 0;

			if( ctx->MultiDropdownInfo.HashedID != 0 )
				ctx->MultiDropdownInfo.HashedID = ctx->MultiDropdownInfo.Open = 0;

			if( ctx->DropdownInfo.ArrowAnimation.size( ) )
				ctx->DropdownInfo.ArrowAnimation.clear( );

			if( ctx->DropdownInfo.ArrowTimer.size( ) )
				ctx->DropdownInfo.ArrowTimer.clear( );

			if( ctx->MultiDropdownInfo._ArrowAnimation.size( ) )
				ctx->MultiDropdownInfo._ArrowAnimation.clear( );

			if( ctx->MultiDropdownInfo._ArrowTimer.size( ) )
				ctx->MultiDropdownInfo._ArrowTimer.clear( );

			if( ctx->dragging )
				ctx->dragging = false;

			if( ctx->typing )
				ctx->typing = false;
		}

		ctx->open = !ctx->open;
	}

	// i'll work on this later, one thing that's a todo is
	// to fix the abrupt and sudden animation change when spamming insert
	if( ctx->open ) {
		ctx->timer += ( 1.0f / 0.3f ) * g_csgo.m_globals->m_frametime;
		ctx->timer = std::clamp<float>( ctx->timer, 0.f, 1.f );

		ctx->animation = Easing::OutQuint( ctx->timer );

		if( ctx->animation >= 0.999998 ) {
			ctx->animation = 1.f;
		}
	}
	else {
		ctx->timer -= ( 1.0f / 0.3f ) * g_csgo.m_globals->m_frametime;
		ctx->timer = std::clamp<float>( ctx->timer, 0.f, 1.f );

		ctx->animation = Easing::InQuint( ctx->timer );

		if( ctx->animation <= 0.001f ) {
			ctx->animation = 0.f;
		}
	}

	if( ctx->animation <= 0.0f ) {
		return false;
	}

	TitleBarHovered = InputHelper::Hovered( ctx->pos, vec2_t( ctx->size.x, 30 ) );
	if( ctx->FocusedID == 0 ) {
		if( !ctx->dragging && InputHelper::Pressed( VK_LBUTTON ) && TitleBarHovered )
			ctx->dragging = true;
		else if( ctx->dragging && InputHelper::Down( VK_LBUTTON ) )
			ctx->pos -= g_input.m_delta;
		else if( ctx->dragging && !InputHelper::Down( VK_LBUTTON ) )
			ctx->dragging = false;
	}

	if (ctx->open) {
		D3D::RectFilled(vec2_t(0, 0), vec2_t(4000, 4000), Color(0, 0, 0).OverrideAlpha(150 * ctx->animation, true));
	}

	if( ctx->animation >= 0.6969f ) {
		D3D::RectFilled( ctx->pos - 2, ctx->size + 4, g_cfg[XOR("menu_color")].get_color().OverrideAlpha( 255 * ctx->animation ) );

	}

	// form (fuck the transparency bro)
	D3D::RectFilled( ctx->pos, ctx->size, Color::Palette_t::MenuColors_t::Form( ).OverrideAlpha( 255 * ctx->animation, true ) );

	// swap cursor pos stack
	std::stack< vec2_t >( ).swap( ctx->CursorPosStack );

	// at what (x, y) relative to the menu's initial cursor position (0,0) we should draw the menu elements
	GUI::PushCursorPos( vec2_t( 16, 46 ) );

	return true;
}




bool show_select_tabsss2;
float blur = 0.f;
float transparnety = 0.f;
bool open = false;
int weignt_tab = 0;
bool should_render_text = false;
float bar_alpha = 0.f;


void GUI::Form::EndWindow( ) {


	
	SHOULD_DRAW = !show_select_tabsss2;




		if (ctx->DropdownInfo.HashedID != 0) {
			vec2_t DrawPos = ctx->DropdownInfo.Pos;
			vec2_t DrawSize = vec2_t(ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 23);

			D3D::RectFilled(DrawPos, vec2_t(DrawSize.x, (20 * ctx->DropdownInfo.Elements.size()) * ctx->DropdownInfo.Animation) - 1, Color(25, 25, 25));

			D3D::RectFilled(DrawPos, vec2_t(DrawSize.x, (20 * ctx->DropdownInfo.Elements.size()) * ctx->DropdownInfo.Animation), Color(25, 25, 25));

			D3D::SetLimit(std::pair<vec2_t, vec2_t>(DrawPos, vec2_t(ctx->DropdownInfo.Size, (20 * ctx->DropdownInfo.Elements.size()) * ctx->DropdownInfo.Animation) - 1));
			for (int i = 0; i < ctx->DropdownInfo.Elements.size(); i++) {
				vec2_t ElementPos = DrawPos + vec2_t(0, 20 * i);
				vec2_t ElementSize = vec2_t(ctx->DropdownInfo.Size, 20);

				if (InputHelper::Hovered(ElementPos, ElementSize)) {
					if (InputHelper::Pressed(VK_LBUTTON)) {
						ctx->DropdownInfo.OutAnimation = true;
						g_cfg[ctx->DropdownInfo.Option].set<int>(i);
					}

					D3D::RectFilled(ElementPos, ElementSize - vec2_t(1, 0), Color::Palette_t::MenuColors_t::ContainerHovered());
				}


				D3D::Text(ElementPos + vec2_t(4, 2), ctx->DropdownInfo.Elements[i].data(),
					g_cfg[ctx->DropdownInfo.Option].get<int>() != i ? Color(255, 255, 255) : g_cfg[XOR("menu_color")].get_color(), D3D::D3D9Fonts.at(D3D::EFonts::MAIN_FONT), D3D::EFontFlags::NONE);

			}
			D3D::ResetLimit();

			if (InputHelper::Pressed(VK_LBUTTON)) {
				ctx->DropdownInfo.OutAnimation = true;
			}
		}

		if (ctx->MultiDropdownInfo.HashedID != 0) {
			vec2_t DrawPos = ctx->MultiDropdownInfo.Pos;
			vec2_t DrawSize = vec2_t(ctx->ParentSize.x - 40 > 300 ? 300 : ctx->ParentSize.x - 40, 23);
			D3D::RectFilled(DrawPos, vec2_t(DrawSize.x, (20 * ctx->MultiDropdownInfo.Elements.size()) * ctx->MultiDropdownInfo.Animation) - 1,
				Color(25, 25, 25));

			//Outlines    g_cfg[XOR("menu_color")].get_color()
			D3D::RectFilled(DrawPos, vec2_t(ctx->MultiDropdownInfo.Size, (20 * ctx->MultiDropdownInfo.Elements.size()) * ctx->MultiDropdownInfo.Animation),
				Color(25,25,25));

			D3D::SetLimit(std::pair<vec2_t, vec2_t>(DrawPos, vec2_t(ctx->MultiDropdownInfo.Size, (20 * ctx->MultiDropdownInfo.Elements.size()) * ctx->MultiDropdownInfo.Animation) - 1));
			for (int i = 0; i < ctx->MultiDropdownInfo.Elements.size(); i++) {
				vec2_t ElementPos = DrawPos + vec2_t(0, 20 * i);
				vec2_t ElementSize = vec2_t(ctx->MultiDropdownInfo.Size, 20);

				int val = pow(2, (i + 2));

				if (InputHelper::Hovered(ElementPos, ElementSize)) {
					if (InputHelper::Pressed(VK_LBUTTON)) {
						bool v1 = g_cfg[ctx->MultiDropdownInfo.Elements.at(i).value].get<bool>();

						g_cfg[ctx->MultiDropdownInfo.Elements.at(i).value].set<bool>(!v1);
					}

					D3D::RectFilled(ElementPos, ElementSize - vec2_t(1, 0), Color::Palette_t::MenuColors_t::ContainerHovered());
				}


				D3D::Text(ElementPos + vec2_t(4, 2), ctx->MultiDropdownInfo.Elements.at(i).name,
					!g_cfg[ctx->MultiDropdownInfo.Elements.at(i).value].get<bool>() ? Color(255, 255, 255) : g_cfg[XOR("menu_color")].get_color(), D3D::D3D9Fonts.at(D3D::EFonts::MAIN_FONT), D3D::EFontFlags::NONE);

			}
			D3D::ResetLimit();

			if (InputHelper::Pressed(VK_LBUTTON)
				&& !InputHelper::Hovered(DrawPos,
					vec2_t(ctx->MultiDropdownInfo.Size, (20 * ctx->MultiDropdownInfo.Elements.size()) * ctx->MultiDropdownInfo.Animation))) {

				ctx->MultiDropdownInfo.OutAnimation = true;
			}

		}

		static float ColorPickerHue = -1.f;
		static std::map<size_t, float> ColorPickerAlpha;
		if (ctx->ColorPickerInfo.HashedID != 0) {
			if (ColorPickerAlpha.find(ctx->ColorPickerInfo.HashedID) == ColorPickerAlpha.end()) {
				ColorPickerAlpha.insert({ ctx->ColorPickerInfo.HashedID, 0.f });
			}

			vec2_t DrawPos = ctx->ColorPickerInfo.Pos;

			Color::Hsv_t v1 = Color::RGBtoHSV(g_cfg[ctx->ColorPickerInfo.Value].get_color());

			float NewHue;

			if (ColorPickerHue == -1.f) {
				NewHue = v1.Hue;
				ColorPickerHue = NewHue;
			}
			else {
				NewHue = ColorPickerHue;
			}

			float NewSaturation = v1.Saturation;
			float NewValue = v1.Value;
			ColorPickerAlpha.at(ctx->ColorPickerInfo.HashedID) = (g_cfg[ctx->ColorPickerInfo.Value].get_color()).a();

			float MaxHeight = (ctx->ColorPickerInfo.Size - 25);
			vec2_t DrawSize = vec2_t(ctx->ColorPickerInfo.Size, MaxHeight);
			vec2_t AnimatedSize = vec2_t(ctx->ColorPickerInfo.Size * ctx->ColorPickerInfo.Animation, DrawSize.y * ctx->ColorPickerInfo.Animation);

			D3D::RectFilled(DrawPos, AnimatedSize - 1, Color::Palette_t::MenuColors_t::Container());
			D3D::Rect(DrawPos, AnimatedSize, g_cfg[XOR("menu_color")].get_color());

			D3D::SetLimit(std::pair<vec2_t, vec2_t>(DrawPos, AnimatedSize - 1));
			{
				vec2_t ColorDrawPos(DrawPos + 10);
				vec2_t ColorDrawSize(DrawSize - 40);

				vec2_t AlphaDrawPos(ColorDrawPos + vec2_t(0, ColorDrawSize.y + 4));
				vec2_t AlphaDrawSize = vec2_t(ColorDrawSize.x, 16);

				vec2_t HueDrawPos(ColorDrawPos + vec2_t(ColorDrawSize.x + 4, -1));
				vec2_t HueDrawSize = vec2_t(16, ColorDrawSize.y);

				static vec2_t ColorPreviewPos;

				bool HoveredMain = InputHelper::Hovered(DrawPos, DrawSize);
				bool HoveredPicker = InputHelper::Hovered(DrawPos, DrawSize - 30);
				bool HoveredAlpha = InputHelper::Hovered(AlphaDrawPos, AlphaDrawSize);
				bool HoveredHue = InputHelper::Hovered(HueDrawPos, HueDrawSize);

				// alpha bar
				if (!ctx->ColorPickerInfo.PickingAlpha && !ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingColor) {
					if (HoveredAlpha && InputHelper::Pressed(VK_LBUTTON)) {
						ctx->ColorPickerInfo.PickingAlpha = true;
					}
				}
				else if (ctx->ColorPickerInfo.PickingAlpha) {
					if (InputHelper::Down(VK_LBUTTON)) {
						float FinalValue = std::clamp<float>(GUI::MapNumber(std::clamp<float>(
							vec2_t(g_input.m_mouse - AlphaDrawPos).x, 0, AlphaDrawSize.x),
							0, AlphaDrawSize.x, 0, 255), 0, 255);

						ColorPickerAlpha.at(ctx->ColorPickerInfo.HashedID) = FinalValue;
					}
					else {
						ctx->ColorPickerInfo.PickingAlpha = false;
					}
				}

				D3D::RectFilled(AlphaDrawPos, AlphaDrawSize, (g_cfg[ctx->ColorPickerInfo.Value].get_color()));
				D3D::Rect(AlphaDrawPos, AlphaDrawSize, Color::Palette_t::MenuColors_t::Outlines());

				// hue bar
				if (!ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingAlpha && !ctx->ColorPickerInfo.PickingColor) {
					if (HoveredHue && InputHelper::Pressed(VK_LBUTTON)) {
						ctx->ColorPickerInfo.PickingHue = true;
					}
				}
				else if (ctx->ColorPickerInfo.PickingHue) {
					if (InputHelper::Down(VK_LBUTTON)) {
						NewHue = ((g_input.m_mouse.y - (HueDrawPos.y)) / HueDrawSize.y) * 360.f;

						NewHue = std::clamp<float>(NewHue, 0.f, 359.f);
						ColorPickerHue = NewHue;
					}
					else {
						ctx->ColorPickerInfo.PickingHue = false;
					}
				}

				for (int i = 0; i < HueDrawSize.y; i++) {
					float Hue = ((float)i / HueDrawSize.y) * 360.f;
					Color HueColor = Color::HSVtoRGB(Hue, 1, 1);

					D3D::Line(HueDrawPos + vec2_t(0, i + 1), HueDrawPos + vec2_t(HueDrawSize.x, i), HueColor);
				}
				D3D::Rect(HueDrawPos + vec2_t(0, 1), HueDrawSize + vec2_t(1, 0), Color::Palette_t::MenuColors_t::Outlines());

				// main picker
				if (!ctx->ColorPickerInfo.PickingColor && !ctx->ColorPickerInfo.PickingHue && !ctx->ColorPickerInfo.PickingAlpha) {
					if (HoveredPicker && InputHelper::Pressed(VK_LBUTTON)) {
						ctx->ColorPickerInfo.PickingColor = true;
					}
				}
				else if (ctx->ColorPickerInfo.PickingColor) {
					if (InputHelper::Down(VK_LBUTTON)) {
						vec2_t CursorDelta = g_input.m_mouse - DrawPos;

						float s = (float)CursorDelta.x / (DrawSize.x - 30);
						float v = 1.f - (float)CursorDelta.y / (DrawSize.y - 30);

						// xd
						ColorPreviewPos = g_input.m_mouse;

						NewSaturation = s;
						NewValue = v;
					}
					else {
						ctx->ColorPickerInfo.PickingColor = false;
					}
				}

				g_cfg[ctx->ColorPickerInfo.Value].set_color(Color::HSVtoRGB(NewHue, std::clamp<float>(NewSaturation, 0.0f, 1.0f), std::clamp<float>(NewValue, 0.0f, 1.0f))
					.OverrideAlpha(ColorPickerAlpha.at(ctx->ColorPickerInfo.HashedID)));

				D3D::RectFilledLinearGradient(ColorDrawPos, DrawSize - 40 - vec2_t(1, 0), Color::Palette_t::White(), Color::HSVtoRGB(NewHue, 1, 1), true);
				D3D::RectFilledLinearGradient(ColorDrawPos, DrawSize - 40 - vec2_t(0, 1), Color(0, 0, 0, 0), Color(0, 0, 0));
				D3D::Rect(ColorDrawPos, DrawSize - 40, Color::Palette_t::MenuColors_t::Outlines());

				// the rects that signify what each part of the colorpicker is at

				// alpha
				float AlphaAdditive = (AlphaDrawSize.x * (ColorPickerAlpha.at(ctx->ColorPickerInfo.HashedID) / 255.f));
				D3D::Rect(AlphaDrawPos + vec2_t((AlphaAdditive - 3) < 0 ? 0 : AlphaAdditive - 3, 0), vec2_t(3, AlphaDrawSize.y), Color::Palette_t::MenuColors_t::Outlines());

				// actual color
				D3D::SetLimit(std::pair<vec2_t, vec2_t>(ColorDrawPos, ColorDrawSize));
				{
					D3D::Rect(ColorPreviewPos, vec2_t(3, 3), Color::Palette_t::MenuColors_t::Outlines());
				}
				D3D::ResetLimit();

				// hue
				float HueAdditive = (HueDrawSize.y * (NewHue / 360.f));
				D3D::Rect(HueDrawPos + vec2_t(0, (HueAdditive - 3) < 0 ? 1 : HueAdditive - 2), vec2_t(HueDrawSize.x + 1, 3), Color::Palette_t::MenuColors_t::Outlines());

				if (InputHelper::Pressed(VK_LBUTTON) && !HoveredMain) {
					ctx->ColorPickerInfo.OutAnimation = true;
				}
			}
			D3D::ResetLimit();
		}
		else {
			ColorPickerHue = -1.f;
		}


	
		if (weignt_tab < 140)
		{
			should_render_text = false;
		}
		if (weignt_tab > 140)
		{
			should_render_text = true;
		}


	if (show_select_tabsss2)
	{
		weignt_tab = weignt_tab + 10;
		open = true;
		blur = blur + 0.5f;
		transparnety = transparnety + 0.1;
		if (blur > 10.f)
		{
			blur = 10.f;
		}
		if (transparnety > 1.f)
		{
			transparnety = 1.f;
		}
		if (weignt_tab > 170)
		{
			weignt_tab = 170;
		}

	}

	if (open)
	{
		D3D::SetBlur(blur);
		D3D::RectFilled(ctx->pos + vec2_t(20, 50), vec2_t(576, 330), Color(0, 0, 0).OverrideAlpha(190 * ctx->animation * transparnety, true));
		D3D::SetBlur(0.f);
	}

	if (!show_select_tabsss2)
	{

		weignt_tab = weignt_tab - 10;
		D3D::SetBlur(blur);
		blur = blur - 0.5f;
		transparnety = transparnety - 0.1;
		if (blur < 0.f)
		{
			open = false;
			blur = 0.f;
			
		}
		if (transparnety < 0.f)
		{
			transparnety = 0.f;
		}
		if (weignt_tab < 0)
		{
			weignt_tab = 0;
		}


	}
	if (open)
	{
		D3D::RectFilled(ctx->pos, vec2_t(weignt_tab, 400), Color::Palette_t::MenuColors_t::Container().OverrideAlpha(255 * ctx->animation * transparnety, true));
	}
	if (show_select_tabsss2)
	{





		


		if (should_render_text)
		{
			bar_alpha = bar_alpha + 0.1f;
			// tabs 
			if (!ctx->Tabs.empty()) {
				constexpr int MaxTabs{ 8 };

				std::array<vec2_t, MaxTabs> TabPos;
				std::array<vec2_t, MaxTabs> TabSize;


				for (int i = 0; i < ctx->Tabs.size(); ++i) {
					TabSize.at(i) = vec2_t(170, 35);
					TabPos.at(i) = (i == 0) ? ctx->pos + vec2_t(0, 5) : TabPos.at(i - 1) + vec2_t(0, TabSize.at(i - 1).y);

					if (InputHelper::Hovered(TabPos.at(i), TabSize.at(i)) && ctx->FocusedID == 0) {
						D3D::RectFilled(TabPos.at(i), TabSize.at(i), Color::Palette_t::MenuColors_t::ContainerHovered().OverrideAlpha(255 * ctx->animation, true));

						if (InputHelper::Pressed(VK_LBUTTON)) {
							ctx->FocusedID = GUI::Hash(std::string(XOR("SelectTab#")).append(std::to_string(i)));
							show_select_tabsss2 = false;

						}
					}
					else {
						D3D::RectFilled(TabPos.at(i), TabSize.at(i), Color::Palette_t::MenuColors_t::Container().OverrideAlpha(255 * ctx->animation, true));

					}//+ vec2_t(0, 100)

					if (ctx->FocusedID == GUI::Hash(std::string(XOR("SelectTab#")).append(std::to_string(i)))) {
						if (!InputHelper::Down(VK_LBUTTON)) {
							ctx->ActiveTab = i;
							ctx->FocusedID = 0;
						}
					}

					vec2_t TextSize(D3D::GetTextSize(ctx->Tabs.at(i).second, D3D::D3D9Fonts.at(D3D::EFonts::MAIN_FONT)));
					D3D::Text((TabPos.at(i) + (TabSize.at(i) / 2)) - (TextSize / 2), ctx->Tabs.at(i).second,
						(ctx->ActiveTab == i ? g_cfg[XOR("menu_color")].get_color() : Color(255, 255, 255)).OverrideAlpha(255 * ctx->animation, true), D3D::D3D9Fonts.at(D3D::EFonts::MAIN_FONT), D3D::EFontFlags::NONE);
				}

			}
		}




	}
	else
	{
		bar_alpha = 0;
	}



	if (!show_select_tabsss2) {
		D3D::RectFilled(ctx->pos + vec2_t(15, 15), vec2_t(15, 1), g_cfg[XOR("menu_color")].get_color().OverrideAlpha(255 * ctx->animation, true));

		D3D::RectFilled(ctx->pos + vec2_t(15, 20), vec2_t(15, 1), g_cfg[XOR("menu_color")].get_color().OverrideAlpha(255 * ctx->animation, true));

		D3D::RectFilled(ctx->pos + vec2_t(15, 25), vec2_t(15, 1), g_cfg[XOR("menu_color")].get_color().OverrideAlpha(255 * ctx->animation, true));
	}

	if (InputHelper::Hovered(ctx->pos, vec2_t(50, 50))) {

		if (InputHelper::Pressed(VK_LBUTTON)) {
			show_select_tabsss2 = !show_select_tabsss2;

		}

	}

	if (!ctx->SubTabs.empty()) {
		if (!ctx->SubTabs.at(ctx->ActiveTab).empty()) {
			ctx->SubTabs.at(ctx->ActiveTab).clear();
		}
	}

	if (!ctx->Tabs.empty()) {
		ctx->Tabs.clear();
	}
}

bool GUI::Form::BeginTab( std::string icon, std::string name ) {
	ctx->Tabs.push_back( std::make_pair( icon, name ) );

	for( int i = 0; i < ctx->Tabs.size( ); ++i ) {
		if( ctx->SubTabs.find( i ) == ctx->SubTabs.end( ) ) {
			ctx->SubTabs.insert( { i, { } } );
			ctx->ActiveSubTab.insert( { i, { } } );
		}
	}

	return ctx->ActiveTab == ctx->Tabs.size( ) - 1;
}

bool GUI::Form::BeginSubTab( std::string icon, std::string name ) {
	ctx->SubTabs.at( ctx->ActiveTab ).push_back( std::make_pair( icon, name ) );

	return ctx->ActiveSubTab.at( ctx->ActiveTab ) == ctx->SubTabs.at( ctx->ActiveTab ).size( ) - 1;
}