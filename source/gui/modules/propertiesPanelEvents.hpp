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
  std::string_view panel;
  std::string_view group;
  std::string_view property;
  PropertyType type;
};

template <typename Type, PropertyType type>
struct PropertyChangedEvent : public PropertyChangedEventBase
{
  Type newValue;
  PropertyChangedEvent(std::string_view panel, std::string_view group, std::string_view property, Type newValue) :
      PropertyChangedEventBase{panel, group, property, type},
      newValue(newValue)
  {
  }
};

using IntPropertyChangedEvent = PropertyChangedEvent<int, PropertyType::Int>;
using FloatPropertyChangedEvent = PropertyChangedEvent<float, PropertyType::Float>;
using Vec2PropertyChangedEvent = PropertyChangedEvent<aw::Vec2, PropertyType::Vec2>;
using Vec3PropertyChangedEvent = PropertyChangedEvent<aw::Vec3, PropertyType::Vec3>;
