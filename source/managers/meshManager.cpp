#include "meshManager.hpp"

#include <aw/runtime/scene/meshNode.hpp>
#include <aw/runtime/scene/scene.hpp>

#include <aw/utils/log.hpp>
#include <fstream>

#include <aw/runtime/loaders/assimpLoader.hpp>

MeshManager::MeshManager(aw::MessageBus& bus, aw::Scene& scene)
    : mSubscription(bus.subscribeToChannel<MeshEvent>([this](const MeshEvent& e) { this->processEvent(e); })),
      mScene(scene)
{
}

void MeshManager::processEvent(const MeshEvent& event)
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

void MeshManager::loadNewMesh(const NewMeshEvent& event)
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
  std::fstream meshFile(event.meshPath);
  if (meshFile.is_open())
  {
    aw::AssimpLoader loader;
    loader.setAssetPath(assetPath);
    if (loader.loadFromStream(event.meshPath, meshFile, "fbx"))
    {
      mMesh = loader.loadMesh("mesh", false);
      if (mMesh)
      {
        LogTemp() << "Loaded";
        auto* oldMesh = mScene.findNodeByName("mesh");
        if (oldMesh)
        {
          oldMesh->setParent(nullptr);
          delete oldMesh;
        }
        auto* meshNode = new aw::MeshNode(*mMesh);
        meshNode->setName("mesh");
        mScene.addChild(meshNode);
      }
    }
    else
    {
      LogTemp() << "Failed";
    }
  }
}
