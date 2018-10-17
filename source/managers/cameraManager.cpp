#include "cameraManager.hpp"

#include <aw/engine/windowEvent.hpp>

#include <aw/utils/log.hpp>

#include <cmath>

CameraManager::CameraManager(aw::Camera* camera) : aw::OrbitCameraController(camera)
{
  updateZoom();
}

void CameraManager::processEvent(const aw::WindowEvent& event)
{
  if (event.type == aw::WindowEvent::MouseMoved)
  {
    if (mRotating)
    {
      auto delta = aw::Vec2{event.mouseMove.x, event.mouseMove.y} - mLastMousePosition;
      mLastMousePosition += delta;
      rotateVertical(-mRotateSensibility * delta.x);
      rotateHorizontal(mRotateSensibility * delta.y);
    }
  }
  else if (event.type == aw::WindowEvent::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Middle)
    {
      mRotating = true;
      mLastMousePosition = aw::Vec2{event.mouseButton.x, event.mouseButton.y};
    }
  }
  else if (event.type == aw::WindowEvent::MouseButtonReleased)
  {
    if (event.mouseButton.button == sf::Mouse::Middle)
      mRotating = false;
  }
  else if (event.type == aw::WindowEvent::MouseWheelScrolled)
  {
    mZoomState -= static_cast<int>(event.mouseWheelScroll.delta);
    mZoomState = std::max(0, mZoomState);
    updateZoom();
  }
}

void CameraManager::updateZoom()
{
  float f = (mZoomState / 25.f);
  setDistanceToViewPoint(f * f * f);
}
