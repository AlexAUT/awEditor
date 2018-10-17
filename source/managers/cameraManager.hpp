#pragma once

#include <aw/engine/windowEventForward.hpp>
#include <aw/graphics/3d/orbitCameraController.hpp>

#include <aw/utils/math/vector.hpp>

class CameraManager : public aw::OrbitCameraController
{
public:
  CameraManager(aw::Camera* mCamera);

  void processEvent(const aw::WindowEvent& event);

  bool isSelected() const { return mRotating || mDragging; }

private:
  void updateZoom();

private:
  bool mRotating{false};
  bool mDragging{false};

  aw::Vec2 mLastMousePosition{0.f};

  float mRotateSensibility{3.14f / 640.f};
  float mZoomSensibility{0.1f};
  int mZoomState{100};
};
