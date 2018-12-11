#include "propertiesPanel.hpp"

#include <aw/gui/widgets/label.hpp>
#include <aw/gui/widgets/textBox.hpp>
#include <aw/gui/widgets/typedTextBox.hpp>

using namespace aw::gui;

PropertiesPanel::PropertiesPanel(const GUI& gui) :
    mGui(gui),
    mPanel{std::make_shared<Panel>(gui)},
    mContainer{std::make_shared<LinearContainer>(gui, Orientation::Vertical)},
    mCurrentSubContainer{mContainer}
{
  mPanel->setChild(mContainer);
}

void PropertiesPanel::addGroup(std::string name)
{
  auto label = std::make_shared<Label>(mGui, name);

  mContainer->addChild(label);

  mCurrentSubContainer = std::make_shared<LinearContainer>(mGui, Orientation::Vertical);
  mCurrentSubContainer->setPadding(Padding{3.f, 3.f, 3.f, 15.f});
  mContainer->addChild(mCurrentSubContainer);
}

void PropertiesPanel::addProperty(std::string name, int value)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  container->addChild(label, 0.5);

  auto textBox = std::make_shared<TextBox>(mGui, std::to_string(value));
  container->addChild(textBox, 0.5);

  mCurrentSubContainer->addChild(container);
}

void PropertiesPanel::addProperty(std::string name, float value)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  container->addChild(label, 0.3);

  auto textBox = std::make_shared<TextBox>(mGui, std::to_string(value));
  container->addChild(textBox, 0.7);

  mCurrentSubContainer->addChild(container);
}

void PropertiesPanel::addProperty(std::string name, aw::Vec2 vec2)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  container->addChild(label, 0.3);

  auto textBox = std::make_shared<TextBox>(mGui, std::to_string(vec2.x));
  container->addChild(textBox, 0.35);

  textBox = std::make_shared<TextBox>(mGui, std::to_string(vec2.y));
  container->addChild(textBox, 0.35);

  mCurrentSubContainer->addChild(container);
}

void PropertiesPanel::addProperty(std::string name, aw::Vec3 vec3)
{
  auto container = std::make_shared<LinearContainer>(mGui, Orientation::Horizontal);

  auto label = std::make_shared<Label>(mGui, name);
  label->setPreferedSize({100.f, 0.f});
  container->addChild(label, 0.f);

  auto textBox = std::make_shared<TextBoxFloat>(mGui, vec3.x);
  container->addChild(textBox);

  textBox = std::make_shared<TextBoxFloat>(mGui, vec3.y);
  container->addChild(textBox);

  textBox = std::make_shared<TextBoxFloat>(mGui, vec3.z);
  container->addChild(textBox);

  mCurrentSubContainer->addChild(container);
}
