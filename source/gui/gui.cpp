#include "gui.hpp"

#include "setup/menuBar.hpp"

#include <aw/gui/style/defaultStyles.hpp>
#include <aw/gui/widgets/linearContainer.hpp>

GUI::GUI(aw::Vec2 screenSize) : mGUI(screenSize)
{
  applyDefaultStyles(mGUI);

  auto screen = mGUI.addScreen();
  using namespace aw::gui;
  auto container = std::make_shared<LinearContainer>(mGUI, Orientation::Vertical);
  setupMenuBar(container);
  screen->setChild(container);
}

void GUI::update(float delta)
{
  mGUI.update(delta);
}

bool GUI::processEvent(const aw::WindowEvent& event)
{
  return mGUI.processEvent(event);
}

void GUI::render()
{
  mGUI.render();
}
