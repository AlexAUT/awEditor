#pragma once

#include <aw/gui/gui.hpp>
#include <aw/utils/math/vector.hpp>

class GUI
{
public:
  GUI(aw::Vec2 screenSize);

  void update(float delta);
  bool processEvent(const aw::WindowEvent& event);
  void render();

private:
private:
  aw::gui::GUI mGUI;
};
