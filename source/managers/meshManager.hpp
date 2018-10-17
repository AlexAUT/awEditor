#pragma once

#include <aw/graphics/3d/mesh.hpp>

#include <aw/runtime/scene/scene.hpp>

#include <aw/utils/messageBus/messageBus.hpp>

#include "../events/meshEvent.hpp"

class MeshManager
{
public:
  MeshManager(aw::MessageBus& bus, aw::Scene& scene);

  aw::Mesh* getMesh() const { return mMesh.get(); }

private:
  void processEvent(const MeshEvent& event);

  void loadNewMesh(const NewMeshEvent& event);

private:
  aw::Channel<MeshEvent>::SubscriptionType mSubscription;

  aw::Scene& mScene;

  std::unique_ptr<aw::Mesh> mMesh;
};
