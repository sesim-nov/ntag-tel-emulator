#include "ntag_tel_emulator.h"
#include "ntag_tools.h"

#include <gui/modules/text_input.h>

void scene_number_entry_text_input_callback(void* ctx) {
    NtagTelEmulatorData* instance = ctx;

    furi_string_set_str(instance->model->phone_number, instance->text_input_buffer);

    //Quit Application
    scene_manager_stop(instance->scene_manager);
    view_dispatcher_stop(instance->view_dispatcher);
}

void scene_get_number_on_enter(void* ctx){
    NtagTelEmulatorData* instance = ctx;

    text_input_set_header_text(instance->text_input, "Enter a phone number.");

    text_input_set_result_callback(
            instance->text_input,
            scene_number_entry_text_input_callback,
            ctx, instance->text_input_buffer,
            NTE_TEXT_BUFFER_MAX,
            true);

    view_dispatcher_switch_to_view(instance->view_dispatcher, NtagTelEmulatorViewTextInput);
}
bool scene_get_number_on_event(void* ctx, SceneManagerEvent event){
    UNUSED(ctx);
    UNUSED(event);
    return false;
}
void scene_get_number_on_exit(void* ctx){
    NtagTelEmulatorData* instance = ctx;

    text_input_reset(instance->text_input);
}
