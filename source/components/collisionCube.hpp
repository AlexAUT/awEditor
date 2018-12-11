#pragma once

#include <aw/runtime/entitySystem/indirectComponentManager.hpp>
#include <aw/utils/spatial/AABB.hpp>

struct CollisionCube
{
  using Manager = aw::ecs::IndirectComponentManager<CollisionCube>;

  aw::AABB aabb;
};
