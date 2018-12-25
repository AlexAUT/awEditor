#pragma once

#include <aw/gui/gui.hpp>
#include <aw/utils/math/vector.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include "modules/propertiesPanel.hpp"

#include <memory>

class GUI
{
public:
  GUI(aw::Vec2 screenSize, aw::MessageBus& bus);

  void update(float delta);
  bool processEvent(const aw::WindowEvent& event);
  void render();

  aw::MessageBus& getMessageBus() { return mMsgBus; }
  const aw::MessageBus& getMessageBus() const { return mMsgBus; }

private:
  void setupCollisionPanel(aw::gui::LinearContainer::SPtr container);

private:
  aw::gui::GUI mGUI;

  aw::MessageBus& mMsgBus;

  std::unique_ptr<PropertiesPanel> mCollisionPanel;
};
