#pragma once
#include "../includes.h"
#include "new_framework/m_render/m_color.h"
#include "new_framework/m_render/m_render.h"

/* new menu related */
namespace Gui {

	// Max of elements.
	inline constexpr int m_Maximum = 1000;

	namespace External
	{
		struct EDrawRect
		{
			EDrawRect( float posX, float posY, float m_Width, float h, color_t m_color_t )
			{
				this->posX = posX;
				this->posY = posY;
				this->m_Width = m_Width;
				this->m_Height = h;
				this->m_color_t = m_color_t;
			}

			float posX;
			float posY;
			float m_Width;
			float m_Height;

			color_t m_color_t;
		};

		struct EDrawRect2
		{
			EDrawRect2( float posX, float posY, float m_Width, float h, Color m_color_t )
			{
				this->posX = posX;
				this->posY = posY;
				this->m_Width = m_Width;
				this->m_Height = h;
				this->m_color_t = m_color_t;
			}

			float posX;
			float posY;
			float m_Width;
			float m_Height;

			Color m_color_t;
		};


		struct EDrawText
		{
			EDrawText( float posX, float posY, std::string m_Text, color_t m_color_t )
			{
				this->posX = posX;
				this->posY = posY;
				this->m_Text = m_Text;
				this->m_color_t = m_color_t;
			}

			float posX;
			float posY;

			std::string m_Text;
			color_t m_color_t;
		};

		struct EDrawGradient
		{
			EDrawGradient( float posX, float posY, float m_Width, float h, color_t m_color_t, color_t m_color_tNext )
			{
				this->posX = posX;
				this->posY = posY;
				this->m_Width = m_Width;
				this->m_Height = h;
				this->m_color_t = m_color_t;
				this->m_color_tNext = m_color_tNext;
			}

			float posX;
			float posY;
			float m_Width;
			float m_Height;

			color_t m_color_t;
			color_t m_color_tNext;
		};

		struct EDrawGradient2
		{
			EDrawGradient2( float posX, float posY, float m_Width, float h, Color m_color_t, Color m_color_tNext )
			{
				this->posX = posX;
				this->posY = posY;
				this->m_Width = m_Width;
				this->m_Height = h;
				this->m_color_t = m_color_t;
				this->m_color_tNext = m_color_tNext;
			}

			float posX;
			float posY;
			float m_Width;
			float m_Height;

			Color m_color_t;
			Color m_color_tNext;
		};
	}

	class Details {
	private:

		// Menu state.
		bool m_Show{ };

		// Our tabs.
		int m_Index{ };

	public:

		// Deconstructor.
		~Details( void ) = default;

		// Set show state.
		void SetMenuState( const bool m_State );

		// Get show state.
		bool GetMenuState( );

		// Setup all elements.
		void Install( );

		// Gui color_t
		color_t m_Defaultcolor_t = color_t( 200, 45, 90, 255 );

	};

	class Input {
	private:

		// Mouse position.
		vec2_t m_MousePos{ };

		// Mouse wheel activity.
		float m_MouseWheel{ };

	public:

		// Init input ( mouse, ... ).
		void PollInput( );

		// Return mouse position.
		vec2_t GetMousePos( );

		// Set mouse position.
		void SetMousePos( const vec2_t m_Pos );

		// Return pressed key.
		bool KeyPressed( const uintptr_t m_Key );

		// Return whether the mouse is hovering over a specific object.
		bool MousePointer( const vec2_t m_Pos, const vec2_t m_Size );

		// Set mouse wheel.
		void SetMouseWheel( const float m_Mouse );

		// Get mouse wheel.
		float GetMouseWheel( );

		// Some additions.
		struct
		{

			bool m_KeyState[ 256 ]{ };
			bool m_PrevKeyState[ 256 ]{ };

		} Helpers;

	};

	class Control {
	private:

		// Elements index.
		int m_Index{ };

	public:

		// Controls type.
		enum ControlType
		{
			// Type: combo.
			COMBO,
			// Type: multi.
			MULTICOMBO,
			// Type: color_tpicker.
			color_tSELECTOR,
			// Type: textinput.
			TEXTBOX,
			// Type: bind.
			BIND,
			// Enum size.
			SIZE
		};

		// Can animate.
		bool m_Activity[ 5 ];

		// Return last element index.
		int GetIndex( );

		// Set last element index.
		void SetIndex( const uintptr_t m_Last );

		// Time since opened.
		int m_Opened[ ControlType::SIZE ];

		// Element opened.
		bool m_OpenedState[ ControlType::SIZE ][ m_Maximum ];

		// Can scroll or do animations.
		bool IsPossible( );

		// Is color_t picker opened.
		bool color_tPickerOpened;
	};

	class ExternalRendering {
	public:

		void Install( );
		void FilledRect2( float x, float y, float w, float h, Color c );

		std::vector< External::EDrawText > TextDraw;

		std::vector< External::EDrawRect > RectDraw;
		std::vector< External::EDrawRect2 > RectDraw2;

		std::vector< External::EDrawRect > OutlineDraw;
		std::vector< External::EDrawRect2 > OutlineDraw2;

		std::vector< External::EDrawGradient > GradientVDraw;
		std::vector< External::EDrawGradient > GradientHDraw;

		std::vector< External::EDrawGradient2 > GradientVDraw2;
		std::vector< External::EDrawGradient2 > GradientHDraw2;
	};

	class Config {
	public:
		void LoadConfig( );
		void SaveConfig( );
	};

	extern Gui::Input m_Input;
	extern Gui::Details m_Details;
	extern Gui::Control m_Control;
	extern Gui::ExternalRendering m_External;
	extern Gui::Config m_Cfg;
}
