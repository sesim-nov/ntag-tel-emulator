
#include "ntag_tel_emulator.h"
#include "scenes.h"
#include "ntag_tools.h"
#include <lib/nfc/protocols/mifare_ultralight.h>

NtagTelEmulatorModel* ntag_tel_emulator_model_alloc(){
    NtagTelEmulatorModel* instance = malloc(sizeof(NtagTelEmulatorModel));

    instance->nfc_file_name = furi_string_alloc();

    // NFC Device
    instance->nfc = nfc_device_alloc();

    // NFC Data
    // TODO: Move elsewhere
    FURI_LOG_D("NtagEmulator", "Setting up NFC Device...");
    ntag_213_init_blank_tag(instance->nfc);

    size_t tag_data_len = 100;
    uint8_t* tag_data = malloc(tag_data_len*sizeof(uint8_t));
    
    size_t number_len = 11;
    char* number = "15558675309";
    uint8_t* number_uint = (uint8_t*)number;

    tag_data_len = ndef_encode_phone_number(tag_data, tag_data_len, number_uint, number_len);

    NfcDeviceData* dev_data = &instance->nfc->dev_data;
    memcpy(dev_data->mf_ul_data.data + 16, tag_data, tag_data_len);
    dev_data->mf_ul_data.data_size += tag_data_len * sizeof(uint8_t);
    FURI_LOG_D("NtagEmulator", "Set up NFC Device...");
    FURI_LOG_D("NtagEmulator", "Data byte 16... %s", dev_data->mf_ul_data.data+16);

    return instance;
}

void ntag_tel_emulator_model_free(NtagTelEmulatorModel* instance){
    furi_string_free(instance->nfc_file_name);
    nfc_device_free(instance->nfc);
    free(instance);
}

NtagTelEmulatorData* ntag_tel_emulator_data_alloc() {
    NtagTelEmulatorData* instance = malloc(sizeof(NtagTelEmulatorData));
    
    // Scene Manager
    instance->scene_manager = scene_manager_alloc(&nte_scene_manager_handlers, instance);
    
    // View Dispatcher
    instance->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(instance->view_dispatcher);

    // Dialog
    instance->dialog_ex = dialog_ex_alloc();
    view_dispatcher_add_view(instance->view_dispatcher, 
            NtagTelEmulatorViewDialog, 
            dialog_ex_get_view(instance->dialog_ex));

    // Text Input
    instance->text_input = text_input_alloc();
    view_dispatcher_add_view(instance->view_dispatcher, 
            NtagTelEmulatorViewTextInput, 
            text_input_get_view(instance->text_input));

    //Model
    instance->model = ntag_tel_emulator_model_alloc();

    return instance;
}

void ntag_tel_emulator_data_free(NtagTelEmulatorData* instance){
    ntag_tel_emulator_model_free(instance->model);

    view_dispatcher_remove_view(instance->view_dispatcher, NtagTelEmulatorViewDialog);
    view_dispatcher_remove_view(instance->view_dispatcher, NtagTelEmulatorViewTextInput);
    dialog_ex_free(instance->dialog_ex);
    text_input_free(instance->text_input);
    view_dispatcher_free(instance->view_dispatcher);

    free(instance);
}

int ntag_tel_emulator_app() {
    Gui* gui = furi_record_open("gui");
    
    FURI_LOG_D("NtagTelEmulator", "Allocating things...");
    NtagTelEmulatorData* app = ntag_tel_emulator_data_alloc();
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    
    scene_manager_next_scene(app->scene_manager, NTESceneWelcome);

    FURI_LOG_D("NtagTelEmulator","Starting View Dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    furi_record_close("gui");
    ntag_tel_emulator_data_free(app);
    return 0;
}
