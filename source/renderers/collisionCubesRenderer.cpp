#include "collisionCubesRenderer.hpp"

#include "../managers/collisionCubeManager.hpp"

#include <aw/graphics/core/camera.hpp>
#include <aw/graphics/core/geometry.hpp>
#include <aw/opengl/opengl.hpp>
#include <aw/utils/file/path.hpp>

CollisionCubesRenderer::CollisionCubesRenderer(const CollisionCubeManager& manager)
    : mCubeManager(manager), mSurfaceRenderer(PrimitiveRenderer::PrimitiveType::Triangles),
      mOutlinesRenderer(PrimitiveRenderer::PrimitiveType::Lines),
      mPointRenderer(PrimitiveRenderer::PrimitiveType::Points)
{
  mSurfaceShader.link(
      *aw::ShaderStage::loadFromPath(aw::ShaderStage::Vertex, aw::createAssetPath("shaders/primitive.vert")),
      *aw::ShaderStage::loadFromPath(aw::ShaderStage::Fragment, aw::createAssetPath("shaders/color.vert")));
}

void CollisionCubesRenderer::render(const aw::Camera& camera)
{
  if (mCacheVersion != mCubeManager.getVersion())
    updateCache();

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

void CollisionCubesRenderer::updateCache()
{
  mCacheVersion = mCubeManager.getVersion();

  auto count = mCubeManager.getRectCount();

  mSurfaceRenderer.clear();
  mOutlinesRenderer.clear();
  mPointRenderer.clear();

  auto surfaceBegin = mSurfaceRenderer.allocate(count * 36);
  auto outlinesBegin = mOutlinesRenderer.allocate(count * 24);
  auto pointsBegin = mPointRenderer.allocate(count * 8);

  for (auto& cube : mCubeManager)
  {
    aw::geo::cube(cube.getCenter(), cube.getSize(), surfaceBegin);
    surfaceBegin += 36;

    aw::geo::cubeLines(cube.getCenter(), cube.getSize(), outlinesBegin);
    outlinesBegin += 24;

    aw::geo::cubePoints(cube.getCenter(), cube.getSize(), pointsBegin);
    pointsBegin += 8;
  }
  LogTemp() << "Update cache!";
}
