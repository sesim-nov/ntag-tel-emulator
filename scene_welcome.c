#include "ntag_tel_emulator.h"
#include "scenes.h"

void scene_welcome_dialog_result_callback(DialogExResult result, void* ctx) {
    NtagTelEmulatorData* instance = ctx;
    switch(result) {
        case DialogExResultRight:
            scene_manager_next_scene(instance->scene_manager, NTESceneNumberEntry);
            break;
        default:
            nfc_device_save(instance->model->nfc, "/ext/nfc/derp.nfc");
            scene_manager_stop(instance->scene_manager);
            view_dispatcher_stop(instance->view_dispatcher);
    }
}

/*
 * This code is rn when the scene manager switches into this scene
 */
void scene_welcome_on_enter(void* ctx) {
    
    NtagTelEmulatorData* instance = ctx;

    dialog_ex_reset(instance->dialog_ex);

    dialog_ex_set_header(instance->dialog_ex, "NTAG213 Tel Record Generator", 10, 3, AlignLeft, AlignTop);

    dialog_ex_set_text(instance->dialog_ex, "Create DEFAULT test tag or ENTER number?", 10, 30, AlignLeft, AlignTop);

    dialog_ex_set_result_callback(instance->dialog_ex, scene_welcome_dialog_result_callback);

    dialog_ex_set_center_button_text(instance->dialog_ex, "Default");

    dialog_ex_set_right_button_text(instance->dialog_ex, "Enter");

    dialog_ex_set_context(instance->dialog_ex, ctx);

    view_dispatcher_switch_to_view(instance->view_dispatcher, NtagTelEmulatorViewDialog);
}

bool scene_welcome_on_event(void* ctx, SceneManagerEvent event) {
    UNUSED(ctx);
    UNUSED(event);
//    FURI_LOG_D("INPUTHANDLNG", "Got event!");
    return false;
}

void scene_welcome_on_exit(void* ctx) {
//    FURI_LOG_D("InputHandling", "Entered Scene Welcome on Exit");
    NtagTelEmulatorData* instance = ctx;

    dialog_ex_reset(instance->dialog_ex);
}
