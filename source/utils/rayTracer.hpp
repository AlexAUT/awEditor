#pragma once

#include <aw/utils/spatial/ray.hpp>

#include <aw/runtime/entitySystem/entity.hpp>

namespace aw
{
class Scene;
}

class RayTracer
{
public:
  RayTracer(const aw::Scene& scene);

  aw::ecs::Entity trace(aw::Ray ray);

private:
private:
  const aw::Scene& mScene;
};
