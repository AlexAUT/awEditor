#pragma once

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include <memory>

void setupCollisionPanel(const aw::MessageBus& messageBus, aw::gui::LinearContainer::SPtr& container)
{
  auto& gui = container->getGUI();
  auto panel = std::make_shared<aw::gui::Panel>(gui);
  panel->setPadding(aw::gui::Padding(10.f));
  container->addChild(panel, 0);
  auto layout = std::make_shared<aw::gui::LinearContainer>(gui, aw::gui::Orientation::Vertical);
  panel->setChild(layout);

  auto addBtn = std::make_shared<aw::gui::Button>(gui, "Add rect");
  layout->addChild(addBtn, 0);
}
