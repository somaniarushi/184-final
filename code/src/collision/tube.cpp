#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "tube.h"

using namespace nanogui;
using namespace CGL;

void Tube::collide(PointMass &pm) {
    //handle collisions with sphere
}

void Tube::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
// }
