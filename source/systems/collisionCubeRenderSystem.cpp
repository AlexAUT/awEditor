#include "collisionCubeRenderSystem.hpp"

#include "../components/collisionCube.hpp"

#include <aw/graphics/core/camera.hpp>
#include <aw/graphics/core/geometry.hpp>
#include <aw/opengl/opengl.hpp>
#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/entitySystem/entitySystem.hpp>
#include <aw/runtime/entitySystem/unpackComponents.hpp>
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

void CollisionCubeRenderSystem::update(float dt)
{
  size_t count = mEntitySystem.getComponentCount<CollisionCube>();

  // Get space in renderer
  mSurfaceRenderer.clear();
  mOutlinesRenderer.clear();
  mPointRenderer.clear();

  auto surfaceBegin = mSurfaceRenderer.allocate(count * 36);
  auto outlinesBegin = mOutlinesRenderer.allocate(count * 24);
  auto pointsBegin = mPointRenderer.allocate(count * 8);

  using Transform = aw::ecs::components::Transform;
  for (auto [id, cCube, transform] : mEntitySystem.getView<CollisionCube, Transform>())
  {
    assert(cCube);
    LogTemp() << "Normal cube: " << cCube->aabb;
    auto cube = aw::AABB::createFromTransform(cCube->aabb, transform->getTransform());
    LogTemp() << "Transform: " << *transform;
    LogTemp() << "Transformed cube: " << cube;

    aw::geo::cube(cube, surfaceBegin);
    surfaceBegin += 36;

    aw::geo::cubeLines(cube.getCenter(), cube.getSize(), outlinesBegin);
    outlinesBegin += 24;

    aw::geo::cubePoints(cube.getCenter(), cube.getSize(), pointsBegin);
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
