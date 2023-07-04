
#include "ntag_tel_emulator.h"
#include "scenes.h"
#include <lib/nfc/protocols/mifare_ultralight.h>

NtagTelEmulatorModel* ntag_tel_emulator_model_alloc(){
    NtagTelEmulatorModel* instance = malloc(sizeof(NtagTelEmulatorModel));

    instance->nfc_file_name = furi_string_alloc();

    // NFC Device
    instance->nfc = nfc_device_alloc();

    // NFC Data
    // TODO: Move elsewhere
    FURI_LOG_D("NtagEmulator", "Setting up NFC Device...");
    instance->nfc->format = NfcDeviceSaveFormatMifareUl;
    NfcDeviceData* dev_data = &instance->nfc->dev_data;
    FuriHalNfcDevData* nfc_data = &dev_data->nfc_data;
    
    nfc_data->uid_len = 7;
    uint8_t new_uid[] = {0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    memcpy(&nfc_data->uid, &new_uid, 7);

    nfc_data->atqa[0] = 0x00;
    nfc_data->atqa[1] = 0x44;

    nfc_data->sak = 0x00;

    dev_data->mf_ul_data.type = MfUltralightTypeNTAG213;

    dev_data->mf_ul_data.data_size = 4;
    uint8_t new_data[] = {0xd3, 0x4d, 0xbe, 0xef};
    memcpy(&dev_data->mf_ul_data.data, &new_data, 4);

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

    //Model
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
