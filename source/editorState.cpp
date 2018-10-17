#include "editorState.hpp"

#include <aw/engine/engine.hpp>
#include <aw/engine/window.hpp>
#include <aw/graphics/core/shaderStage.hpp>
#include <aw/utils/colors.hpp>

#include <aw/gui/style/defaultStyles.hpp>
#include <aw/gui/widgets/widgets.hpp>

#include <aw/utils/file/fileDialog.hpp>
#include <aw/utils/log.hpp>
DEFINE_LOG_CATEGORY(EditorD, aw::log::Debug, "Editor")

#include <aw/utils/math/constants.hpp>
using namespace aw::constantsF;

aw::gui::Widget::SPtr window;

EditorState::EditorState(aw::Engine& engine)
    : aw::State(engine.getStateMachine()), mEngine(engine), mGUI(engine.getWindow().getSize(), engine.getMessageBus()),
      mMeshManager(engine.getMessageBus(), mScene),
      mCamera(aw::Camera::createPerspective(engine.getWindow().getAspectRatio(), 60.f * TO_RAD, 0.1f, 200.f)),
      mCamController(&mCamera)
{
  mListenerId = mEngine.getWindow().registerEventListener([this](auto e) { this->processEvent(e); });
  mEngine.getWindow().setClearColor(aw::Colors::DARKRED);

  LogTemp() << engine.getWindow().getAspectRatio();

  auto vShader = aw::ShaderStage::loadFromAssetFile(aw::ShaderStage::Vertex, "shaders/simple.vert");
  auto fShader = aw::ShaderStage::loadFromAssetFile(aw::ShaderStage::Fragment, "shaders/mesh.frag");
  assert(vShader && fShader);
  mMeshShader.link(*vShader, *fShader);

  NewMeshEvent event{"/home/alex/Documents/git/flightGame/assets/meshes/airplane1.fbx"};
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

  mMeshRenderer.analyzeScene(mScene);
}

void EditorState::render()
{
  mEngine.getWindow().clear();

  mCamera.setAspectRatio(mEngine.getWindow().getAspectRatio());
  mMeshRenderer.renderForwardPass(mCamera, mMeshShader);

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

  if (event.type == aw::WindowEvent::Closed)
    mEngine.terminate();

  if (event.type == aw::WindowEvent::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Escape)
      mEngine.terminate();
  }
}
