#include "simple_logger.h"
#include "gf3d_vgraphics.h"
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
				entity[x].model = gf3d_model_load(modelName, x);
			gf3d_matrix_identity(entity[x].modelMatrix);
			return;
		}
	slog("no space for new entity");
}

void drawEntities(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	for (Uint16 x = 0; x < MAX_ENTITIES; x++)
		if (entity[x].model != 0) {
			gf3d_vgraphics_update_uniform_buffer(x, entity[x].modelMatrix);
			gf3d_model_draw(entity[x].model, bufferFrame, commandBuffer);
		}
}

Uint16 findEntity(TextLine name) {
	for (Uint16 x = 0; x < MAX_ENTITIES; x++)
		if (!gf3d_line_cmp(entity[x].name, name))
			return x;
	slog("entity name not found");
}

void moveEntity(Uint16 index, Vector3D delta) {
	// scale accuracy with speed
	Uint16 spliceMax = ceil((abs(delta.x) + abs(delta.y) + abs(delta.z)) * 100);
	for (Uint16 splice = 0; splice < spliceMax; splice++) {
		for (Uint16 x = 0; x < MAX_ENTITIES; x++)
			// check for collisions
			if (x != index && entity[x].model != NULL &&
				abs(entity[index].modelMatrix[3][0] - entity[x].modelMatrix[3][0]) - (entity[index].modelMatrix[0][0] + entity[x].modelMatrix[0][0]) < 0 &&
				abs(entity[index].modelMatrix[3][1] - entity[x].modelMatrix[3][1]) - (entity[index].modelMatrix[1][1] + entity[x].modelMatrix[1][1]) < 0 &&
				abs(entity[index].modelMatrix[3][2] - entity[x].modelMatrix[3][2]) - (entity[index].modelMatrix[2][2] + entity[x].modelMatrix[2][2]) < 0)
			{
				// move back a bit upon detecting a collision so objects dont get stuck
				entity[index].modelMatrix[3][0] -= delta.x / spliceMax;
				entity[index].modelMatrix[3][1] -= delta.y / spliceMax;
				entity[index].modelMatrix[3][2] -= delta.z / spliceMax;
				return;
			}
		entity[index].modelMatrix[3][0] += delta.x / spliceMax;
		entity[index].modelMatrix[3][1] += delta.y / spliceMax;
		entity[index].modelMatrix[3][2] += delta.z / spliceMax;
	}
}

void posEntity(Uint16 index, Vector3D pos) {
	entity[index].modelMatrix[3][0] = pos.x;
	entity[index].modelMatrix[3][1] = pos.y;
	entity[index].modelMatrix[3][2] = pos.z;
}

void rotateEntity(Uint16 index, Vector3D delta) {
	gf3d_matrix_rotate(entity[index].modelMatrix, entity[index].modelMatrix, delta.x, vector3d(1, 0, 0));
	gf3d_matrix_rotate(entity[index].modelMatrix, entity[index].modelMatrix, delta.y, vector3d(0, 1, 0));
	gf3d_matrix_rotate(entity[index].modelMatrix, entity[index].modelMatrix, delta.z, vector3d(0, 0, 1));
}

void scaleEntity(Uint16 index, Vector3D scale) {
	entity[index].modelMatrix[0][0] = scale.x;
	entity[index].modelMatrix[1][1] = scale.y;
	entity[index].modelMatrix[2][2] = scale.z;
}

void freeEntity(Uint16 index) {
	gf3d_line_cpy(entity[index].name, "");
	gf3d_model_free(entity[index].model);
	entity[index].model = NULL;
}