#include "m_groupbox.h"

/* include elements */
#include "../m_button/m_button.h"
#include "../m_checkbox/m_checkbox.h"
#include "../m_listbox/m_listbox.h"
#include "../m_textinput/m_textinput.h"
#include "../m_colorpicker/m_colorpicker.h"
#include "../m_combo/m_combo.h"
#include "../m_multi/m_multi.h"
#include "../m_slider/m_slider.h"

static int m_Slide[ Gui::m_Maximum ];
static bool m_SlideMore[ Gui::m_Maximum ];
static float m_SlideOffset[ Gui::m_Maximum ];

Groupbox::Groupbox( std::string m_Name, int m_GroupNum, float  X, float  Y, float  m_Width, float  m_Height, int m_CurrentTab, uint32_t m_Flags )
{
	this->m_Name = m_Name;
	this->m_GroupNum = m_GroupNum;
	this->m_Pos = { X, Y };
	this->m_Size = { m_Width, m_Height };
	this->m_Tab = m_CurrentTab;
	this->m_Flags = m_Flags;
}

bool Groupbox::GetVisible( )
{
	return m_Visible;
}

void Groupbox::SetVisible( bool m_Vis )
{
	m_Visible = m_Vis;
}

vec2_t Groupbox::GetPos( )
{
	return m_Pos;
}

void Groupbox::SetPos( vec2_t m_Position )
{
	m_Pos += m_Position;
}

void Groupbox::Run( )
{
	m_Elements.m_OffsetX = this->m_Pos.x + 10;
	m_Elements.m_OffsetY[ this->m_GroupNum ] = this->m_Pos.y + 32 + m_Slide[ this->m_GroupNum ];
	m_SlideOffset[ m_GroupNum ] = 0;

	Draw( );
}

void Groupbox::Draw( )
{
	static int m_Alpha[ Gui::m_Maximum ];
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ m_Size.x, m_Size.y + 20 }
	);

	if ( isHovered && !Gui::m_Control.color_tPickerOpened && !Gui::m_Control.m_Activity[ 0 ] && !Gui::m_Control.m_Activity[ 1 ]
		&& !Gui::m_Control.m_Activity[ 2 ] && !Gui::m_Control.m_Activity[ 3 ] && Gui::m_Control.IsPossible( ) )
		m_Alpha[ m_GroupNum ] += 5;
	else
		m_Alpha[ m_GroupNum ] -= 5;

	if ( m_Alpha[ m_GroupNum ] > 255 )
		m_Alpha[ m_GroupNum ] = 255;

	if ( m_Alpha[ m_GroupNum ] < 0 )
		m_Alpha[ m_GroupNum ] = 0;

	std::clamp<int>( m_Alpha[ m_GroupNum ], 0, 255 );

	if ( !( m_Flags & Flags::GROUP_NO_DRAW ) )
	{
		DrawList.GradientVertical( m_Pos.x, m_Pos.y, m_Size.x, 20, color_t( 35, 30, 35 ), color_t( 45, 45, 60 ) );
		DrawList.GradientVertical( m_Pos.x, m_Pos.y + 20, this->m_Size.x, 10, color_t( 100, 100, 100, 50 ), color_t( 45, 45, 45, 0 ) );
		DrawList.FilledRect( m_Pos.x, m_Pos.y + 20, this->m_Size.x, this->m_Size.y, color_t( 25, 20, 25 ) );
		DrawList.FilledRect( m_Pos.x, m_Pos.y + 20, this->m_Size.x, 1, color_t( 130, 125, 150 ) );
		DrawList.Rect( m_Pos.x, m_Pos.y, this->m_Size.x, this->m_Size.y + 20, color_t( 140, 135, 140, m_Alpha[ m_GroupNum ] ) );
	}

	if ( !( m_Flags & Flags::GROUP_NO_DRAW_TEXT ) ) {
		DrawList.DrawString( { m_Pos.x + 10, m_Pos.y + 3 }, this->m_Name, color_t( 255, 255, 255, 255 ), Fonts::Main, font_flags::drop_shadow );
	}
}

void Groupbox::SetLimit( )
{
	DrawList.SetViewport
	(
		{ ( DWORD )m_Pos.x, ( DWORD )m_Pos.y + 27,
		( DWORD )m_Size.x, ( DWORD )m_Size.y - 15, 0.f, 1.f }
	);
}

bool Groupbox::IsHovered( )
{
	return Gui::m_Input.MousePointer(
		{ m_Pos.x, m_Pos.y + 20 },
		{ m_Size.x, m_Size.y - 15 }
	);
}

void Groupbox::AddElement( Checkbox* m_Check )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Check->m_Pos = { m_Elements.m_OffsetX, m_Elements.m_OffsetY[ m_GroupNum ] };

		if ( IsHovered( ) )
			m_Check->Update( );

		m_Check->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 30;
		m_SlideOffset[ m_GroupNum ] += 30;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Check;
}

void Groupbox::AddElement( Slider* m_Slider )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Slider->m_Pos = { m_Elements.m_OffsetX, m_Elements.m_OffsetY[ m_GroupNum ] };

		if ( IsHovered( ) ) {
			m_Slider->Update( );
			m_Slider->m_AllowDraw = true;
		}
		else
			m_Slider->m_AllowDraw = false;

		m_Slider->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 40;
		m_SlideOffset[ m_GroupNum ] += 40;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Slider;
}

void Groupbox::AddElement( Combo* m_Combo )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );

		m_Combo->m_Pos = { m_Elements.m_OffsetX, m_Elements.m_OffsetY[ m_GroupNum ] };

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::COMBO ][ Gui::m_Control.GetIndex( ) ] ) {
			if ( IsHovered( ) )
				m_Combo->Update( );
		}
		else if ( Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::COMBO ][ Gui::m_Control.GetIndex( ) ] )
			m_Combo->Update( );

		SetLimit( );
		m_Combo->DrawBox( );
		DrawList.SetViewport( m_Old );

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::COMBO ][ Gui::m_Control.GetIndex( ) ] )
			SetLimit( );

		m_Combo->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 50;
		m_SlideOffset[ m_GroupNum ] += 50;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Combo;
}

void Groupbox::AddElement( Multi* m_Multi )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );

		m_Multi->m_Pos = { m_Elements.m_OffsetX, m_Elements.m_OffsetY[ m_GroupNum ] };

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::MULTICOMBO ][ Gui::m_Control.GetIndex( ) ] ) {
			if ( IsHovered( ) )
				m_Multi->Update( );
		}
		else if ( Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::MULTICOMBO ][ Gui::m_Control.GetIndex( ) ] )
			m_Multi->Update( );

		SetLimit( );
		m_Multi->DrawBox( );
		DrawList.SetViewport( m_Old );

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::MULTICOMBO ][ Gui::m_Control.GetIndex( ) ] )
			SetLimit( );

		m_Multi->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 50;
		m_SlideOffset[ m_GroupNum ] += 50;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Multi;
}

void Groupbox::AddElement( ColorPicker* m_Color )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Color->m_Pos = { m_Elements.m_OffsetX + 160, m_Elements.m_OffsetY[ m_GroupNum ] - 26 };

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::color_tSELECTOR ][ Gui::m_Control.GetIndex( ) ] ) {
			if ( IsHovered( ) )
				m_Color->Update( );
		}
		else
			m_Color->Update( );

		m_Color->Draw( );

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Color;
}

/*
void Groupbox::AddElement( Bind* m_Bind )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Bind->m_Pos = { m_Elements.m_OffsetX + 177, m_Elements.m_OffsetY[ m_GroupNum ] - 26 };

		if ( IsHovered( ) )
			m_Bind->Update( );

		m_Bind->Draw( );

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::BIND ][ Gui::m_Control.GetIndex( ) ] ) {
			if ( IsHovered( ) )
				m_Bind->UpdateList( );
		}
		else
			m_Bind->UpdateList( );

		m_Bind->InitList( );

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Bind;
}
*/

void Groupbox::AddElement( TextInput* m_Input )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Input->m_Pos = { m_Elements.m_OffsetX, m_Elements.m_OffsetY[ m_GroupNum ] };

		if ( !Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::TEXTBOX ][ Gui::m_Control.GetIndex( ) ] ) {
			if ( IsHovered( ) )
				m_Input->Update( );
		}
		else
			m_Input->Update( );

		m_Input->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 40;
		m_SlideOffset[ m_GroupNum ] += 40;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Input;
}

void Groupbox::AddElement( Button* m_Button )
{
	if ( GetVisible( ) )
	{
		D3DVIEWPORT9 m_Old = DrawList.GetViewport( );
		SetLimit( );

		m_Button->m_Pos = { m_Elements.m_OffsetX,
			m_Elements.m_OffsetY[ m_GroupNum ]
		};

		if ( IsHovered( ) )
			m_Button->Update( );

		m_Button->Draw( );

		m_Elements.m_OffsetY[ m_GroupNum ] += 40;
		m_SlideOffset[ m_GroupNum ] += 40;

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );

		DrawList.SetViewport( m_Old );
	}

	delete m_Button;
}

void Groupbox::AddElement( ListBox* m_List )
{
	if ( GetVisible( ) )
	{
		m_List->m_Pos = { m_Elements.m_OffsetX + 2, m_Elements.m_OffsetY[ m_GroupNum ] };

		m_List->Update( );
		m_List->Draw( );

		Gui::m_Control.SetIndex( Gui::m_Control.GetIndex( ) + 1 );
	}

	delete m_List;
}

void Groupbox::AddEmpty( int m_EmptyPlace, int m_Slide )
{
	m_Elements.m_OffsetY[ m_GroupNum ] += m_EmptyPlace;
	m_SlideOffset[ m_GroupNum ] += m_Slide;
}

void Groupbox::InitScroll( )
{
	if ( !GetVisible( ) )
		return;

	if ( !( m_SlideOffset[ m_GroupNum ] > m_Size.y - 24 ) ) {
		m_Slide[ m_GroupNum ] = 0;
		return;
	}

	bool isColorPicker = Gui::m_Control.color_tPickerOpened;
	bool isHovered = Gui::m_Input.MousePointer
	(
		{ m_Pos.x, m_Pos.y },
		{ m_Size.x, m_Size.y + 20 }
	);

	auto ScrollProcess = [ this ]( )
	{
		if ( m_Slide[ m_GroupNum ] > 0 )
			m_Slide[ m_GroupNum ] = 0;

		if ( m_Slide[ m_GroupNum ] < ( m_Size.y - 19 ) - m_SlideOffset[ m_GroupNum ] )
			m_Slide[ m_GroupNum ] = ( m_Size.y - 19 ) - m_SlideOffset[ m_GroupNum ];
	};

	if ( !isColorPicker && Gui::m_Control.IsPossible( ) && isHovered && Gui::m_Input.GetMouseWheel( ) != 0 )
	{
		m_Slide[ m_GroupNum ] += Gui::m_Input.GetMouseWheel( ) * 6;
		Gui::m_Input.SetMouseWheel( 0 );
		ScrollProcess( );
	}

	int m_Max = m_SlideOffset[ m_GroupNum ] - m_Size.y + 29.f;
	float m_ScrollPos = ( ( float )m_Slide[ m_GroupNum ] / ( float )m_SlideOffset[ m_GroupNum ] ) * ( m_Size.y - 20.f ) * ( -1 );
	float m_ScrollPosMax = m_Max / ( float )m_SlideOffset[ m_GroupNum ] * ( m_Size.y - 20.f );

	static int m_Old[ Gui::m_Maximum ];
	if ( m_Old[ m_GroupNum ] != m_Max ) {
		ScrollProcess( );
		m_Old[ m_GroupNum ] = m_Max;
	}

	DrawList.FilledRect( m_Pos.x + m_Size.x - 5, m_Pos.y + 21, 5, m_Size.y - 1, color_t( 45, 45, 50 ) );
	DrawList.FilledRect( m_Pos.x + m_Size.x - 4, m_Pos.y + 22, 3, 3, color_t( 100, 90, 130 ) );

	DrawList.FilledRect( m_Pos.x + m_Size.x - 4, m_Pos.y + m_Size.y + 16, 3, 3, color_t( 100, 90, 130 ) );
	DrawList.FilledRect( m_Pos.x + m_Size.x - 4, m_Pos.y + 26 + m_ScrollPos, 3, m_Size.y - m_ScrollPosMax - 3, color_t( 100, 90, 130 ) );
}