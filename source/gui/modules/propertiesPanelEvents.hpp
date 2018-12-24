#pragma once

#include <aw/utils/math/vector.hpp>

#include <string>

enum class PropertyType
{
  Int,
  Float,
  Vec2,
  Vec3
};

struct PropertyChangedEventBase
{
  PropertyType type;
  std::string_view panel;
  std::string_view group;
  std::string_view property;
};

template <typename Type, PropertyType eventType>
struct PropertyChangedEvent : public PropertyChangedEventBase
{
  PropertyChangedEvent(std::string_view panel, std::string_view group, std::string_view property, Type newValue) :
      PropertyChangedEventBase{eventType, panel, group, property},
      newValue(newValue)
  {
  }
  Type newValue;
};

using IntPropertyChangedEvent = PropertyChangedEvent<int, PropertyType::Int>;
using FloatPropertyChangedEvent = PropertyChangedEvent<float, PropertyType::Float>;
using Vec2PropertyChangedEvent = PropertyChangedEvent<aw::Vec2, PropertyType::Vec2>;
using Vec3PropertyChangedEvent = PropertyChangedEvent<aw::Vec3, PropertyType::Vec3>;
