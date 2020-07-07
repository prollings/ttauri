// Copyright 2019 Pokitec
// All rights reserved.

#include "ttauri/GUI/globals.hpp"
#include "ttauri/GUI/GUISystem.hpp"
#include "ttauri/GUI/ThemeMode.hpp"
#include "ttauri/GUI/ThemeBook.hpp"
#include "ttauri/GUI/RenderDoc.hpp"
#include "ttauri/text/globals.hpp"
#include "ttauri/globals.hpp"

#include "ttauri/GUI/PipelineImage.vert.spv.inl"
#include "ttauri/GUI/PipelineImage.frag.spv.inl"
#include "ttauri/GUI/PipelineFlat.vert.spv.inl"
#include "ttauri/GUI/PipelineFlat.frag.spv.inl"
#include "ttauri/GUI/PipelineBox.vert.spv.inl"
#include "ttauri/GUI/PipelineBox.frag.spv.inl"
#include "ttauri/GUI/PipelineSDF.vert.spv.inl"
#include "ttauri/GUI/PipelineSDF.frag.spv.inl"
#include "ttauri/GUI/PipelineToneMapper.vert.spv.inl"
#include "ttauri/GUI/PipelineToneMapper.frag.spv.inl"

namespace tt {

/** Reference counter to determine the amount of startup/shutdowns.
*/
static std::atomic<uint64_t> startupCount = 0;



void gui_startup()
{
    if (startupCount.fetch_add(1) != 0) {
        // The library has already been initialized.
        return;
    }

    foundation_startup();
    text_startup();
    LOG_INFO("GUI startup");

    renderDoc = new RenderDoc();

    themeBook = new ThemeBook(std::vector<URL>{
        URL::urlFromResourceDirectory() / "themes"
    });

    themeBook->setThemeMode(readOSThemeMode());

    addStaticResource(PipelineImage_vert_spv_filename, PipelineImage_vert_spv_bytes);
    addStaticResource(PipelineImage_frag_spv_filename, PipelineImage_frag_spv_bytes);
    addStaticResource(PipelineFlat_vert_spv_filename, PipelineFlat_vert_spv_bytes);
    addStaticResource(PipelineFlat_frag_spv_filename, PipelineFlat_frag_spv_bytes);
    addStaticResource(PipelineBox_vert_spv_filename, PipelineBox_vert_spv_bytes);
    addStaticResource(PipelineBox_frag_spv_filename, PipelineBox_frag_spv_bytes);
    addStaticResource(PipelineSDF_vert_spv_filename, PipelineSDF_vert_spv_bytes);
    addStaticResource(PipelineSDF_frag_spv_filename, PipelineSDF_frag_spv_bytes);
    addStaticResource(PipelineToneMapper_vert_spv_filename, PipelineToneMapper_vert_spv_bytes);
    addStaticResource(PipelineToneMapper_frag_spv_filename, PipelineToneMapper_frag_spv_bytes);


    try {
        keyboardBindings.loadSystemBindings();
    } catch (error &e) {
        LOG_FATAL("Could not load keyboard bindings {}", to_string(e));
    }

    guiSystem = new GUISystem(guiDelegate);
}

void gui_shutdown()
{
    if (startupCount.fetch_sub(1) != 1) {
        // This is not the last instantiation.
        return;
    }
    LOG_INFO("GUI shutdown");

    delete guiSystem;
    delete themeBook;
    delete renderDoc;
    text_shutdown();
    foundation_shutdown();
}

}
