#pragma once

#include <vector>

#include <aw/runtime/entitySystem/entity.hpp>
#include <aw/utils/messageBus/messageBus.hpp>
#include <aw/utils/spatial/AABB.hpp>

#include "../events/collisionMeshEvent.hpp"

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
  void changeCube(size_t index, aw::AABB cube);

  size_t getCubeCount() const { return mCollisionRects.size(); }

  aw::ecs::Entity getCubeById(std::string_view id) const;

  CubeContainer::const_iterator begin() const { return mCollisionRects.cbegin(); }
  CubeContainer::const_iterator end() const { return mCollisionRects.cend(); };

  int getVersion() const { return mVersion; }

private:
  void handleEvent(const ColMeshEvent& event);

private:
  aw::Scene& mScene;
  aw::MessageBus& mMsgBus;

  CubeContainer mCollisionRects;

  aw::Channel<ColMeshEvent>::SubscriptionType mSubscription;

  int mVersion{0};

  int mCubeIdCounter{0};
};
