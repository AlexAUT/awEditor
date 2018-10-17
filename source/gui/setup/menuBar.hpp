#pragma once

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>

#include <aw/utils/log.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include "../guiEvent.hpp"

namespace priv
{
void setupFileMenu(const aw::MessageBus& bus, aw::gui::MenuItem::SPtr item);
}

void setupMenuBar(const aw::MessageBus& messageBus, aw::gui::LinearContainer::SPtr& container)
{
  const auto& gui = container->getGUI();
  using namespace aw::gui;
  auto menu = std::make_shared<Menu>(gui);
  menu->setConsumeEvent(true);
  container->addChild(menu, 0);

  priv::setupFileMenu(messageBus, menu->addMenuEntry("File"));
}

namespace priv
{
void setupFileMenu(const aw::MessageBus& bus, aw::gui::MenuItem::SPtr item)
{
  auto fileNew = item->addSubEntry("New");
  fileNew->onClick = [&bus](auto) { bus.broadcast(GuiEvent::NewLevel); };
  auto fileSave = item->addSubEntry("Save");
  fileSave->onClick = [&bus](auto) { bus.broadcast(GuiEvent::SaveLevel); };
  auto fileLoad = item->addSubEntry("Load");
  fileLoad->onClick = [&bus](auto) { bus.broadcast(GuiEvent::LoadLevel); };
}
} // namespace priv
