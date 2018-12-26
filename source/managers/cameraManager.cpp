#include "cameraManager.hpp"

#include <aw/engine/windowEvent.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/math/constants.hpp>

#include <cmath>

using namespace aw::constantsF;

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
  else if (event.type == aw::WindowEvent::KeyPressed)
  {
    // Numpad rotate camera
    const auto rotationStep = PI_2 / 6.f;
    if (event.key.code == sf::Keyboard::Numpad2)
      rotateHorizontal(-rotationStep);
    else if (event.key.code == sf::Keyboard::Numpad8)
      rotateHorizontal(rotationStep);
    else if (event.key.code == sf::Keyboard::Numpad4)
      rotateVertical(-rotationStep);
    else if (event.key.code == sf::Keyboard::Numpad6)
      rotateVertical(rotationStep);

    // Numpad snap positions
    else if (event.key.code == sf::Keyboard::Numpad1)
    {
      setHorizontalRotation(0.f);
      setVerticalRotation(event.key.control ? PI : 0.f);
    }
    else if (event.key.code == sf::Keyboard::Numpad3)
    {
      setHorizontalRotation(0.f);
      setVerticalRotation(event.key.control ? -PI_2 : PI_2);
    }
    else if (event.key.code == sf::Keyboard::Numpad7)
    {
      setHorizontalRotation(event.key.control ? -PI_2 : PI_2);
      setVerticalRotation(0.f);
    }
    else if (event.key.code == sf::Keyboard::Numpad9)
    {
      setHorizontalRotation(event.key.control ? PI_2 : -PI_2);
      setVerticalRotation(0.f);
    }
    else if (event.key.code == sf::Keyboard::Numpad5)
    {
      auto* cam = getCamera();
      using ProjType = aw::Camera::ProjectionType;
      if (cam->getProjectionType() == ProjType::Perspective)
      {
        cam->setProjectionType(ProjType::Orthographic);
      }
      else if (cam->getProjectionType() == ProjType::Orthographic)
      {
        cam->setProjectionType(ProjType::Perspective);
      }
    }
  }
}

void CameraManager::updateZoom()
{
  float f = (mZoomState / 25.f);
  setDistanceToViewPoint(f * f * f);
}
