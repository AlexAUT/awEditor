#include "collisionCubeRenderSystem.hpp"

#include "../components/collisionCube.hpp"

#include <aw/graphics/core/camera.hpp>
#include <aw/graphics/core/geometry.hpp>
#include <aw/opengl/opengl.hpp>
#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/entitySystem/componentIterator.hpp>
#include <aw/runtime/entitySystem/componentView.hpp>
#include <aw/runtime/entitySystem/entitySystem.hpp>
#include <aw/runtime/entitySystem/multiComponentView.hpp>
#include <aw/utils/file/path.hpp>

CollisionCubeRenderSystem::CollisionCubeRenderSystem(aw::ecs::EntitySystem& entitySystem) :
    mEntitySystem(entitySystem),
    mSurfaceRenderer(PrimitiveRenderer::PrimitiveType::Triangles),
    mOutlinesRenderer(PrimitiveRenderer::PrimitiveType::Lines),
    mPointRenderer(PrimitiveRenderer::PrimitiveType::Points)
{
  mSurfaceShader.link(
      *aw::ShaderStage::loadFromPath(aw::ShaderStage::Vertex, aw::createAssetPath("shaders/primitive.vert")),
      *aw::ShaderStage::loadFromPath(aw::ShaderStage::Fragment, aw::createAssetPath("shaders/color.vert")));
}

void CollisionCubeRenderSystem::update(float dt, aw::ecs::Entity selectedEntity)
{
  size_t count = mEntitySystem.getComponentCount<CollisionCube>();

  // Get space in renderer
  mSurfaceRenderer.clear();
  mOutlinesRenderer.clear();
  mPointRenderer.clear();

  auto outlinesBegin = mOutlinesRenderer.allocate(count * 24);
  auto pointsBegin = mPointRenderer.allocate(count * 8);

  using Transform = aw::ecs::components::Transform;
  int t1 = 0;
  for (auto [id, cCube, transform] : mEntitySystem.getView<CollisionCube, Transform>())
  {
    t1++;
    assert(cCube);
    aw::Vec3 center = transform->getTransform() * aw::Vec4(cCube->center, 1.f);
    aw::Vec3 size = transform->getTransform() * aw::Vec4(cCube->size, 0.f);

    if (selectedEntity.getId() == id)
    {
      auto surfaceBegin = mSurfaceRenderer.allocate(36);
      aw::geo::cube(center, size, surfaceBegin);
    }

    aw::geo::cubeLines(center, size, outlinesBegin);
    outlinesBegin += 24;

    aw::geo::cubePoints(center, size, pointsBegin);
    pointsBegin += 8;
  }
}

void CollisionCubeRenderSystem::render(const aw::Camera& camera)
{
  GL_CHECK(glEnable(GL_BLEND));
  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
  GL_CHECK(glDisable(GL_DEPTH_TEST));
  GL_CHECK(glPointSize(4.f));

  mSurfaceShader.bind();
  mSurfaceShader.setUniform("mvp_matrix", camera.getVPMatrix());

  mSurfaceShader.setUniform("color", aw::Vec4({0.75f, 0.4f, 0.0f, 0.25f}));
  mSurfaceRenderer.render();

  mSurfaceShader.setUniform("color", aw::Vec4({1.f, 0.4f, 0.0f, 0.3}));
  mOutlinesRenderer.render();

  mSurfaceShader.setUniform("color", aw::Vec4({1.f, 0.4f, 0.0f, 0.5}));
  mPointRenderer.render();

  mSurfaceShader.unbind();

  GL_CHECK(glEnable(GL_DEPTH_TEST));
  GL_CHECK(glDisable(GL_BLEND));
}
