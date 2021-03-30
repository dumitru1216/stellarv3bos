#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include "m_font.h"
#include "m_color.h"


struct VerticeT
{
	float x, y, z, rhw;
	int c;
};

namespace Fonts
{
	extern std::shared_ptr<c_font> Main;
	extern std::shared_ptr<c_font> Icon;
}

class DirectRender
{
public:

	// Constructor.
	DirectRender( ) = default;
	~DirectRender( ) = default;

private:

	static void StringPrototype( vec2_t pos, std::wstring string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	static vec2_t TextSizeProtype( std::wstring string, std::shared_ptr<c_font>& font );
	void GradientHorizontalPrototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b );

	void GradientVerticalPrototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b );
	void FilledPrototype( vec2_t a, vec2_t b, color_t c );
	void RectPrototype( vec2_t a, vec2_t b, color_t c );
	void LinePrototype( vec2_t a, vec2_t b, color_t c );

public:
	void SetupRenderStates( );
	void Init( IDirect3DDevice9* dev );

	void InitDevice( IDirect3DDevice9* dev );
	void InvalidateDevice( );

	// ------------------------------------------ //

	D3DVIEWPORT9 GetViewport( );
	void SetViewport( D3DVIEWPORT9 vp );

	// ------------------------------------------ //

	void DrawString( vec2_t pos, std::string string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags = 0 ) const;
	vec2_t GetTextSize( std::string string, std::shared_ptr<c_font>& font ) const;
	void GradientHorizontal( float x, float y, float w, float h, color_t c_a, color_t c_b );
	void GradientVertical( float x, float y, float w, float h, color_t c_a, color_t c_b );
	void FilledRect( float x, float y, float w, float h, color_t c );
	void Rect( float x, float y, float w, float h, color_t c );
	void Line( float x, float y, float x1, float x2, color_t c );
	IDirect3DDevice9* dev;
private:

	bool bReady = false;
	std::vector<std::shared_ptr<c_font>> m_Fonts;
};

extern DirectRender DrawList;