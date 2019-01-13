#include "editorState.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/window.hpp>
#include <aw/graphics/core/shaderStage.hpp>
#include <aw/gui/style/defaultStyles.hpp>
#include <aw/gui/widgets/widgets.hpp>
#include <aw/opengl/opengl.hpp>
#include <aw/utils/colors.hpp>
#include <aw/utils/file/fileDialog.hpp>
#include <aw/utils/file/path.hpp>
#include <aw/utils/log.hpp>

DEFINE_LOG_CATEGORIES(Editor, "Editor state")

#include <aw/utils/math/constants.hpp>
using namespace aw::constantsF;

#include <utility>

aw::gui::Widget::SPtr window;

EditorState::EditorState(aw::Engine& engine) :
    aw::State(engine.getStateMachine()),
    mEngine(engine),
    mGUI(engine.getWindow().getSize(), engine.getMessageBus()),
    mMeshHandler(engine.getMessageBus(), mScene),
    mCollisionCubeManager(mScene, engine.getMessageBus()),
    mViewportManager(engine, mScene),
    mCamera(aw::Camera::createPerspective(engine.getWindow().getAspectRatio(), 60.f * TO_RAD, 0.1f, 200.f)),
    mCamController(&mCamera),
    mMeshRendererSystem(mScene.getEntitySystem()),
    mTransformSystem(mScene.getEntitySystem()),
    mCollisionCubeRenderSystem(mScene.getEntitySystem())
{
  mListenerId = mEngine.getWindow().registerEventListener([this](auto e) { this->processEvent(e); });
  mEngine.getWindow().setClearColor(aw::Colors::DIMSLATEGREY);

  auto vShader = aw::ShaderStage::loadFromPath(aw::ShaderStage::Vertex, aw::createAssetPath("shaders/simple.vert"));
  auto fShader = aw::ShaderStage::loadFromPath(aw::ShaderStage::Fragment, aw::createAssetPath("shaders/mesh.frag"));
  mMeshShader.link(*vShader, *fShader);

  NewMeshEvent event{"/home/alex/Documents/git/flightgame/assets/meshes/airplane1.fbx"};
  engine.getMessageBus().broadcast<MeshEvent>(event);
}

EditorState::~EditorState()
{
  mEngine.getWindow().unregisterEventListener(mListenerId);
}

void EditorState::update(float delta)
{
  mGUI.update(delta);

  mCamController.update(delta);

  mTransformSystem.update(delta);
  mMeshRendererSystem.update(delta);
  mCollisionCubeRenderSystem.update(delta, mCollisionCubeManager.getSelectedCube());
}

void EditorState::render()
{
  mEngine.getWindow().clear();

  GL_CHECK(glViewport(0, 0, mEngine.getWindow().getSize().x, mEngine.getWindow().getSize().y));
  mCamera.setAspectRatio(mEngine.getWindow().getAspectRatio());

  auto diff = mCamera.getPosition() - aw::Vec3{0.f}; // camPos - lookAt
  auto distance = glm::dot(mCamera.getViewDirection(), diff);

  auto cache = mCamera.getProjectionType();
  mCamera.setProjectionType(aw::Camera::ProjectionType::Perspective);
  auto p1 = mCamera.getPointInDistance({0.f, 0.f}, distance);
  auto p2 = mCamera.getPointInDistance({1.f, 0.f}, distance);
  mCamera.setProjectionType(cache);
  mCamera.setOrthoWidth(glm::distance(p2, p1));

  mMeshRendererSystem.render(mCamera);
  mCollisionCubeRenderSystem.render(mCamera);

  mGUI.render();
}

void EditorState::processEvent(const aw::WindowEvent& event)
{
  auto isCamControllerSelected = mCamController.isSelected();
  if (isCamControllerSelected)
    mCamController.processEvent(event);

  auto eventConsumed = mGUI.processEvent(event);
  if (eventConsumed)
    return;

  if (!isCamControllerSelected)
    mCamController.processEvent(event);

  mViewportManager.processEvent(event, mCamera);

  if (event.type == aw::WindowEvent::Closed)
    mEngine.terminate();

  if (event.type == aw::WindowEvent::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Escape)
      mEngine.terminate();
  }
}
