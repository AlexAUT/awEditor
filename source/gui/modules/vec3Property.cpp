#include "vec3Property.hpp"

#include "propertiesPanel.hpp"
#include "propertiesPanelEvents.hpp"

#include <aw/gui/widgets/label.hpp>
#include <aw/gui/widgets/linearContainer.hpp>
#include <aw/utils/log.hpp>

Vec3Property::Vec3Property(PropertiesPanel& panel, std::string name, aw::Vec3 initialValue) :
    Property(panel, name),
    mValue(initialValue)
{
  using namespace aw::gui;

  const auto& gui = panel.getGUI();
  auto container = std::make_shared<LinearContainer>(gui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(gui, std::string(getName()));
  label->setPreferedSize({100.f, 0.f});
  container->addChild(label, 3.f);

  auto addInputField = [this, &gui, &container](auto& inputField, auto& value) {
    inputField = std::make_shared<TextBoxFloat>(gui, value);
    container->addChild(inputField, 100.f);

    // Change callback
    inputField->onTextChange = [this, &value](auto& widget) {
      auto& box = static_cast<TextBoxFloat&>(widget);
      if (box.isTextValid())
      {
        value = box.getValue();
        getPanel().getMessageBus().broadcast<PropertyChangedEventBase>(
            Vec3PropertyChangedEvent{getPanel().getName(), getPanel().getCurrentGroupName(), getName(), getValue()});
      }
    };
  };

  addInputField(mInputFields[0], mValue.x);
  addInputField(mInputFields[1], mValue.y);
  addInputField(mInputFields[2], mValue.z);

  panel.getCurrentSubContainer()->addChild(container);
}
