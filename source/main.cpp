#include <iostream>

#include <aw/engine/engine.hpp>
#include <aw/utils/log.hpp>

#include "editorState.hpp"

#include <aw/utils/messageBus/messageBus.hpp>

int main()
{
  aw::LOG_INITIALIZE();

  aw::Engine engine;
  auto startState = std::make_shared<EditorState>(engine);
  engine.getStateMachine().pushState(startState);

  return engine.run();
}
