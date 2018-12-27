#include "viewportManager.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/window.hpp>
#include <aw/engine/windowEvent.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/math/vector.hpp>
#include <aw/utils/spatial/ray.hpp>

#include "../components/collisionCubeId.hpp"
#include "../events/collisionMeshEvent.hpp"

ViewportManager::ViewportManager(aw::Engine& engine, aw::Scene& scene) : mEngine(engine), mRayTracer(scene)
{
  engine.getMessageBus().subscribeToChannelUnsafe<ColMeshEvent>([this](const auto& e) { handleEvent(e); });
}

bool ViewportManager::processEvent(const aw::WindowEvent& event, const aw::Camera& camera)
{
  using WindowEvent = aw::WindowEvent;
  if (event.type == WindowEvent::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      confirm();
    }
    else if (event.mouseButton.button == sf::Mouse::Right)
    {
      if (mDragging || mScaling)
      {
        cancel();
      }
      else
      {
        aw::Vec2 relMousePos = {event.mouseButton.x / static_cast<float>(mEngine.getWindow().getSize().x),
                                event.mouseButton.y / static_cast<float>(mEngine.getWindow().getSize().y)};
        // Intersect click ray with collision shapes
        auto ray = camera.createRayFromScreenspace(relMousePos);
        LogTemp() << ray;
        LogTemp() << "view: " << camera.getViewDirection();
        LogTemp() << "up: " << camera.getUpDirection();
        LogTemp() << "right: " << camera.getRightDirection();
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
  }
  else if (event.type == WindowEvent::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::G)
    {
      if (!mScaling && !mDragging && mSelectedEntity.isValid())
      {
        mDragging = true;
        auto mousePos = sf::Mouse::getPosition(mEngine.getWindow().getSFMLWindow());
        mLastMousePos = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
        mInitialPosition = mSelectedEntity.get<aw::ecs::components::Transform>()->getPosition();
        resetAxisMask();
      }
    }
    else if (event.key.code == sf::Keyboard::S)
    {
      if (!mScaling && !mDragging && mSelectedEntity.isValid())
      {
        mScaling = true;
        mInitialDistance = getAnchorMouseDistanceScreenspace(camera);
        mInitialScale = mSelectedEntity.get<aw::ecs::components::Transform>()->getScale();
        resetAxisMask();
      }
    }
    else if (event.key.code == sf::Keyboard::X)
    {
      mAxisMask[0] = true;
      mAxisMask[1] = false;
      mAxisMask[2] = false;
    }
    else if (event.key.code == sf::Keyboard::Y)
    {
      mAxisMask[0] = false;
      mAxisMask[1] = true;
      mAxisMask[2] = false;
    }
    else if (event.key.code == sf::Keyboard::Z)
    {
      mAxisMask[0] = false;
      mAxisMask[1] = false;
      mAxisMask[2] = true;
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      cancel();
    }
  }
  else if (event.type == WindowEvent::MouseMoved)
  {
    if (mDragging && mSelectedEntity.isValid())
    {
      aw::Vec2 oldRelPos = {mLastMousePos.x / static_cast<float>(mEngine.getWindow().getSize().x),
                            mLastMousePos.y / static_cast<float>(mEngine.getWindow().getSize().y)};
      auto mousePosI = sf::Mouse::getPosition(mEngine.getWindow().getSFMLWindow());
      LogTemp() << "Mouse moved: " << mousePosI.x << "," << mousePosI.y;
      mLastMousePos = {static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y)};
      aw::Vec2 newRelPos = {mLastMousePos.x / static_cast<float>(mEngine.getWindow().getSize().x),
                            mLastMousePos.y / static_cast<float>(mEngine.getWindow().getSize().y)};

      using Transform = aw::ecs::components::Transform;
      auto diff = mSelectedEntity.get<Transform>()->getPosition() - camera.getPosition();
      LogTemp() << "Diff: " << glm::length(newRelPos - oldRelPos) << (newRelPos - oldRelPos);

      auto v = camera.getViewDirection();
      auto normalDistanceToCamera = glm::dot(diff, camera.getViewDirection());
      auto oldPoint = camera.getPointInDistance(oldRelPos, normalDistanceToCamera);
      auto newPoint = camera.getPointInDistance(newRelPos, normalDistanceToCamera);
      auto movement = mSelectedEntity.get<Transform>()->getPosition() + (newPoint - oldPoint);

      setPositionOfSelection(movement);
    }
    else if (mScaling && mSelectedEntity.isValid())
    {
      auto newDistance = getAnchorMouseDistanceScreenspace(camera);
      auto scalingFactor = newDistance / mInitialDistance;
      auto newScaling = mInitialScale * scalingFactor;
      setScalingOfSelection(newScaling);
    }
  }
  return false;
}

void ViewportManager::handleEvent(const ColMeshEvent& event)
{
  if (event.type == ColMeshEventType::Selected)
  {
    auto& e = static_cast<const SelectedColMeshEvent&>(event);
    mSelectedEntity = e.entity;
  }
}

void ViewportManager::resetAxisMask()
{
  mAxisMask = {true, true, true};
}

void ViewportManager::cancel()
{
  if (mDragging)
  {
    // Reset the position
    mDragging = false;
    setPositionOfSelection(mInitialPosition);
  }
  if (mScaling)
  {
    mScaling = false;
    setScalingOfSelection(mInitialScale);
  }
}

void ViewportManager::confirm()
{
  if (mDragging)
  {
    mDragging = false;
  }
  else if (mScaling)
  {
    mScaling = false;
  }
}

void ViewportManager::setPositionOfSelection(aw::Vec3 newPosition)
{
  // Apply axis mask
  for (int i = 0; i < 3; i++)
    newPosition[i] = mAxisMask[i] ? newPosition[i] : mInitialPosition[i];

  using Transform = aw::ecs::components::Transform;
  mSelectedEntity.get<Transform>()->setPosition(newPosition);
  MovedSelectedColMesh event(newPosition);
  mEngine.getMessageBus().broadcast<ColMeshEvent>(event);
}

void ViewportManager::setScalingOfSelection(aw::Vec3 newScale)
{
  // Apply axis mask
  for (int i = 0; i < 3; i++)
    newScale[i] = mAxisMask[i] ? newScale[i] : mInitialScale[i];

  using Transform = aw::ecs::components::Transform;
  mSelectedEntity.get<Transform>()->setScale(newScale);
  ScaledSelectedColMesh event(newScale);
  mEngine.getMessageBus().broadcast<ColMeshEvent>(event);
}

float ViewportManager::getAnchorMouseDistanceScreenspace(const aw::Camera& camera)
{
  auto mousePosI = sf::Mouse::getPosition(mEngine.getWindow().getSFMLWindow());
  aw::Vec2 mousePos{static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y)};

  aw::Vec2 relMousePos = {mousePos.x / static_cast<float>(mEngine.getWindow().getSize().x),
                          mousePos.y / static_cast<float>(mEngine.getWindow().getSize().y)};

  using Transform = aw::ecs::components::Transform;
  auto t = mSelectedEntity.get<Transform>();

  aw::Vec4 relAnchorPointScreenspace = camera.getVPMatrix() * aw::Vec4(t->getPosition(), 1.f);
  aw::Vec2 res = aw::Vec2(relAnchorPointScreenspace) / relAnchorPointScreenspace.w;

  // Map [-1,1] to [0,1] and invert y axis
  res = (res + 1.f) / 2.f;
  res.y = glm::abs(res.y - 1.f);
  LogTemp() << "Anchor point: " << relAnchorPointScreenspace << res;
  return glm::distance(relMousePos, res);
  /*
    LogTemp() << "Mouse moved: " << mousePosI.x << "," << mousePosI.y;
    mLastMousePos = {static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y)};
    aw::Vec2 newRelPos = {mLastMousePos.x / static_cast<float>(mEngine.getWindow().getSize().x),
                          mLastMousePos.y / static_cast<float>(mEngine.getWindow().getSize().y)};

    using Transform = aw::ecs::components::Transform;
    auto diff = camera.getPosition() - mSelectedEntity.get<Transform>()->getPosition();
    LogTemp() << "Diff: " << glm::length(newRelPos - oldRelPos) << (newRelPos - oldRelPos);

    auto v = camera.getViewDirection();
    auto normalDistanceToCamera = glm::dot(diff, camera.getViewDirection());
    auto oldPoint = camera.getPointInDistance(oldRelPos, normalDistanceToCamera);
    auto newPoint = camera.getPointInDistance(newRelPos, normalDistanceToCamera);
    auto movement = mSelectedEntity.get<Transform>()->getPosition() + (newPoint - oldPoint);
    */
}
