#include "viewportManager.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/window.hpp>
#include <aw/engine/windowEvent.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/math/vector.hpp>
#include <aw/utils/spatial/ray.hpp>

#include "../components/collisionCubeId.hpp"
#include "../events/collisionMeshEvent.hpp"

ViewportManager::ViewportManager(const aw::Engine& engine, const aw::Scene& scene) : mEngine(engine), mRayTracer(scene)
{
}

bool ViewportManager::processEvent(const aw::WindowEvent& event, const aw::Camera& camera)
{
  using WindowEvent = aw::WindowEvent;
  if (event.type == WindowEvent::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Right)
    {
      aw::Vec2 relMousePos = {event.mouseButton.x / static_cast<float>(mEngine.getWindow().getSize().x),
                              event.mouseButton.y / static_cast<float>(mEngine.getWindow().getSize().y)};
      // Intersect click ray with collision shapes
      auto ray = camera.createRayFromScreenspace(relMousePos);
      auto hitEntity = mRayTracer.trace(ray);
      if (hitEntity.isValid())
      {
        std::string_view id = hitEntity.get<CollisionCubeId>()->id;
        SelectColMeshEvent event{id};
        mEngine.getMessageBus().broadcast<ColMeshEvent>(event);
        LogTemp() << "Rect clicked!";
      }
    }
  }
  return false;
}
