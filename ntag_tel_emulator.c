
#include "ntag_tel_emulator.h"

NtagTelEmulatorModel* ntag_tel_emulator_model_alloc(){
    NtagTelEmulatorModel* instance = malloc(sizeof(NtagTelEmulatorModel));

    instance->nfc_file_name = furi_string_alloc();

    return instance;
}

void ntag_tel_emulator_model_free(NtagTelEmulatorModel* instance){
    furi_string_free(instance->nfc_file_name);
    free(instance);
}

NtagTelEmulatorData* ntag_tel_emulator_data_alloc() {
    NtagTelEmulatorData* instance = malloc(sizeof(NtagTelEmulatorData));
    
    //instance->scene_manager = scene_manager_alloc()
    
    instance->view_dispatcher = view_dispatcher_alloc();
    instance->dialog_ex = dialog_ex_alloc();
    view_dispatcher_add_view(instance->view_dispatcher, 
            NtagTelEmulatorViewDialog, 
            dialog_ex_get_view(instance->dialog_ex));

    instance->model = ntag_tel_emulator_model_alloc();
    return instance;
}

void ntag_tel_emulator_data_free(NtagTelEmulatorData* instance){
    ntag_tel_emulator_model_free(instance->model);

    view_dispatcher_remove_view(instance->view_dispatcher, NtagTelEmulatorViewDialog);
    dialog_ex_free(instance->dialog_ex);
    view_dispatcher_free(instance->view_dispatcher);

    free(instance);
}

int ntag_tel_emulator_app() {
    return 0;
}
