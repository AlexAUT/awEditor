#pragma once

#include <aw/graphics/3d/mesh.hpp>
#include <aw/runtime/entitySystem/entity.hpp>
#include <aw/runtime/scene/scene.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include "../events/meshEvent.hpp"

class MeshPreviewHandler
{
public:
  MeshPreviewHandler(aw::MessageBus& bus, aw::Scene& scene);

private:
  void processEvent(const MeshEvent& event);

  void loadNewMesh(const NewMeshEvent& event);

private:
  aw::Channel<MeshEvent>::SubscriptionType mSubscription;

  aw::Scene& mScene;
  aw::ecs::Entity mEntity;
};
