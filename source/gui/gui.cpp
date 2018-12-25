#include "gui.hpp"

#include "setup/menuBar.hpp"
#include "setup/modals.hpp"

#include <aw/gui/style/defaultStyles.hpp>
#include <aw/gui/widgets/linearContainer.hpp>

GUI::GUI(aw::Vec2 screenSize, aw::MessageBus& bus) : mGUI(screenSize), mMsgBus(bus)
{
  applyDefaultStyles(mGUI);

  auto screen = mGUI.addScreen();
  using namespace aw::gui;
  auto container = std::make_shared<LinearContainer>(mGUI, Orientation::Vertical);
  setupMenuBar(mMsgBus, container);
  screen->setChild(container);

  auto layoutSideBar = std::make_shared<LinearContainer>(mGUI, Orientation::Horizontal);
  container->addChild(layoutSideBar, 0);
  setupCollisionPanel(layoutSideBar);

  setupModals(mMsgBus);
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
