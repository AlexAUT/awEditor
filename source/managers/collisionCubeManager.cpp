#include "collisionCubeManager.hpp"

#include <aw/utils/log.hpp>

#include <cassert>

CollisionCubeManager::CollisionCubeManager(aw::MessageBus& bus)
    : mSubscription(bus.subscribeToChannel<ColMeshEvent>([this](const auto& e) { handleEvent(e); }))
{
}

void CollisionCubeManager::addRect(aw::AABB rect)
{
  mCollisionRects.push_back(rect);
  mVersion++;
}

void CollisionCubeManager::changeRect(size_t index, aw::AABB rect)
{
  assert(index < mCollisionRects.size());
  mCollisionRects[index] = rect;
  mVersion++;
}

void CollisionCubeManager::handleEvent(const ColMeshEvent& event)
{
  if (event.type == ColMeshEventType::New)
  {
    addRect(aw::AABB(aw::Vec3(1.f)));
  }
}
