#include "scenes.h"

const AppSceneOnEnterCallback nte_scene_enter_handlers[] = {};
const AppSceneOnEventCallback nte_scene_event_handlers[] = {};
const AppSceneOnExitCallback nte_scene_exit_handlers[] = {};

const SceneManagerHandlers nte_scene_manager_handlers = {
	.on_enter_handlers = nte_scene_enter_handlers,
	.on_event_handlers = nte_scene_event_handlers,
	.on_exit_handlers = nte_scene_exit_handlers,
	.scene_num = NTESceneMAX
};
