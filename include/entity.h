#include "gf3d_model.h"
#include "gf3d_matrix.h"

#define MAX_ENTITIES 5

typedef struct
{
	TextLine name;
	Model *model;
	Matrix4 modelMatrix;
}Entity;

/**
 * @brief load an entity
 * @param name the entity name
 * @param modelName the model to be loaded
 */
void loadEntity(TextLine name, TextLine modelName);
/**
 * @brief draw the entities that contain a model
 */
void drawEntities(Uint32 bufferFrame, VkCommandBuffer commandBuffer);
/**
 * @brief move the specified entity by delta
 * @param index the entity index
 * @param delta the change in position
 */
void moveEntity(Uint16 index, Vector3D delta);
/**
 * @brief free an entity
 * @param index the entity index
 */
void freeEntity(Uint16 index);