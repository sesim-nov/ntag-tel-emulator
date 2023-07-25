#include "ntag_tel_emulator.h"
#include <gui/modules/text_input.h>

void scene_number_entry_text_input_callback(void* ctx) {
    NtagTelEmulatorData* instance = ctx;
    UNUSED(instance);
}

void scene_get_number_on_enter(void* ctx){
    NtagTelEmulatorData* instance = ctx;

    text_input_set_header_text(instance->text_input, "Enter a phone number.");

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
