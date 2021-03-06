#include <SDL.h>            

#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_matrix.h"
#include "gf3d_camera.h"
#include "gf3d_vector.h"
#include "gf3d_texture.h"
#include <SDL_mixer.h>
#include "entity.h"

int main(int argc, char *argv[])
{
	int done = 0;
	const Uint8 * keys;
	Uint32 bufferFrame = 0;
	VkCommandBuffer commandBuffer;
	float sens = 0.001;

	// Music code
	Mix_OpenAudio(44100, 0x8010, 2, 1024);
	Mix_Init(MIX_INIT_OGG);
	Mix_AllocateChannels(2);
	Mix_Music *music;

	init_logger("gf3d.log");
	slog("gf3d begin");
	gf3d_vgraphics_init(
		"gf3d",                 //program name
		1200,                   //screen width
		700,                    //screen height
		vector4d(0.51, 0.75, 1, 1),//background color
		0,                      //fullscreen
		0                       //validation
	);

	// main game loop
	slog("gf3d main loop begin");
	loadEntity("agumon", "agumon");
	loadEntity("cube", "cube");
	loadEntity("cube1", "cube");
	loadEntity("cube2", "cube");
	int cursorX = 0, cursorY = 0;
	Uint32 mouseButtons;
	SDL_SetRelativeMouseMode(1);
	Vector3D pos;
	float camX, camY;
	camX = camY = 0;
	pos = vector3d(0, 0, 0);
	float cosCam, sinCam;
	SDL_KeyboardEvent keyEvent;
	Uint8 repeat = 0; // for filtering out some repeating keys

	music = Mix_LoadMUS("music/music.ogg");

	if (!music)
		slog(Mix_GetError());
	else {
		Mix_PlayMusic(music, -1);
		slog("music loaded");
		atexit(Mix_Quit);
	}

	// setup starting scene
	posEntity(0, vector3d(0, 0, 20));
	scaleEntity(1, vector3d(100, 0, 100));
	posEntity(1, vector3d(0, -5, 0));
	posEntity(2, vector3d(-20, 0, -20));
	posEntity(3, vector3d(20, 0, -20));
	scaleEntity(2, vector3d(1, 1, 1));
	scaleEntity(3, vector3d(3, 3, 3));

	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		SDL_GetRelativeMouseState(&cursorX, &cursorY);

		// Update absolute yaw and pitch for the camera
		camX += cursorX * sens;
		camY += cursorY * sens;

		// Reset yaw after doing a 360 degree turn
		while (camX > 2 * M_PI)
			camX -= 2 * M_PI;
		while (camX < 2 * M_PI)
			camX += 2 * M_PI;
		// Lock pitch to prevent gimbal-lock
		if (camY > M_PI / 2)
			camY = M_PI / 2;
		if (camY < -M_PI / 2)
			camY = -M_PI / 2;

		cosCam = cos(camX);
		sinCam = sin(camX);

		// if-else block for handling key events that shouldn't repeat
		if (keys[SDL_SCANCODE_H]) {
			if (repeat == 0) {
				loadEntity("agumon", "agumon");
				repeat = 1;
			}
		}
		else if (keys[SDL_SCANCODE_T]) {
			if (repeat == 0) {
				freeEntity(0);
				repeat = 1;
			}
		}
		else
			repeat = 0;

		// Position is updated with deltas that are dependent on camera orientation
		if (keys[SDL_SCANCODE_W]) {
			pos.x -= sinCam;
			pos.z += cosCam;
		}
		if (keys[SDL_SCANCODE_A]) {
			pos.x += cosCam;
			pos.z += sinCam;
		}
		if (keys[SDL_SCANCODE_S]) {
			pos.x += sinCam;
			pos.z -= cosCam;
		}
		if (keys[SDL_SCANCODE_D]) {
			pos.x -= cosCam;
			pos.z -= sinCam;
		}

		if (keys[SDL_SCANCODE_J])
			moveEntity(2, vector3d(-1, 0, 0));
		if (keys[SDL_SCANCODE_L])
			moveEntity(2, vector3d(1, 0, 0));
		if (keys[SDL_SCANCODE_I])
			moveEntity(2, vector3d(0, 1, 0));
		if (keys[SDL_SCANCODE_K])
			moveEntity(2, vector3d(0, -1, 0));
		if (keys[SDL_SCANCODE_COMMA])
			moveEntity(2, vector3d(0, 0, 1));
		if (keys[SDL_SCANCODE_PERIOD])
			moveEntity(2, vector3d(0, 0, -1));

        //update game things here
		rotateEntity(0, vector3d(0, 0.05, 0));

		updateCamera(-camX, -camY, pos);

        /* configure render command for graphics command pool
        for each mesh, get a command and configure it from the pool*/
        bufferFrame = gf3d_vgraphics_render_begin();
        commandBuffer = gf3d_command_rendering_begin(bufferFrame);

		drawEntities(bufferFrame, commandBuffer);
            
        gf3d_command_rendering_end(commandBuffer);
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
