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

void moveEntity(Uint16 index, Vector3D delta) {
	entity[index].modelMatrix[3][0] += delta.x;
	entity[index].modelMatrix[3][1] += delta.y;
	entity[index].modelMatrix[3][2] += delta.z;
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
}