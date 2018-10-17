#pragma once

#include <aw/gui/gui.hpp>
#include <aw/utils/math/vector.hpp>

#include <aw/utils/messageBus/messageBus.hpp>

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
private:
  aw::gui::GUI mGUI;

  aw::MessageBus& mMsgBus;
};
