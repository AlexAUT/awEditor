#include "propertiesPanel.hpp"

#include "propertiesPanelEvents.hpp"

#include <aw/gui/widgets/label.hpp>
#include <aw/gui/widgets/textBox.hpp>
#include <aw/gui/widgets/typedTextBox.hpp>
#include <aw/utils/log.hpp>

using namespace aw::gui;

PropertiesPanel::PropertiesPanel(const GUI& gui, aw::MessageBus& bus, std::string name) :
    mGui(gui),
    mMsgBus(bus),
    mPanelName(std::move(name)),
    mPanel{std::make_shared<Panel>(gui)},
    mContainer{std::make_shared<LinearContainer>(gui, Orientation::Vertical)},
    mCurrentSubContainer{mContainer}
{
  mPanel->setChild(mContainer);
}

void PropertiesPanel::addGroup(std::string name)
{
  auto label = std::make_shared<Label>(mGui, name);

  mContainer->addChild(label);

  mCurrentSubContainer = std::make_shared<LinearContainer>(mGui, Orientation::Vertical);
  mCurrentSubContainer->setPadding(Padding{3.f, 3.f, 3.f, 15.f});
  mContainer->addChild(mCurrentSubContainer);

  mCurrentGroupName = std::move(name);
}

void PropertiesPanel::addProperty(std::string name, int value)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  container->addChild(label, 0.5);

  auto textBox = std::make_shared<TextBoxInt>(mGui, value);
  textBox->onTextChange = [this, name](auto& widget) {
    auto& box = static_cast<TextBoxInt&>(widget);
    if (box.isTextValid())
    {
      mMsgBus.broadcast<PropertyChangedEventBase>(
          IntPropertyChangedEvent{mPanelName, mCurrentGroupName, name, box.getValue()});
    }
  };
  container->addChild(textBox, 0.5);

  mCurrentSubContainer->addChild(container);
}

void PropertiesPanel::addProperty(std::string name, float value)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  container->addChild(label, 0.3);

  auto textBox = std::make_shared<TextBoxFloat>(mGui, value);
  textBox->onTextChange = [this, name](auto& widget) {
    auto& box = static_cast<TextBoxFloat&>(widget);
    if (box.isTextValid())
    {
      mMsgBus.broadcast<PropertyChangedEventBase>(
          FloatPropertyChangedEvent{mPanelName, mCurrentGroupName, name, box.getValue()});
    }
  };
  container->addChild(textBox, 0.7);

  mCurrentSubContainer->addChild(container);
}
