#include "m_render.h"

DirectRender DrawList;

namespace Fonts
{
	std::shared_ptr<c_font> Main = std::make_shared<c_font>( "Verdana", 7, 100 );
	std::shared_ptr<c_font> Icon = std::make_shared<c_font>( "Verdana", 10, 100 );
}

void DirectRender::Init( IDirect3DDevice9* dev )
{
	if ( bReady )
		return;

	m_Fonts.push_back( Fonts::Main );
	m_Fonts.push_back( Fonts::Icon );

	InitDevice( dev );
	bReady = true;
}

void DirectRender::InitDevice( IDirect3DDevice9* dev )
{
	this->dev = dev;

	for ( auto& font : m_Fonts )
	{
		font->init_device_objects( dev );
		font->restore_device_objects( );
	}
}

void DirectRender::InvalidateDevice( )
{
	dev = nullptr;

	for ( auto& font : m_Fonts )
		font->invalidate_device_objects( );
}

void DirectRender::SetupRenderStates( )
{
	dev->SetVertexShader( nullptr );
	dev->SetPixelShader( nullptr );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	dev->SetRenderState( D3DRS_STENCILENABLE, false );

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );

	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	dev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

D3DVIEWPORT9 DirectRender::GetViewport( )
{
	D3DVIEWPORT9 vp;
	dev->GetViewport( &vp );
	return vp;
}

void DirectRender::SetViewport( D3DVIEWPORT9 vp )
{
	dev->SetViewport( &vp );
}

void DirectRender::StringPrototype( vec2_t pos, std::wstring string, color_t c, std::shared_ptr<c_font>& font, uint8_t flags )
{
	font->draw_string( std::roundf( pos.x ), std::roundf( pos.y ),
		D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ), string.c_str( ), flags );
}

void DirectRender::DrawString( vec2_t pos, std::string string, color_t color, std::shared_ptr<c_font>& font, uint8_t flags ) const
{
	StringPrototype( pos, std::wstring( string.begin( ), string.end( ) ),
		color, font, flags );
}

vec2_t DirectRender::TextSizeProtype( std::wstring string, std::shared_ptr<c_font>& font )
{
	SIZE size;
	font->get_text_extent( string.c_str( ), &size );
	return vec2_t( static_cast< float >( size.cx ), static_cast< float >( size.cy ) );
}

vec2_t DirectRender::GetTextSize( std::string string, std::shared_ptr<c_font>& font ) const
{
	return TextSizeProtype( std::wstring( string.begin( ), string.end( ) ), font );
}

void DirectRender::GradientHorizontalPrototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}

void DirectRender::GradientHorizontal( float x, float y, float w, float h, color_t c_a, color_t c_b )
{
	GradientHorizontalPrototype( { x, y }, { w, h }, c_a, c_b );
}

void DirectRender::GradientVerticalPrototype( vec2_t a, vec2_t b, color_t c_a, color_t c_b )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_a.r, c_a.g, c_a.b, c_a.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c_b.r, c_b.g, c_b.b, c_b.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}

void DirectRender::GradientVertical( float x, float y, float w, float h, color_t c_a, color_t c_b )
{
	GradientVerticalPrototype( { x, y }, { w, h }, c_a, c_b );
}

void DirectRender::RectPrototype( vec2_t a, vec2_t b, color_t c )
{
	b += a;

	b.x -= 1;
	b.y -= 1;

	VerticeT verts[ 5 ] = {
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( b.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( b.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( a.x ), float( b.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ float( a.x ), float( a.y ), 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &verts, 20 );
}

void DirectRender::Rect( float x, float y, float w, float h, color_t c )
{
	RectPrototype( { x, y }, { w + 1, h + 1 }, c );
}

void DirectRender::FilledPrototype( vec2_t a, vec2_t b, color_t c )
{
	b += a;

	VerticeT verts[ 4 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &verts, 20 );
}

void DirectRender::FilledRect( float x, float y, float w, float h, color_t c )
{
	FilledPrototype( { x, y }, { w, h }, c );
}

void DirectRender::LinePrototype( vec2_t a, vec2_t b, color_t c )
{
	VerticeT verts[ 2 ] = {
		{ a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) },
		{ b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA( c.r, c.g, c.b, c.a ) }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINELIST, 1, &verts, 20 );
}

void DirectRender::Line( float x, float y, float x1, float x2, color_t c )
{
	LinePrototype( { x, y }, { x1, x2 }, c );
}