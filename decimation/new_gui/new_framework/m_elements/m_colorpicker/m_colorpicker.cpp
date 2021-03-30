#include "m_colorpicker.h"

/* animatino shits and values / strings */
static auto StoredIndex = -1;
static bool m_State = false;

struct RGB { double r, g, b; };
struct HSV { double h, s, v; };

static int m_AlphaIn[ Gui::m_Maximum ];

/* hsv to rgb and rgb to hsv */
HSV RGB_TO_HSV( const RGB& In )
{
	HSV m_Result;
	double Min, Max, Delta;

	Min = In.r < In.g ? In.r : In.g;
	Min = Min < In.b ? Min : In.b;

	Max = In.r > In.g ? In.r : In.g;
	Max = Max > In.b ? Max : In.b;

	m_Result.v = Max;
	Delta = Max - Min;

	if ( Delta < 0.0001 ) {
		m_Result.s = 0;
		m_Result.h = 0;

		return m_Result;
	}

	if ( Max > 0 ) {
		m_Result.s = ( Delta / Max );
	}
	else {
		m_Result.s = 0;
		m_Result.h = NAN;

		return m_Result;
	}

	if ( In.r >= Max ) {
		m_Result.h = ( In.g - In.b ) / Delta;
	}
	else {
		if ( In.g >= Max ) {
			m_Result.h = 2 + ( In.b - In.r ) / Delta;
		}
		else {
			m_Result.h = 4 + ( In.r - In.g ) / Delta;
		}
	}

	m_Result.h *= 60;

	if ( m_Result.h < 0 )
		m_Result.h += 360;

	return m_Result;
}
RGB HSV_TO_RGB( const HSV& In )
{
	RGB m_Result;

	double HH, P, Q, T, FF;
	long i;

	if ( In.s <= 0 ) {
		m_Result.r = In.v;
		m_Result.g = In.v;
		m_Result.b = In.v;

		return m_Result;
	}

	HH = ( In.h >= 360 ? 0 : In.h ) / 60;
	i = ( long )HH;

	FF = HH - i;

	P = In.v * ( 1 - In.s );
	Q = In.v * ( 1 - ( In.s * FF ) );
	T = In.v * ( 1 - ( In.s * ( 1 - FF ) ) );

	switch ( i ) {
	case 0:
		m_Result.r = In.v;
		m_Result.g = T;
		m_Result.b = P;

		break;

	case 1:
		m_Result.r = Q;
		m_Result.g = In.v;
		m_Result.b = P;

		break;

	case 2:
		m_Result.r = P;
		m_Result.g = In.v;
		m_Result.b = T;

		break;

	case 3:
		m_Result.r = P;
		m_Result.g = Q;
		m_Result.b = In.v;

		break;

	case 4:
		m_Result.r = T;
		m_Result.g = P;
		m_Result.b = In.v;

		break;

	case 5:
	default:
		m_Result.r = In.v;
		m_Result.g = P;
		m_Result.b = Q;

		break;

	}

	return m_Result;
}

/* setup colorpicker */
ColorPicker::ColorPicker( Color* m_Select, float* m_Hue, bool m_Alpha ) {
	this->m_Select = m_Select;
	this->m_AlphaBar = m_Alpha;

	HSV NewColor = RGB_TO_HSV( {
		( double )( ( float )m_Select->r() / 255.f ),
		( double )( ( float )m_Select->g() / 255.f ),
		( double )( ( float )m_Select->b() / 255.f )
		} );

	this->m_Hue = m_Hue;
	this->m_CursorPos.x = ( float )NewColor.s * 150.f;
	this->m_CursorPos.y = 150.f - ( ( float )NewColor.v * 150.f );

	if ( m_Alpha )
		this->m_Alpha = ( float )m_Select->a() / 255.f;
	else
		this->m_Alpha = 1.0f;
}

/* get color */
static Color GetColor( int i ) {
	auto ST = m_AlphaIn[ Gui::m_Control.GetIndex( ) ];

	switch ( i )
	{
	case 0:
		return Color( 255, 0, 0, ST );
		break;
	case 1:
		return Color( 255, 255, 0, ST );
		break;
	case 2:
		return Color( 0, 255, 0, ST );
		break;
	case 3:
		return Color( 0, 255, 255, ST );
		break;
	case 4:
		return Color( 0, 0, 255, ST );
		break;
	case 5:
		return Color( 255, 0, 255, ST );
		break;
	case 6:
		return Color( 255, 0, 0, ST );
		break;
	}
}

void ColorPicker::Update( )
{
	if ( m_State && StoredIndex == Gui::m_Control.GetIndex( ) && Gui::m_Input.KeyPressed( VK_LBUTTON ) && !Gui::m_Input.MousePointer( { m_Pos.x + 23, m_Pos.y }, { 180, float( m_AlphaBar ? 180 : 164 ) } ) )
	{
		m_State = !m_State;
		StoredIndex = -1;
	}

	if ( Gui::m_Control.IsPossible( ) && Gui::m_Input.MousePointer( { m_Pos.x, m_Pos.y }, { 17, 9 } ) && Gui::m_Input.KeyPressed( VK_LBUTTON ) && !Gui::m_Control.m_Activity[ 3 ] )
	{
		m_State = !m_State;
		StoredIndex = Gui::m_Control.GetIndex( );
	}

	if ( StoredIndex == Gui::m_Control.GetIndex( ) )
		m_Focused = m_State;

	Gui::m_Control.m_Activity[ 0 ] = m_State;
	Gui::m_Control.color_tPickerOpened = m_Focused;
	Gui::m_Control.m_OpenedState[ Gui::m_Control.ControlType::color_tSELECTOR ][ Gui::m_Control.GetIndex( ) ] = m_Focused;

	if ( m_Focused )
	{
		if ( GetAsyncKeyState( VK_LBUTTON ) )
		{
			if ( Gui::m_Input.MousePointer( { m_Pos.x + 30, m_Pos.y + 6 }, { 150, 150 } ) ) {
				m_CursorPos.x = Gui::m_Input.GetMousePos( ).x - ( m_Pos.x + 30 );
				m_CursorPos.y = Gui::m_Input.GetMousePos( ).y - ( m_Pos.y + 7 );

				if ( m_CursorPos.x < 0.f )
					m_CursorPos.x = 0.f;

				if ( m_CursorPos.x > 150.f )
					m_CursorPos.x = 150.f;

				if ( m_CursorPos.y < 0.f )
					m_CursorPos.y = 0.f;

				if ( m_CursorPos.y > 150.f )
					m_CursorPos.y = 150.f;
			}
			else if ( Gui::m_Input.MousePointer( { m_Pos.x + 26, m_Pos.y + 163 }, { 155, 11 } ) )
			{
				if ( m_AlphaBar )
				{
					m_Alpha = ( Gui::m_Input.GetMousePos( ).x - ( m_Pos.x + 27 ) ) / 153.f;

					if ( m_Alpha < 0.f )
						m_Alpha = 0.f;

					if ( m_Alpha > 1.f )
						m_Alpha = 1.f;
				}
			}
			else if ( Gui::m_Input.MousePointer( { m_Pos.x + 187, m_Pos.y + 7 }, { 11, 150 } ) )
			{
				*m_Hue = ( Gui::m_Input.GetMousePos( ).y - ( m_Pos.y + 7 ) ) / 150.f;

				if ( *m_Hue < 0.f )
					*m_Hue = 0.f;

				if ( *m_Hue > 1.f )
					*m_Hue = 1.f;
			}

			float m_Sat = m_CursorPos.x / 150.f;
			float m_Bright = 1.f - ( m_CursorPos.y / 150.f );

			RGB NewColor = HSV_TO_RGB( { *m_Hue * 360.f, m_Sat, m_Bright } );
			*m_Select = Color((int)( NewColor.r * 255.f ), ( int )( NewColor.g * 255.f ), ( int )( NewColor.b * 255.f ), ( int )( m_Alpha * 255.f ));/*(int)*/
		}
	}
}

void ColorPicker::Draw( )
{
	DrawList.FilledRect( m_Pos.x + 2, m_Pos.y + 2, 13, 9, color_t( 30, 30, 30 ) );
	DrawList.FilledRect( m_Pos.x + 9, m_Pos.y + 7, 6, 4, color_t( 255, 255, 255 ) );
	DrawList.FilledRect( m_Pos.x + 2, m_Pos.y + 2, 6, 4, color_t( 255, 255, 255 ) );

	Gui::m_External.FilledRect2( m_Pos.x + 2, m_Pos.y + 2, 13, 9, *m_Select );
	DrawList.Rect( m_Pos.x, m_Pos.y, 16, 12, color_t( 130, 125, 150 ) );

	if ( m_Focused )
		m_AlphaIn[ Gui::m_Control.GetIndex( ) ] += 10;

	else
		m_AlphaIn[ Gui::m_Control.GetIndex( ) ] -= 10;

	if ( m_AlphaIn[ Gui::m_Control.GetIndex( ) ] > 255 )
		m_AlphaIn[ Gui::m_Control.GetIndex( ) ] = 255;

	if ( m_AlphaIn[ Gui::m_Control.GetIndex( ) ] < 0 )
		m_AlphaIn[ Gui::m_Control.GetIndex( ) ] = 0;

	std::clamp<int>( m_AlphaIn[ Gui::m_Control.GetIndex( ) ], 0, 255 );

	if ( m_AlphaIn[ Gui::m_Control.GetIndex( ) ] > 0 )
	{
		if ( m_AlphaBar )
		{
			Gui::m_External.RectDraw2.insert( Gui::m_External.RectDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 30, m_Pos.y + 163, 150, 11, *m_Select ) );
			Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 30, m_Pos.y + 163, 150, 11, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
			Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 30 + ( 146.f * m_Alpha ), m_Pos.y + 163, 4, 11, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
		}

		Gui::m_External.RectDraw2.insert( Gui::m_External.RectDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 23, m_Pos.y, 180, m_AlphaBar ? 180 : 164, Color( 30, 20, 30, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
		Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 23, m_Pos.y, 180, m_AlphaBar ? 180 : 164, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );

		Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 29, m_Pos.y + 6, 151, 151, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
		Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 186, m_Pos.y + 6, 12, 151, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );

		RGB hue_color_t = HSV_TO_RGB( { ( double )*m_Hue * 360.f, 1.f, 1.f } );
		Gui::m_External.GradientHDraw2.insert( Gui::m_External.GradientHDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 30, m_Pos.y + 7, 150, 150, Color( 255, 255, 255, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ), Color(
			( int )( hue_color_t.r * 255.0f ),
			( int )( hue_color_t.g * 255.0f ),
			( int )( hue_color_t.b * 255.0f ),
			m_AlphaIn[ Gui::m_Control.GetIndex( ) ]
		) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 30, m_Pos.y + 7, 150, 150, Color( 0, 0, 0, 0 ), Color( 0, 0, 0, 255 ) ) );

		/* done */
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 0 ), 11, 25, GetColor( 0 ), GetColor( 1 ) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 1 ), 11, 25, GetColor( 1 ), GetColor( 2 ) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 2 ), 11, 25, GetColor( 2 ), GetColor( 3 ) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 3 ), 11, 25, GetColor( 3 ), GetColor( 4 ) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 4 ), 11, 25, GetColor( 4 ), GetColor( 5 ) ) );
		Gui::m_External.GradientVDraw2.insert( Gui::m_External.GradientVDraw2.begin( ), Gui::External::EDrawGradient2( m_Pos.x + 187, m_Pos.y + 7 + ( 25 * 5 ), 11, 25, GetColor( 5 ), GetColor( 6 ) ) );

		Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 30 + ( 148.f * ( m_CursorPos.x / 150.0f ) ), m_Pos.y + 6 + ( 148.f * ( m_CursorPos.y / 150.f ) ), 4, 4, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
		Gui::m_External.OutlineDraw2.insert( Gui::m_External.OutlineDraw2.begin( ), Gui::External::EDrawRect2( m_Pos.x + 186, m_Pos.y + 6 + ( 146.f * *m_Hue ), 12, 4, Color( 130, 130, 170, m_AlphaIn[ Gui::m_Control.GetIndex( ) ] ) ) );
	}
}
