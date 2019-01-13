#include "meshPreviewHandler.hpp"

#include <aw/runtime/components/meshProvider.hpp>
#include <aw/runtime/components/meshRenderer.hpp>
#include <aw/runtime/components/transform.hpp>
#include <aw/runtime/loaders/assimpLoader.hpp>
#include <aw/runtime/resourceManager/factories/meshFactory.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/log.hpp>

#include <fstream>

MeshPreviewHandler::MeshPreviewHandler(aw::MessageBus& bus, aw::Scene& scene) :
    mSubscription(bus.subscribeToChannel<MeshEvent>([this](const MeshEvent& e) { this->processEvent(e); })),
    mScene(scene),
    mEntity(mScene.getEntitySystem().createEntity())
{
  using namespace aw::ecs::components;
  mEntity.add<Transform>(mEntity);
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
  auto path = aw::createAbsolutePath(event.meshPath);

  using namespace aw::factories;
  auto& resManager = mScene.getResourceManager();
  auto mesh = resManager.getRegistry<aw::Mesh>().create<MeshFactory>(path.getRelativePath(), path, resManager);

  if (!mesh)
  {
    LogTemp() << "Failed to load mesh: " << path << "!";
  }
  else
  {
    // ECS
    using namespace aw::ecs::components;
    if (!mEntity.has<MeshProvider>())
      mEntity.add<MeshProvider>(mesh);
    else
      mEntity.get<MeshProvider>()->mMesh = mesh;

    mEntity.get<Transform>()->setPosition(aw::Vec3{0.f});
    mEntity.get<Transform>()->setRotation(aw::Vec3{0.f});
    mEntity.get<Transform>()->setScale(aw::Vec3{1.f});
  }
}
