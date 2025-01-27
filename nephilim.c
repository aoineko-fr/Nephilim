// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Program template
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "fsm.h"
#include "compress/pletter.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Function prototypes
void State_TitleInit();
void State_TitleUpdate();
void State_ControlInit();
void State_ControlUpdate();
void State_GameInit();
void State_GameUpdate();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
// #include "font/font_mgl_sample6.h"

// Title screen data
#include "content/title_bmp.h"
#include "content/title_palette.h"

// Control screen data
#include "content/control.h"
#include "palette/pal_msx1.h"

// Control screen data
#include "content/sprite_player1.h"

// Animation characters
const u8 g_ChrAnim[] = { '-', '/', '|', '\\' };

const FSM_State g_StateTitle    = { 0, State_TitleInit, State_TitleUpdate, NULL };
const FSM_State g_StateControl  = { 0, State_ControlInit, State_ControlUpdate, NULL };
const FSM_State g_StateGame     = { 0, State_GameInit, State_GameUpdate, NULL };

//=============================================================================
// VARIABLES
//=============================================================================

u8 g_FrameCount = 0;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// 
void State_TitleInit()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_BLACK);
	VDP_EnableSprite(FALSE);
	VDP_ClearVRAM();

	VDP_SetPalette(table_palette);
	Pletter_UnpackToVRAM(g_TitleBmp, 32 * 128);
	VDP_FillVRAM_16K(0, (32 + 88) * 128, 128);
}

//-----------------------------------------------------------------------------
// 
void State_TitleUpdate()
{
	if(Keyboard_IsKeyPressed(KEY_SPACE))
	{
		FSM_SetState(&g_StateControl);
	}
}

//-----------------------------------------------------------------------------
// 
void State_ControlInit()
{
	VDP_SetMode(VDP_MODE_SCREEN4);
	VDP_SetColor(COLOR_BLACK);
	VDP_EnableSprite(FALSE);
	VDP_ClearVRAM();

	VDP_SetPalette(g_PaletteMSX1);

	VDP_SetPatternTable(0x0000); // 768 * 8
	VDP_SetColorTable(0x2000); // 768 * 8
	VDP_SetLayoutTable(0x1800); // 32 * 24 (32 * 26.5)

	Pletter_UnpackToVRAM(g_ControlMenu_Names, VDP_GetLayoutTable()); // 0x0000 - 0x3FFF
	Pletter_UnpackToVRAM(g_ControlMenu_Patterns, VDP_GetPatternTable());
	Pletter_UnpackToVRAM(g_ControlMenu_Patterns, VDP_GetPatternTable() + 256 * 8);
	Pletter_UnpackToVRAM(g_ControlMenu_Patterns, VDP_GetPatternTable() + 512 * 8);
	Pletter_UnpackToVRAM(g_ControlMenu_Colors, VDP_GetColorTable());
	Pletter_UnpackToVRAM(g_ControlMenu_Colors, VDP_GetColorTable() + 256 * 8);
	Pletter_UnpackToVRAM(g_ControlMenu_Colors, VDP_GetColorTable() + 512 * 8);

	// VDP_WriteVRAM_16K(g_ControlMenu_Names, VDP_GetLayoutTable(), );
}

//-----------------------------------------------------------------------------
// 
void State_ControlUpdate()
{
	if(Keyboard_IsKeyPressed(KEY_SPACE))
	{
		FSM_SetState(&g_StateGame);
	}
}

//-----------------------------------------------------------------------------
// 
void State_GameInit()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

	VDP_SetPalette(g_PaletteMSX1);

	VDP_WriteVRAM_128K(g_SpritePlayer1, VADDR_LO(VDP_GetSpritePatternTable()), VADDR_HI(VDP_GetSpritePatternTable()), sizeof(g_SpritePlayer1));
	VDP_SetSpriteExUniColor(0, 100, 100, 1, COLOR_WHITE);
}

const u8 g_Anim[] = { 4, 0, 4, 8 };

//-----------------------------------------------------------------------------
// 
void State_GameUpdate()
{
	VDP_SetSprite(0, g_FrameCount, 100, g_Anim[(g_FrameCount / 8) % 4]);
}


//-----------------------------------------------------------------------------
// V-blank interrupt
void VDP_InterruptHandler()
{
	g_FrameCount++;
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_EnableVBlank(TRUE);
	// FSM_SetState(&g_StateTitle);
	FSM_SetState(&g_StateGame);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt(); // Wait V-Blank

		FSM_Update();
	}

	// Bios_Exit(0);
}