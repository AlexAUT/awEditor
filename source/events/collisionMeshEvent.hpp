#pragma once

#include <aw/runtime/entitySystem/entity.hpp>

#include <string>

enum class ColMeshEventType
{
  New,
  Created,
  Select,
  Selected,
};

struct ColMeshEvent
{
  ColMeshEventType type;
};

struct NewColMeshEvent : public ColMeshEvent
{
  std::string meshPath;

  NewColMeshEvent() : ColMeshEvent{ColMeshEventType::New} {}
};

struct CreatedColMeshEvent : public ColMeshEvent
{
  aw::ecs::Entity entity;

  CreatedColMeshEvent(aw::ecs::Entity entity) : ColMeshEvent{ColMeshEventType::Created}, entity(entity) {}
};

struct SelectColMeshEvent : public ColMeshEvent
{
  std::string_view id;

  SelectColMeshEvent(std::string_view id) : ColMeshEvent{ColMeshEventType::Select}, id(id) {}
};

struct SelectedColMeshEvent : public ColMeshEvent
{
  aw::ecs::Entity entity;

  SelectedColMeshEvent(aw::ecs::Entity entity) : ColMeshEvent{ColMeshEventType::Selected}, entity(entity) {}
};
