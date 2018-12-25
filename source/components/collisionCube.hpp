#pragma once

#include <aw/runtime/entitySystem/indirectComponentManager.hpp>
#include <aw/utils/math/vector.hpp>

struct CollisionCube
{
  using Manager = aw::ecs::IndirectComponentManager<CollisionCube>;

  aw::Vec3 center;
  aw::Vec3 size;
};
