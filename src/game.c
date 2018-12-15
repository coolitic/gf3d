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
#include "entity.h"

int main(int argc,char *argv[])
{
    int done = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        0                       //validation
    );
    
    // main game loop
    slog("gf3d main loop begin");
    loadEntity("agumon", "agumon");
	loadEntity("cube", "cube");
	int cursorX = 0, cursorY = 0;
	Uint32 mouseButtons;
	SDL_SetRelativeMouseMode(1);
	Vector3D move;
	float camX, camY;
	camX = camY = 0;
	move = vector3d(0, 0, 0);
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		SDL_GetRelativeMouseState(&cursorX, &cursorY);

		camX += cursorX;
		camY += cursorY;
		
		if (keys[SDL_SCANCODE_T])
			moveEntity(1, vector3d(0.1, 0.1, 0.1));
		if (keys[SDL_SCANCODE_W])
			move.z += 1;
		if (keys[SDL_SCANCODE_A])
			move.x += 1;
		if (keys[SDL_SCANCODE_S])
			move.z -= 1;
		if (keys[SDL_SCANCODE_D])
			move.x -= 1;

		updateCamera(-camX / 1000, -camY / 1000, move);

        //update game things here
        
        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
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
