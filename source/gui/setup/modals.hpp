#pragma once

#include <aw/gui/gui.hpp>
#include <aw/gui/widgets/widgets.hpp>

#include <aw/utils/file/fileDialog.hpp>
#include <aw/utils/log.hpp>
#include <aw/utils/messageBus/messageBus.hpp>

#include "../../events/meshEvent.hpp"
#include "../guiEvent.hpp"

namespace priv
{
void newMeshModal(const aw::MessageBus& messageBus);
}

void setupModals(aw::MessageBus& messageBus)
{
  messageBus.subscribeToChannelUnsafe<GuiEvent>([&messageBus](const GuiEvent& event) {
    switch (event)
    {
    case GuiEvent::NewLevel:
      priv::newMeshModal(messageBus);
      break;
    default:
      break;
    }
  });
}

namespace priv
{
void newMeshModal(const aw::MessageBus& messageBus)
{
  auto meshPath = aw::openFileDialog<1>("Select mesh file", "/home/alex/Documents/git/flightGame/assets/meshes/",
                                        {{"*.fbx"}}, "Mesh file (.fbx)", false);

  NewMeshEvent event{meshPath};
  messageBus.broadcast(static_cast<const MeshEvent&>(event));
}
} // namespace priv
