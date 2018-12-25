#include "../gui.hpp"

#include "../../components/collisionCubeId.hpp"
#include "../../events/collisionMeshEvent.hpp"
#include "../guiEvent.hpp"
#include "../modules/propertiesPanel.hpp"
#include "../modules/vec3Property.hpp"

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>
#include <aw/runtime/components/transform.hpp>
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

void updateTransformTab(aw::gui::List::SPtr cubeList, PropertiesPanel* panel, aw::ecs::Entity entity)
{
  auto t = entity.get<aw::ecs::components::Transform>();
  panel->getProperty<Vec3Property>("Position")->setValue(t->getPosition());
  panel->getProperty<Vec3Property>("Scale")->setValue(t->getScale());

  cubeList->selectItem(entity.get<CollisionCubeId>()->id);
}

} // namespace priv

void GUI::setupCollisionPanel(aw::gui::LinearContainer::SPtr container)
{
  using namespace aw::gui;
  auto tranformPanel = std::make_shared<aw::gui::Panel>(mGUI);
  tranformPanel->setPadding(aw::gui::Padding(10.f));
  container->addChild(tranformPanel, 0);
  auto layout = std::make_shared<aw::gui::LinearContainer>(mGUI, aw::gui::Orientation::Vertical);
  tranformPanel->setChild(layout);

  mCollisionPanel = std::make_unique<PropertiesPanel>(mGUI, mMsgBus, "CollisionTab");
  mCollisionPanel->addGroup("Transform");

  mCollisionPanel->addProperty<Vec3Property>("Position", aw::Vec3{0.f});
  mCollisionPanel->addProperty<Vec3Property>("Scale", aw::Vec3{1.f});
  // cubePanel.addProperty<Vec3Property>("Rotation", aw::Vec3{0.f});

  layout->addChild(mCollisionPanel->getRootPanel());

  auto addBtn = std::make_shared<aw::gui::Button>(mGUI, "Add rect");
  addBtn->onClick = [&](auto) { mMsgBus.broadcast<ColMeshEvent>(NewColMeshEvent()); };
  layout->addChild(addBtn, 0);

  auto cubeList = std::make_shared<List>(mGUI);
  cubeList->onChildSelect = [& messageBus = mMsgBus](aw::gui::ListItem& item) {
    SelectColMeshEvent event{item.getId()};
    messageBus.broadcast<ColMeshEvent>(event);
  };
  layout->addChild(cubeList);

  mMsgBus.subscribeToChannelUnsafe<ColMeshEvent>(
      [& messageBus = mMsgBus, cubeList, cubePanel = mCollisionPanel.get()](const ColMeshEvent& event) {
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
          priv::updateTransformTab(cubeList, cubePanel, e.entity);
        }
      });
}
