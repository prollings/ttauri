// Copyright 2019 Pokitec
// All rights reserved.

#include "TTauri/GUI/WindowToolbarWidget.hpp"
#include "TTauri/GUI/WindowTrafficLightsWidget.hpp"
#include "TTauri/GUI/ToolbarButtonWidget.hpp"
#include "TTauri/GUI/utils.hpp"
#include <cmath>

namespace TTauri::GUI::Widgets {

using namespace std;

WindowToolbarWidget::WindowToolbarWidget() noexcept :
    Widget()
{
}

void WindowToolbarWidget::setParent(Widget *parent) noexcept
{
    Widget::setParent(parent);

    trafficLightButtons = addWidget<WindowTrafficLightsWidget>(
        getResource<Path>(URL("resource:Themes/Icons/Application Icon.tticon"))
    );
    window->addConstraint(trafficLightButtons->box.top == box.top);
    window->addConstraint(trafficLightButtons->box.left == box.left);
    window->addConstraint(trafficLightButtons->box.bottom == box.bottom);

    if constexpr (operatingSystem == OperatingSystem::Windows) {
        let scale = mat::S(0.33f, 0.33f);

        closeWindowButton = addWidget<ToolbarButtonWidget>(
            scale * getResource<Path>(URL("resource:Themes/Icons/Close%20Window.tticon")),
            [&]() { window->closeWindow(); }
        );
        closeWindowButton->hoverBackgroundColor = vec{ 0.5, 0.0, 0.0, 1.0 };
        closeWindowButton->pressedBackgroundColor = vec{ 1.0, 0.0, 0.0, 1.0 };
        window->addConstraint(closeWindowButton->box.top == box.top);
        window->addConstraint(closeWindowButton->box.right == box.right);
        window->addConstraint(closeWindowButton->box.bottom == box.bottom);

        maximizeWindowButton = addWidget<ToolbarButtonWidget>(
            scale * getResource<Path>(URL("resource:Themes/Icons/Maximize%20Window.tticon")),
            [&]() { 
                switch (window->size) {
                case Window::Size::Normal:
                    window->maximizeWindow();
                    break;
                case Window::Size::Maximized:
                    window->normalizeWindow();
                    break;
                default:
                    no_default;
                }
            }
        );
        window->addConstraint(maximizeWindowButton->box.top == box.top);
        window->addConstraint(maximizeWindowButton->box.right == closeWindowButton->box.left);
        window->addConstraint(maximizeWindowButton->box.bottom == box.bottom);

        minimizeWindowButton = addWidget<ToolbarButtonWidget>(
            scale * getResource<Path>(URL("resource:Themes/Icons/Minimize%20Window.tticon")),
            //getResource<Path>(URL("resource:Themes/Icons/MultiColor.tticon")),
            [&]() { window->minimizeWindow(); }
        );
        window->addConstraint(minimizeWindowButton->box.top == box.top);
        window->addConstraint(minimizeWindowButton->box.right == maximizeWindowButton->box.left);
        window->addConstraint(minimizeWindowButton->box.bottom == box.bottom);
    }
}

bool WindowToolbarWidget::updateAndPlaceVertices(
    bool modified,
    vspan<PipelineFlat::Vertex> &flat_vertices,
    vspan<PipelineBox::Vertex> &box_vertices,
    vspan<PipelineImage::Vertex> &image_vertices,
    vspan<PipelineSDF::Vertex> &sdf_vertices) noexcept
{
    auto continueRendering = false;
    PipelineFlat::DeviceShared::placeVerticesBox(flat_vertices, box.currentRectangle(), backgroundColor, box.currentRectangle(), depth);

    continueRendering |= Widget::updateAndPlaceVertices(modified, flat_vertices, box_vertices, image_vertices, sdf_vertices);
    return continueRendering;
}

HitBox WindowToolbarWidget::hitBoxTest(vec position) noexcept
{
    auto r = box.contains(position) ?
        HitBox{this, depth, HitBox::Type::MoveArea} :
        HitBox{};

    r = std::max(r, trafficLightButtons->hitBoxTest(position));

    r = std::max(r, closeWindowButton->hitBoxTest(position));
    r = std::max(r, maximizeWindowButton->hitBoxTest(position));
    r = std::max(r, minimizeWindowButton->hitBoxTest(position));

    for (auto& widget : children) {
        r = std::max(r, widget->hitBoxTest(position));
    }
    return r;
}

}
