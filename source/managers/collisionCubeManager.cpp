#include "collisionCubeManager.hpp"

#include "../components/collisionCube.hpp"
#include "../components/collisionCubeId.hpp"

#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/entitySystem/entitySystem.hpp>
#include <aw/runtime/entitySystem/unpackComponents.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/math/constants.hpp>

#include <cassert>

using namespace aw::constantsF;
using Transform = aw::ecs::components::Transform;

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
  auto& newCube = mCollisionCubes.emplace_back(mScene.getEntitySystem().createEntity());
  newCube.add<CollisionCube>(aw::Vec3{0.f}, aw::Vec3{1.f});
  newCube.add<Transform>();
  newCube.add<CollisionCubeId>(std::move(id));
  mVersion++;

  CreatedColMeshEvent event{newCube};
  mMsgBus.broadcast<ColMeshEvent>(event);
}

void CollisionCubeManager::changeCube(size_t index, aw::Vec3 center, aw::Vec3 size)
{
  assert(index < mCollisionCubes.size());
  assert(mCollisionCubes[index].has<CollisionCube>());
  auto cube = mCollisionCubes[index].get<CollisionCube>();
  cube->center = center;
  cube->size = size;
  mVersion++;
}

void CollisionCubeManager::selectCube(std::string_view id)
{
  mSelectedCube = getCubeById(id);

  if (mSelectedCube.isValid())
  {
    SelectedColMeshEvent newEvent{mSelectedCube};
    mMsgBus.broadcast<ColMeshEvent>(newEvent);
  }
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
    selectCube(e.id);
  }
}
void CollisionCubeManager::handlePropertyChangeEvent(const PropertyChangedEventBase& event)
{
  if (event.panel == "CollisionTab")
  {
    if (event.group == "Transform")
    {
      if (!mSelectedCube.isValid())
        return;

      auto& vec3Event = static_cast<const Vec3PropertyChangedEvent&>(event);
      if (event.property == "Position")
      {
        assert(event.type == PropertyType::Vec3);
        mSelectedCube.get<Transform>()->setPosition(vec3Event.newValue);
      }
      if (event.property == "Scale")
      {
        assert(event.type == PropertyType::Vec3);
        mSelectedCube.get<Transform>()->setScale(vec3Event.newValue);
      }
      if (event.property == "Rotation")
      {
        assert(event.type == PropertyType::Vec3);
        mSelectedCube.get<Transform>()->setRotation(aw::Quaternion(TO_RAD * vec3Event.newValue));
      }
    }
  }
}
