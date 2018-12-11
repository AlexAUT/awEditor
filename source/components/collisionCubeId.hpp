#pragma once

#include <aw/runtime/entitySystem/indirectComponentManager.hpp>

#include <string>

struct CollisionCubeId
{
  using Manager = aw::ecs::IndirectComponentManager<CollisionCubeId>;

  std::string id;
};
