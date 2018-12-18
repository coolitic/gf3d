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
 * @brief return the index of the 1st occurrence of the inputted name in the entity list
 * @param name the name to be searched for
 */
Uint16 findEntity(TextLine name);
/**
 * @brief move entity by delta
 * @param index the entity index
 * @param delta the change in position
 */
void moveEntity(Uint16 index, Vector3D delta);
/**
 * @brief set entity position
 * @param index the entity index
 * @param pos the absolute position
 */
void posEntity(Uint16 index, Vector3D pos);
/**
 * @brief rotate entity by delta
 * @param index the entity index
 * @param delta the change in rotation
 */
void rotateEntity(Uint16 index, Vector3D delta);
/**
 * @brief set entity scale
 * @param index the entity index
 * @param pos the absolute scale
 */
void scaleEntity(Uint16 index, Vector3D scale);
/**
 * @brief free an entity
 * @param index the entity index
 */
void freeEntity(Uint16 index);