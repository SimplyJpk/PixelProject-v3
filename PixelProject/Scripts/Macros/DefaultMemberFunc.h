#pragma once

// Macro to generate a default copy constructor for a given type.
// The copy constructor is used to initialize a new object as a copy of an existing object.
#define DEFAULT_COPY_CONSTRUCTOR(Type) Type(const Type&) = default;

// Macro to generate a default copy assignment operator for a given type.
// The copy assignment operator is used to copy the values from one object to another already existing object.
#define DEFAULT_COPY_ASSIGNMENT(Type) Type& operator=(const Type&) = default;

// Macro to generate a default move constructor for a given type.
// The move constructor is used to initialize a new object from an existing object by moving (rather than copying) its resources.
#define DEFAULT_MOVE_CONSTRUCTOR(Type) Type(Type&&) = default;

// Macro to generate a default move assignment operator for a given type.
// The move assignment operator is used to transfer the resources of one object to another already existing object.
#define DEFAULT_MOVE_ASSIGNMENT(Type) Type& operator=(Type&&) = default;

// Macro to generate a default destructor for a given type.
// The destructor is a special function that is called when an object goes out of scope or is explicitly deleted.
#define DEFAULT_DESTRUCTOR(Type) ~Type() = default;