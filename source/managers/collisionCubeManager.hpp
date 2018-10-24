#pragma once

#include <vector>

#include <aw/utils/spatial/AABB.hpp>

#include <aw/utils/messageBus/messageBus.hpp>

#include "../events/collisionMeshEvent.hpp"

class CollisionCubeManager
{
public:
  using RectContainer = std::vector<aw::AABB>;

public:
  CollisionCubeManager(aw::MessageBus& bus);

  void addRect(aw::AABB rect);
  void changeRect(size_t index, aw::AABB rect);

  size_t getRectCount() const { return mCollisionRects.size(); }

  RectContainer::const_iterator begin() const { return mCollisionRects.cbegin(); }
  RectContainer::const_iterator end() const { return mCollisionRects.cend(); };

  int getVersion() const { return mVersion; }

private:
  void handleEvent(const ColMeshEvent& event);

private:
  RectContainer mCollisionRects;

  aw::Channel<ColMeshEvent>::SubscriptionType mSubscription;

  int mVersion{0};
};
