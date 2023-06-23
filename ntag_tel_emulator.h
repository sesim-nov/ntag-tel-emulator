#pragma once

#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>

typedef struct {
    FuriString* nfc_file_name;
} NtagTelEmulatorModel;

typedef struct {
    SceneManager* scene_manager;

    ViewDispatcher* view_dispatcher;
    DialogEx* dialog_ex;

    NtagTelEmulatorModel* model;
} NtagTelEmulatorData;

