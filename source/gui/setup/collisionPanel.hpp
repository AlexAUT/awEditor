#pragma once

#include "../../components/collisionCubeId.hpp"
#include "../../events/collisionMeshEvent.hpp"
#include "../guiEvent.hpp"
#include "../modules/propertiesPanel.hpp"
#include "../modules/vec3Property.hpp"

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>
#include <aw/utils/log.hpp>
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
  label->setPadding({5.f, 0.f, 3.f, 0.f});
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

void updateTransformTab(const aw::ecs::Entity& entity) {}
} // namespace priv

void setupCollisionPanel(aw::MessageBus& messageBus, aw::gui::LinearContainer::SPtr& container)
{
  using namespace aw::gui;
  auto& gui = container->getGUI();
  auto tranformPanel = std::make_shared<aw::gui::Panel>(gui);
  tranformPanel->setPadding(aw::gui::Padding(10.f));
  container->addChild(tranformPanel, 0);
  auto layout = std::make_shared<aw::gui::LinearContainer>(gui, aw::gui::Orientation::Vertical);
  tranformPanel->setChild(layout);

  static PropertiesPanel cubePanel(gui, messageBus, "CollisionTab");
  cubePanel.addGroup("Transform");

  cubePanel.addProperty<Vec3Property>("Position", aw::Vec3{0.f});
  cubePanel.addProperty<Vec3Property>("Scale", aw::Vec3{1.f});
  // cubePanel.addProperty<Vec3Property>("Rotation", aw::Vec3{0.f});

  layout->addChild(cubePanel.getRootPanel());

  auto addBtn = std::make_shared<aw::gui::Button>(gui, "Add rect");
  addBtn->onClick = [&](auto) { messageBus.broadcast<ColMeshEvent>(NewColMeshEvent()); };
  layout->addChild(addBtn, 0);

  auto cubeList = std::make_shared<List>(gui);
  cubeList->onChildSelect = [&messageBus](aw::gui::ListItem& item) {
    SelectColMeshEvent event{item.getId()};
    messageBus.broadcast<ColMeshEvent>(event);
  };
  layout->addChild(cubeList);

  messageBus.subscribeToChannelUnsafe<ColMeshEvent>([&messageBus, cubeList](const ColMeshEvent& event) {
    if (event.type == ColMeshEventType::Created)
    {
      auto& e = static_cast<const CreatedColMeshEvent&>(event);
      auto id = e.entity.get<CollisionCubeId>()->id;
      auto displayName = "Cube" + id;

      cubeList->addItem(std::move(displayName), std::move(id));
      cubeList->getLastItem()->select();
    }
    else if (event.type == ColMeshEventType::Selected)
    {
      auto& e = static_cast<const SelectedColMeshEvent&>(event);
      priv::updateTransformTab(e.entity);
    }
  });
}
