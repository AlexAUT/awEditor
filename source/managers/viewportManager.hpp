#pragma once

#include "../utils/rayTracer.hpp"

#include <aw/engine/windowEventForward.hpp>

namespace aw
{
class Camera;
class Scene;
class Window;
} // namespace aw

class ViewportManager
{
public:
  ViewportManager(const aw::Window& window, const aw::Scene& scene);
  bool processEvent(const aw::WindowEvent& event, const aw::Camera& camera);

private:
private:
  const aw::Window& mWindow;
  RayTracer mRayTracer;
};
