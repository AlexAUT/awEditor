#pragma once

#include "../utils/rayTracer.hpp"

#include <aw/engine/windowEventForward.hpp>
#include <aw/runtime/entitySystem/entity.hpp>
#include <aw/utils/math/vector.hpp>

#include <array>

namespace aw
{
class Camera;
class Scene;
class Engine;
} // namespace aw

class ColMeshEvent;

class ViewportManager
{
public:
  ViewportManager(aw::Engine& engine, const aw::Scene& scene);
  bool processEvent(const aw::WindowEvent& event, const aw::Camera& camera);

  bool isConsumingEvents() const { return mDragging; }

private:
  void handleEvent(const ColMeshEvent& event);

  void resetAxisMask();
  void cancel();
  void confirm();

  void setPositionOfSelection(aw::Vec3 newPosition);
  void setScalingOfSelection(aw::Vec3 newScale);

  float getAnchorMouseDistanceScreenspace(const aw::Camera& camera);

private:
  aw::Engine& mEngine;
  RayTracer mRayTracer;

  std::array<bool, 3> mAxisMask{true, true, true};

  bool mDragging{false};
  aw::Vec3 mInitialPosition;
  aw::Vec2 mLastMousePos;

  bool mScaling{false};
  float mInitialDistance;
  aw::Vec3 mInitialScale;

  aw::ecs::Entity mSelectedEntity{};
};
