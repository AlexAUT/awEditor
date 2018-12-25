#pragma once

#include <aw/graphics/core/shaderProgram.hpp>
#include <aw/runtime/entitySystem/entity.hpp>
#include <aw/runtime/renderers/primitiveRenderer.hpp>

namespace aw
{
class Camera;
}

namespace aw::ecs
{
class EntitySystem;
}

class CollisionCubeRenderSystem
{
public:
  CollisionCubeRenderSystem(aw::ecs::EntitySystem& entitySystem);

  void update(float dt, aw::ecs::Entity selectedEntity);
  void render(const aw::Camera& camera);

private:
private:
  aw::ecs::EntitySystem& mEntitySystem;

  using PrimitiveRenderer = aw::PrimitiveRenderer<aw::VertexPos>;
  PrimitiveRenderer mSurfaceRenderer;
  PrimitiveRenderer mOutlinesRenderer;
  PrimitiveRenderer mPointRenderer;

  aw::ShaderProgram mSurfaceShader;
};
