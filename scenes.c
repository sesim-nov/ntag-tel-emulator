#include "scenes.h"
#include "scene_welcome.h"
#include "scene_number_entry.h"

const AppSceneOnEnterCallback nte_scene_enter_handlers[] = {scene_welcome_on_enter, scene_get_number_on_enter};
const AppSceneOnEventCallback nte_scene_event_handlers[] = {scene_welcome_on_event, scene_get_number_on_event};
const AppSceneOnExitCallback nte_scene_exit_handlers[] = {scene_welcome_on_exit, scene_get_number_on_exit};

const SceneManagerHandlers nte_scene_manager_handlers = {
	.on_enter_handlers = nte_scene_enter_handlers,
	.on_event_handlers = nte_scene_event_handlers,
	.on_exit_handlers = nte_scene_exit_handlers,
	.scene_num = NTESceneMAX
};
