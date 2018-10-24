#pragma once

#include <aw/graphics/core/shaderProgram.hpp>
#include <aw/runtime/renderers/primitiveRenderer.hpp>

namespace aw
{
class Camera;
}

class CollisionCubeManager;

class CollisionCubesRenderer
{
public:
  CollisionCubesRenderer(const CollisionCubeManager& manager);

  void render(const aw::Camera& mCamera);

private:
  void updateCache();

private:
  const CollisionCubeManager& mCubeManager;

  int mCacheVersion{-1};

  using PrimitiveRenderer = aw::PrimitiveRenderer<aw::VertexPos>;
  PrimitiveRenderer mSurfaceRenderer;
  PrimitiveRenderer mOutlinesRenderer;
  PrimitiveRenderer mPointRenderer;

  aw::ShaderProgram mSurfaceShader;
};
