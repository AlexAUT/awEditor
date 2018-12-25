#pragma once

#include <vector>

#include <aw/runtime/entitySystem/entity.hpp>
#include <aw/utils/messageBus/messageBus.hpp>
#include <aw/utils/spatial/AABB.hpp>

#include "../events/collisionMeshEvent.hpp"
#include "../gui/modules/propertiesPanelEvents.hpp"

namespace aw
{
class Scene;
}

class CollisionCubeManager
{
public:
  using CubeContainer = std::vector<aw::ecs::Entity>;

public:
  CollisionCubeManager(aw::Scene& scene, aw::MessageBus& bus);

  void addCube();
  void changeCube(size_t index, aw::Vec3 center, aw::Vec3 size);

  void selectCube(std::string_view id);

  size_t getCubeCount() const { return mCollisionCubes.size(); }
  aw::ecs::Entity getCubeById(std::string_view id) const;

  CubeContainer::const_iterator begin() const { return mCollisionCubes.cbegin(); }
  CubeContainer::const_iterator end() const { return mCollisionCubes.cend(); };

  int getVersion() const { return mVersion; }

private:
  void handleEvent(const ColMeshEvent& event);
  void handlePropertyChangeEvent(const PropertyChangedEventBase& event);

private:
  aw::Scene& mScene;
  aw::MessageBus& mMsgBus;

  CubeContainer mCollisionCubes;

  aw::Channel<ColMeshEvent>::SubscriptionType mSubscription;
  aw::Channel<PropertyChangedEventBase>::SubscriptionType mSubProperties;

  int mVersion{0};
  int mCubeIdCounter{0};

  aw::ecs::Entity mSelectedCube{};
};
