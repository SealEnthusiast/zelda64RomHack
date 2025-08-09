#include <uLib.h>

extern void OvlMessage_Update(PlayState* play);
#if MOTION_BLUR

extern void Gameplay_DrawMotionBlur(PlayState* play);
#endif


void z64rom_PrePlayUpdate(PlayState* play) {
	
}

void z64rom_PostPlayUpdate(PlayState* play) {
	OvlMessage_Update(play);
}

void z64rom_PrePlayDraw(PlayState* play) {
	
}

void z64rom_PostPlayDraw(PlayState* play) {
#if MOTION_BLUR
Gameplay_DrawMotionBlur(play);
#endif
}
