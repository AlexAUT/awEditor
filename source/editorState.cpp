#include "editorState.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/window.hpp>
#include <aw/utils/colors.hpp>

#include <aw/gui/style/defaultStyles.hpp>
#include <aw/gui/widgets/widgets.hpp>

#include <aw/utils/file/fileDialog.hpp>
#include <aw/utils/log.hpp>
DEFINE_LOG_CATEGORY(EditorD, aw::log::Debug, "Editor")

#include <cassert>

aw::gui::Widget::SPtr window;

EditorState::EditorState(aw::Engine& engine)
    : aw::State(engine.getStateMachine()), mEngine(engine), mGUI(engine.getWindow().getSize())
{
  mListenerId = mEngine.getWindow().registerEventListener([this](auto e) { this->processEvent(e); });
  mEngine.getWindow().setClearColor(aw::Colors::DARKRED);
}

EditorState::~EditorState()
{
  mEngine.getWindow().unregisterEventListener(mListenerId);
}

void EditorState::update(float delta)
{
  static float elapsedTime = 0.f;
  elapsedTime += delta;
  if (elapsedTime > 3.f)
  {
    elapsedTime -= 3.f;
    // window->setSize(window->getSize() * 1.1f);
  }
  mGUI.update(delta);
}

void EditorState::render()
{
  mEngine.getWindow().clear();
  mGUI.render();
}

void EditorState::processEvent(const aw::WindowEvent& event)
{
  auto eventConsumed = mGUI.processEvent(event);
  if (eventConsumed)
    return;

  if (event.type == aw::WindowEvent::Closed)
    mEngine.terminate();

  if (event.type == aw::WindowEvent::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Escape)
      mEngine.terminate();
    if (event.key.code == sf::Keyboard::U)
      window->setSize(window->getSize() * 1.1f);
    if (event.key.code == sf::Keyboard::I)
      window->setSize(window->getSize() * 0.9f);
  }
}
