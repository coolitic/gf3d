#include "gf3d_model.h"

#define MAX_ENTITIES 5

typedef struct
{
	TextLine name;
	Model *model;
}Entity;

void loadEntity(TextLine name, TextLine modelName);

void drawEntities(Uint32 bufferFrame, VkCommandBuffer commandBuffer);