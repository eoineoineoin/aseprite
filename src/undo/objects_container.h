// ASEPRITE Undo Library
// Copyright (C) 2001-2013  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#ifndef UNDO_OBJECTS_CONTAINER_H_INCLUDED
#define UNDO_OBJECTS_CONTAINER_H_INCLUDED

#include "base/exception.h"
#include "undo/object_id.h"

namespace undo {

// Thrown when you use ObjectsContainer::insertObject() with an
// existent ID or pointer.
class ExistentObjectException : base::Exception
{
public:
  ExistentObjectException() { }
};

// Thrown when an object is not found when
// ObjectsContainer::getObject() or removeObject() methods are used.
class ObjectNotFoundException : base::Exception
{
public:
  ObjectNotFoundException() { }
};

// A container of any kind of object used to generate serializable
// references (ID) to instances in memory. It converts a pointer to a
// 32-bits ID, and then you can get back the original object pointer
// from the ID.
//
// If the original pointer is deleted, you must use removeObject(),
// and if the object is re-created (e.g. by an undo operation),
// the object can be added back to the container using insertObject().
class ObjectsContainer
{
public:
  virtual ~ObjectsContainer() { };

  // Adds an object in the container and returns an ID, so then you
  // can retrieve the original object pointer using getObject()
  // method. If the object is already in the container, the returned
  // ID must be the same as the already assigned. It means that this
  // method cannot return different IDs for the same given "void*"
  // pointer.
  virtual ObjectId addObject(void* object) = 0;

  // Inserts an existent object with the specific ID. If an object
  // with the given ID or the pointer already exist in the container,
  // it should throw an ExistentObjectException. This method is used
  // to insert back in the container a previously removed object
  // with a removeObject() call.
  virtual void insertObject(ObjectId id, void* object) = 0;

  // Removes the given object from the container because it cannot be
  // referenced anymore. Anyway the ID is not re-used by following
  // calls to addObject(), so the object can be added back into the
  // container with the same ID using insertObject() method. If the
  // object does not exist in the container, it throws an
  // ObjectNotFoundException exception.
  virtual void removeObject(ObjectId id) = 0;

  // Returns the object pointer associated to the given ID. The
  // pointer is the same as the used in addObject() or insertObject()
  // method. If the object does not exist, it throws an
  // ObjectNotFoundException.
  virtual void* getObject(ObjectId id) = 0;

  // Helper method to cast getObject() to the expected object type.
  template<class T>
  T* getObjectT(ObjectId id)
  {
    return reinterpret_cast<T*>(getObject(id));
  }

};

} // namespace undo

#endif  // UNDO_OBJECTS_CONTAINER_H_INCLUDED
