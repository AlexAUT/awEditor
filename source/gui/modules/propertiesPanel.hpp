#pragma once

#include "property.hpp"

#include <aw/gui/widgets/linearContainer.hpp>
#include <aw/gui/widgets/panel.hpp>
#include <aw/utils/math/vector.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

namespace aw::gui
{
class GUI;
}

class PropertiesPanel
{
public:
  PropertiesPanel(const aw::gui::GUI& gui, aw::MessageBus& bus, std::string name);

  const aw::gui::GUI& getGUI() const { return mGui; }
  aw::MessageBus& getMessageBus() const { return mMsgBus; }
  aw::gui::Panel::SPtr getRootPanel() const { return mPanel; }
  aw::gui::LinearContainer::SPtr getCurrentSubContainer() const { return mCurrentSubContainer; }

  std::string_view getName() const { return mPanelName; }
  std::string_view getCurrentGroupName() const { return mCurrentGroupName; }

  void addGroup(std::string name);

  void addProperty(std::string name, int value);
  void addProperty(std::string name, float value);

  void addProperty(std::string name, aw::Vec2 vec2);

  template <typename Property, typename... Args>
  void addProperty(std::string name, Args&&... args);

  template <typename Property>
  Property* getProperty(std::string_view name);

  template <typename Property>
  const Property* getProperty(std::string_view name) const;

private:
private:
  const aw::gui::GUI& mGui;
  aw::MessageBus& mMsgBus;
  std::string mPanelName;
  std::string mCurrentGroupName{};

  aw::gui::Panel::SPtr mPanel;
  aw::gui::LinearContainer::SPtr mContainer;
  aw::gui::LinearContainer::SPtr mCurrentSubContainer;

  std::vector<Property::UPtr> mProperties;
};

template <typename Property, typename... Args>
void PropertiesPanel::addProperty(std::string name, Args&&... args)
{
  mProperties.push_back(std::make_unique<Property>(*this, name, std::forward<Args...>(args...)));
}

template <typename Property>
Property* PropertiesPanel::getProperty(std::string_view name)
{
  return const_cast<Property*>(static_cast<const PropertiesPanel*>(this)->getProperty<Property>(name));
}

template <typename Property>
const Property* PropertiesPanel::getProperty(std::string_view name) const
{
  for (auto& prop : mProperties)
  {
    if (prop->getName() == name)
      return static_cast<const Property*>(prop.get());
  }
  return nullptr;
}
