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
#include "app/color.h"
#include "commands/command.h"
#include "console.h"
#include "context_access.h"
#include "document_api.h"
#include "modules/gui.h"
#include "raster/cel.h"
#include "raster/image.h"
#include "raster/layer.h"
#include "raster/sprite.h"
#include "ui/gui.h"
#include "undo_transaction.h"
#include "widgets/status_bar.h"

#include <stdexcept>

//////////////////////////////////////////////////////////////////////
// new_frame

class NewFrameCommand : public Command
{
public:
  NewFrameCommand();
  Command* clone() { return new NewFrameCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

NewFrameCommand::NewFrameCommand()
  : Command("NewFrame",
            "New Frame",
            CmdRecordableFlag)
{
}

bool NewFrameCommand::onEnabled(Context* context)
{
  return context->checkFlags(ContextFlags::ActiveDocumentIsWritable |
                             ContextFlags::HasActiveSprite |
                             ContextFlags::HasActiveLayer |
                             ContextFlags::ActiveLayerIsReadable |
                             ContextFlags::ActiveLayerIsWritable |
                             ContextFlags::ActiveLayerIsImage);
}

void NewFrameCommand::onExecute(Context* context)
{
  ContextWriter writer(context);
  Document* document(writer.document());
  Sprite* sprite(writer.sprite());
  {
    UndoTransaction undoTransaction(writer.context(), "New Frame");
    document->getApi().addFrame(sprite, writer.frame().next());
    undoTransaction.commit();
  }
  update_screen_for_document(document);

  StatusBar::instance()
    ->showTip(1000, "New frame %d/%d",
              (int)context->getActiveLocation().frame()+1,
              (int)sprite->getTotalFrames());
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::createNewFrameCommand()
{
  return new NewFrameCommand;
}
