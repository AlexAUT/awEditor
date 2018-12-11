#pragma once

#include <aw/gui/widgets/linearContainer.hpp>
#include <aw/gui/widgets/panel.hpp>
#include <aw/utils/math/vector.hpp>

namespace aw::gui
{
class GUI;
}

class PropertiesPanel
{
public:
  PropertiesPanel(const aw::gui::GUI& gui);

  aw::gui::Panel::SPtr getRootPanel() const { return mPanel; }

  void addGroup(std::string name);

  void addProperty(std::string name, int value);
  void addProperty(std::string name, float value);

  void addProperty(std::string name, aw::Vec2 vec2);
  void addProperty(std::string name, aw::Vec3 vec3);

private:
private:
  const aw::gui::GUI& mGui;
  aw::gui::Panel::SPtr mPanel;
  aw::gui::LinearContainer::SPtr mContainer;
  aw::gui::LinearContainer::SPtr mCurrentSubContainer;
};
