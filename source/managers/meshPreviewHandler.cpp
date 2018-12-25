#include "meshPreviewHandler.hpp"

#include <aw/runtime/scene/scene.hpp>

#include <aw/utils/log.hpp>
#include <fstream>

#include <aw/runtime/components/meshProvider.hpp>
#include <aw/runtime/components/meshRenderer.hpp>
#include <aw/runtime/components/transform.hpp>

#include <aw/runtime/components/transform.hpp>

#include <aw/runtime/loaders/assimpLoader.hpp>

MeshPreviewHandler::MeshPreviewHandler(aw::MessageBus& bus, aw::Scene& scene) :
    mSubscription(bus.subscribeToChannel<MeshEvent>([this](const MeshEvent& e) { this->processEvent(e); })),
    mScene(scene),
    mEntity(mScene.getEntitySystem().createEntity())
{
  using namespace aw::ecs::components;
  mEntity.add<Transform>();
  mEntity.add<MeshRenderer>();
}

void MeshPreviewHandler::processEvent(const MeshEvent& event)
{
  switch (event.type)
  {
  case MeshEventType::NewMesh:
    loadNewMesh(static_cast<const NewMeshEvent&>(event));
    break;
  default:
    break;
  }
}

void MeshPreviewHandler::loadNewMesh(const NewMeshEvent& event)
{
  // Check path and get asset directory
  std::string key = "/assets/";
  auto found = event.meshPath.find("assets");
  if (found == std::string::npos)
  {
    LogTemp() << "Mesh has to be placed inside a assets directory";
    return;
  }
  std::string assetPath = event.meshPath.substr(0, found + key.size() - 1);
  LogTemp() << assetPath;
  LogTemp() << "Load mesh: " << event.meshPath;

  aw::AssimpLoader loader;
  loader.setAssetPath(assetPath);
  aw::Path path(aw::Path::Type::Absolute, event.meshPath);
  loader.loadFromPath(path);

  auto mesh = std::shared_ptr<aw::Mesh>(loader.loadMesh("previewMesh").release());
  if (!mesh)
  {
    LogTemp() << "Failed to load mesh: " << path.getAbsolutePath() << "!";
  }
  else
  {
    // ECS
    using namespace aw::ecs::components;
    if (!mEntity.has<MeshProvider>())
      mEntity.add<MeshProvider>(mesh);
    else
      mEntity.get<MeshProvider>()->mMesh = mesh;

    *mEntity.get<Transform>() = {};
  }
}
