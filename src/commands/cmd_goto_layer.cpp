/* ASEPRITE
 * Copyright (C) 2001-2013  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include "app.h"
#include "commands/command.h"
#include "context_access.h"
#include "modules/editors.h"
#include "modules/gui.h"
#include "raster/layer.h"
#include "raster/sprite.h"
#include "widgets/editor/editor.h"
#include "widgets/status_bar.h"

//////////////////////////////////////////////////////////////////////
// goto_previous_layer

class GotoPreviousLayerCommand : public Command
{
public:
  GotoPreviousLayerCommand();
  Command* clone() { return new GotoPreviousLayerCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

GotoPreviousLayerCommand::GotoPreviousLayerCommand()
  : Command("GotoPreviousLayer",
            "Goto Previous Layer",
            CmdUIOnlyFlag)
{
}

bool GotoPreviousLayerCommand::onEnabled(Context* context)
{
  return context->checkFlags(ContextFlags::ActiveDocumentIsWritable |
                             ContextFlags::HasActiveSprite);
}

void GotoPreviousLayerCommand::onExecute(Context* context)
{
  const ContextReader reader(context);
  const Sprite* sprite = reader.sprite();
  DocumentLocation location = *reader.location();

  if (location.layerIndex() > 0)
    location.layerIndex(location.layerIndex().previous());
  else
    location.layerIndex(LayerIndex(sprite->countLayers()-1));

  // Flash the current layer
  ASSERT(current_editor != NULL && "Current editor cannot be null when we have a current sprite");
  current_editor->setLayer(location.layer());
  current_editor->flashCurrentLayer();

  StatusBar::instance()
    ->setStatusText(1000, "Layer `%s' selected",
                    location.layer()->getName().c_str());
}

//////////////////////////////////////////////////////////////////////
// goto_next_layer

class GotoNextLayerCommand : public Command
{
public:
  GotoNextLayerCommand();
  Command* clone() { return new GotoNextLayerCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

GotoNextLayerCommand::GotoNextLayerCommand()
  : Command("GotoNextLayer",
            "Goto Next Layer",
            CmdUIOnlyFlag)
{
}

bool GotoNextLayerCommand::onEnabled(Context* context)
{
  return context->checkFlags(ContextFlags::ActiveDocumentIsWritable |
                             ContextFlags::HasActiveSprite);
}

void GotoNextLayerCommand::onExecute(Context* context)
{
  const ContextReader reader(context);
  const Sprite* sprite = reader.sprite();
  DocumentLocation location = *reader.location();

  if (location.layerIndex() < sprite->countLayers()-1)
    location.layerIndex(location.layerIndex().next());
  else
    location.layerIndex(LayerIndex(0));

  // Flash the current layer
  ASSERT(current_editor != NULL && "Current editor cannot be null when we have a current sprite");
  current_editor->setLayer(location.layer());
  current_editor->flashCurrentLayer();

  StatusBar::instance()
    ->setStatusText(1000, "Layer `%s' selected",
                    location.layer()->getName().c_str());
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::createGotoPreviousLayerCommand()
{
  return new GotoPreviousLayerCommand;
}

Command* CommandFactory::createGotoNextLayerCommand()
{
  return new GotoNextLayerCommand;
}
