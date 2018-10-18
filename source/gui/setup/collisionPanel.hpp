#pragma once

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include <memory>

namespace priv
{
void addVector3(aw::gui::LinearContainer::SPtr& container, std::string title)
{
  using namespace aw::gui;
  auto& gui = container->getGUI();

  auto label = std::make_shared<Label>(gui, std::move(title));
  label->setAlignmentH(AlignmentH::Center);
  label->setPadding({5.f, 0.f, 2.f, 0.f});
  container->addChild(label, 0);

  auto subContainer = std::make_shared<LinearContainer>(gui, Orientation::Horizontal);
  subContainer->setSpaceBetweenElements(3.f);
  container->addChild(subContainer, 0);
  auto x = std::make_shared<TextBox>(gui, "0.0");
  subContainer->addChild(x);
  auto y = std::make_shared<TextBox>(gui, "0.0");
  subContainer->addChild(y);
  auto z = std::make_shared<TextBox>(gui, "0.0");
  subContainer->addChild(z);
}
} // namespace priv

void setupCollisionPanel(const aw::MessageBus& messageBus, aw::gui::LinearContainer::SPtr& container)
{
  using namespace aw::gui;
  auto& gui = container->getGUI();
  auto panel = std::make_shared<aw::gui::Panel>(gui);
  panel->setPadding(aw::gui::Padding(10.f));
  container->addChild(panel, 0);
  auto layout = std::make_shared<aw::gui::LinearContainer>(gui, aw::gui::Orientation::Vertical);
  panel->setChild(layout);

  auto addBtn = std::make_shared<aw::gui::Button>(gui, "Add rect");
  layout->addChild(addBtn, 0);

  priv::addVector3(layout, "Position");
  priv::addVector3(layout, "Scale");
  priv::addVector3(layout, "Rotation");
}
