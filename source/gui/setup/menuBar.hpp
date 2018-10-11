#pragma once

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>

namespace priv
{
void setupFileMenu(aw::gui::MenuItem::SPtr item);
}

void setupMenuBar(aw::gui::LinearContainer::SPtr& container)
{
  const auto& gui = container->getGUI();
  using namespace aw::gui;
  auto menu = std::make_shared<Menu>(gui);
  container->addChild(menu, 0);

  priv::setupFileMenu(menu->addMenuEntry("File"));
}

namespace priv
{
void setupFileMenu(aw::gui::MenuItem::SPtr item)
{
}
}
