#pragma once

#include <vector>

#include <aw/utils/spatial/AABB.hpp>

class CollisionCubeManager
{
public:
  using RectContainer = std::vector<aw::AABB>;

public:
  void addRect(aw::AABB rect);

  RectContainer::const_iterator cbegin() { return mCollisionRects.cbegin(); }
  RectContainer::const_iterator cend() { return mCollisionRects.cend(); };

private:
private:
  RectContainer mCollisionRects;
};
