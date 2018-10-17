#pragma once

#include <string>

enum class MeshEventType
{
  NewMesh,
};

struct MeshEvent
{
  MeshEventType type;
};

struct NewMeshEvent : public MeshEvent
{
  std::string meshPath;

  NewMeshEvent(std::string meshPath_) : meshPath(meshPath_) { type = MeshEventType::NewMesh; }
};
