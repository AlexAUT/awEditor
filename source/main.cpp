#include <iostream>

#include <aw/engine/engine.hpp>
#include <aw/reflection/operations/print.hpp>
#include <aw/reflection/reflection.hpp>
#include <aw/reflection/typeDescriptorClass.hpp>
#include <aw/utils/log.hpp>

#include "editorState.hpp"

#include <aw/runtime/resourceManager/resourceManager.hpp>

struct Vec
{
  Vec(int x, int y, int z) : x(x), y(y), z(z) {}
  int x, y, z;
};

int main()
{
  aw::LOG_INITIALIZE();

  aw::Engine engine;
  auto startState = std::make_shared<EditorState>(engine);
  engine.getStateMachine().pushState(startState);

  return engine.run();
}
