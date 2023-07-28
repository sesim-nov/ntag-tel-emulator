#include <gui/scene_manager.h>

typedef enum{
	NTESceneWelcome,
	NTESceneNumberEntry,
	NTESceneFilename,
	NTESceneMAX,
} NTEScenes;

extern const SceneManagerHandlers nte_scene_manager_handlers;
