#pragma once

#include <aw/engine/runtime/state.hpp>
#include <aw/engine/window.hpp>

#include "gui/gui.hpp"

namespace aw
{
class Engine;
} // namespace aw

class EditorState : public aw::State
{
public:
  EditorState(aw::Engine& engine);
  ~EditorState();

  void update(float delta) override;
  void render() override;

  void processEvent(const aw::WindowEvent& event);

private:
  aw::Engine& mEngine;
  unsigned mListenerId;

  GUI mGUI;
};
