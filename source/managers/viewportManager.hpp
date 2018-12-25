#pragma once

#include "../utils/rayTracer.hpp"

#include <aw/engine/windowEventForward.hpp>

namespace aw
{
class Camera;
class Scene;
class Engine;
} // namespace aw

class ViewportManager
{
public:
  ViewportManager(const aw::Engine& engine, const aw::Scene& scene);
  bool processEvent(const aw::WindowEvent& event, const aw::Camera& camera);

private:
private:
  const aw::Engine& mEngine;
  RayTracer mRayTracer;
};
