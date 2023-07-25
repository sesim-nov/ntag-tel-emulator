#pragma once

#include <furi.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/text_input.h>
#include <lib/nfc/nfc_device.h>

extern const int NTE_TEXT_BUFFER_MAX;

typedef struct {
    FuriString* nfc_file_name;
    FuriString* phone_number;

    // NFC device to store and process generated nfc data. 
    NfcDevice* nfc;
} NtagTelEmulatorModel;

typedef struct {
    SceneManager* scene_manager;

    ViewDispatcher* view_dispatcher;
    DialogEx* dialog_ex;
    TextInput* text_input;

    char* text_input_buffer;

    NtagTelEmulatorModel* model;
} NtagTelEmulatorData;

typedef enum {
    NtagTelEmulatorViewDialog,
    NtagTelEmulatorViewTextInput,
    NtagTelEmulatorViewMAX,
} NtagTelEmulatorViews;
