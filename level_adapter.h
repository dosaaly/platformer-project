#pragma once
#include "level.h"

// single global instance for now
extern Level gLevel;

// wrappers so the old procedural code compiles unchanged
inline bool  is_inside_level(int r,int c)                  { return gLevel.inside(r,c); }
inline bool  is_colliding(Vector2 p,char look)             { return gLevel.colliding(p,look); }
inline char& get_collider(Vector2 p,char look)             { return gLevel.colliderRef(p,look); }
