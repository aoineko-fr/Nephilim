@echo off
REM setlocal EnableDelayedExpansion

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=.\content

if not exist %Dest% md %Dest%

echo Building image data...

:: Export title bitmap
@REM %MSXtk%\MSXimg.exe datasrc\title.png -out %Dest%\title.h -mode bmp -bpc 4 -pal custom
@REM %MSXtk%\MSXimg.exe datasrc\title.png -out %Dest%\title.bin -mode bmp -bpc 4 -pal custom
@REM %Tools%\compress\Pletter\pletter.exe %Dest%\title.bin %Dest%\title.pl5
@REM %MSXtk%\MSXbin.exe %Dest%\title.pl5 -t g_TitleBmp -ad -o content\title_bmp.h

:: Export controls bitmap
%MSXtk%\MSXimg.exe datasrc\Controls_Screen_sc4.png -out %Dest%\control.h -name g_ControlMenu -mode gm2 -compress pletter --tilesComp -pal msx1
@REM %MSXtk%\MSXimg.exe datasrc\Controls_Screen_sc4.png -out %Dest%\control.h -name g_ControlMenu -mode gm2 -pal msx1

:: Export sprites
%MSXtk%\MSXimg.exe datasrc\MSX_Spritesheet_2.png -out %Dest%\sprite_player1.h -name g_SpritePlayer1 -mode sprt -pos 0 0 -size 16 16 -num 3 1 -pal custom