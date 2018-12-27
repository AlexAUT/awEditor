#include "rayTracer.hpp"

#include "../components/collisionCube.hpp"

#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/spatial/intersectors/rayIntersectors.hpp>

#include <aw/utils/log.hpp>

RayTracer::RayTracer(aw::Scene& scene) : mScene(scene) {}

aw::ecs::Entity RayTracer::trace(aw::Ray ray)
{
  using namespace aw::ecs::components;
  for (auto [id, transform, cube] : mScene.getEntitySystem().getView<Transform, CollisionCube>())
  {
    auto t = transform->getTransform();
    auto pos = t * aw::Vec4(cube->center, 1.f);
    auto size = t * aw::Vec4(cube->size, 0.f);
    aw::AABB box(pos, size);

    if (aw::RayAABBIntersector()(ray, box))
    {
      return mScene.getEntitySystem().getEntity(id);
    }
  }
  return {};
}
