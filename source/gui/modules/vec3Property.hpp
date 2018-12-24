#pragma once

#include "property.hpp"

#include <aw/gui/widgets/typedTextBox.hpp>
#include <aw/utils/math/vector.hpp>

#include <array>

class PropertiesPanel;

class Vec3Property : public Property
{
public:
  Vec3Property(PropertiesPanel& panel, std::string name, aw::Vec3 initialValue = aw::Vec3{0.f});

  void setValue(aw::Vec3 value);
  aw::Vec3 getValue() const { return mValue; }

  void addToPropertyPanel(const PropertiesPanel& panel);

private:
private:
  aw::Vec3 mValue;

  std::array<aw::gui::TextBoxFloat::SPtr, 3> mInputFields{nullptr};
};
