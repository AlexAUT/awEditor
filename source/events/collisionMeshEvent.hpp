#pragma once

#include <string>

enum class ColMeshEventType
{
  New,
};

struct ColMeshEvent
{
  ColMeshEventType type;
};

struct NewColMeshEvent : public ColMeshEvent
{
  std::string meshPath;

  NewColMeshEvent() { type = ColMeshEventType::New; }
};
