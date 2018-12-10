#pragma once

#include <aw/engine/runtime/state.hpp>
#include <aw/engine/window.hpp>

#include <aw/graphics/3d/orbitCameraController.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/graphics/core/shaderProgram.hpp>

#include <aw/runtime/renderers/primitiveRenderer.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/runtime/systems/meshRendererSystem.hpp>

#include "gui/gui.hpp"
#include "managers/cameraManager.hpp"
#include "managers/collisionCubeManager.hpp"
#include "managers/meshPreviewHandler.hpp"
#include "renderers/collisionCubesRenderer.hpp"

namespace aw
{
class Engine;
} // namespace aw

class EditorState : public aw::State
{
public:
  EditorState(aw::Engine& engine);
  ~EditorState();

  void update(float delta) override;
  void render() override;

  void processEvent(const aw::WindowEvent& event);

private:
  aw::Engine& mEngine;
  unsigned mListenerId;

  aw::Scene mScene;

  GUI mGUI;
  MeshPreviewHandler mMeshHandler;
  CollisionCubeManager mCollisionCubeManager;

  aw::ShaderProgram mMeshShader;

  aw::Camera mCamera;
  CameraManager mCamController;

  aw::ecs::systems::MeshRendererSystem mMeshRendererSystem;

  CollisionCubesRenderer mColMeshRenderer;
};
