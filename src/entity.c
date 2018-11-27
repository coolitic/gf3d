#include "simple_logger.h"
#include "entity.h"

Entity entity[MAX_ENTITIES] = { 0 };

void loadEntity(TextLine name, TextLine modelName) {
	if (!gf3d_line_cmp(name, "")) {
		slog("Entity must have a name");
		return;
	}
	for (Uint16 x = 0; x < MAX_ENTITIES; x++)
		if (!gf3d_line_cmp(entity[x].name, "")) {
			gf3d_line_cpy(entity[x].name, name);
			if (gf3d_line_cmp(modelName, ""))
				entity[x].model = gf3d_model_load(modelName);
			return;
		}
	slog("no space for new entity");
}

void drawEntities(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	for (Uint16 x = 0; x < MAX_ENTITIES; x++)
		if (entity[x].model != 0)
			gf3d_model_draw(entity[x].model, bufferFrame, commandBuffer);
}