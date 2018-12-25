#include "viewportManager.hpp"

#include <aw/engine/window.hpp>
#include <aw/engine/windowEvent.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/math/vector.hpp>
#include <aw/utils/spatial/ray.hpp>

ViewportManager::ViewportManager(const aw::Window& window, const aw::Scene& scene) : mWindow(window), mRayTracer(scene)
{
}

bool ViewportManager::processEvent(const aw::WindowEvent& event, const aw::Camera& camera)
{
  using WindowEvent = aw::WindowEvent;
  if (event.type == WindowEvent::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Right)
    {
      aw::Vec2 relMousePos = {event.mouseButton.x / static_cast<float>(mWindow.getSize().x),
                              event.mouseButton.y / static_cast<float>(mWindow.getSize().y)};
      // Intersect click ray with collision shapes
      auto ray = camera.createRayFromScreenspace(relMousePos);
      auto hitEntity = mRayTracer.trace(ray);
      if (hitEntity.isValid())
      {
        LogTemp() << "Rect clicked!";
      }
    }
  }
  return false;
}
