#include "collisionCubeManager.hpp"

#include "../components/collisionCube.hpp"
#include "../components/collisionCubeId.hpp"

#include <aw/runtime/entitySystem/entitySystem.hpp>
#include <aw/runtime/entitySystem/unpackComponents.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/log.hpp>

#include <cassert>

CollisionCubeManager::CollisionCubeManager(aw::Scene& scene, aw::MessageBus& bus) :
    mScene{scene},
    mMsgBus{bus},
    mSubscription(bus.subscribeToChannel<ColMeshEvent>([this](const auto& e) { handleEvent(e); })),
    mSubProperties(
        bus.subscribeToChannel<PropertyChangedEventBase>([this](const auto& e) { handlePropertyChangeEvent(e); }))
{
}

void CollisionCubeManager::addCube()
{

  std::string id{std::to_string(++mCubeIdCounter)};
  mCollisionRects.emplace_back(mScene.getEntitySystem().createEntity());
  mCollisionRects.back().add<CollisionCube>(aw::AABB(1.f));
  mCollisionRects.back().add<CollisionCubeId>(id);
  mVersion++;

  CreatedColMeshEvent event{id, mCollisionRects.back()};
  mMsgBus.broadcast<ColMeshEvent>(event);
}

void CollisionCubeManager::changeCube(size_t index, aw::AABB cube)
{
  assert(index < mCollisionRects.size());
  assert(mCollisionRects[index].has<CollisionCube>());
  mCollisionRects[index].get<CollisionCube>()->aabb = cube;
  mVersion++;
}

aw::ecs::Entity CollisionCubeManager::getCubeById(std::string_view id) const
{
  for (auto [entityId, cubeId] : mScene.getEntitySystem().getView<CollisionCubeId>())
  {
    if (cubeId->id == id)
    {
      return mScene.getEntitySystem().getEntity(entityId);
    }
  }
  return {};
}

void CollisionCubeManager::handleEvent(const ColMeshEvent& event)
{
  if (event.type == ColMeshEventType::New)
  {
    addCube();
  }
  if (event.type == ColMeshEventType::Select)
  {
    auto& e = static_cast<const SelectColMeshEvent&>(event);
    auto entity = getCubeById(e.id);
    if (entity)
    {
      SelectedColMeshEvent newEvent{entity};
      mMsgBus.broadcast<ColMeshEvent>(newEvent);
    }
  }
}
void CollisionCubeManager::handlePropertyChangeEvent(const PropertyChangedEventBase& event)
{
  LogTemp() << "Got props event: " << event.panel << ", " << event.group << ", " << event.property;
}
