// HI group! This is where I shall explain what I was thinking. I do not know whether my reasoning
// is correct. But I think its a good place to start.

// What's important to know about inflatable tube man is that when the air speed within the tube decreases,
// pressure increases, causing the tube to expand and straighten. When the air speed decreases, pressure increases. 
// But air particles are difficult to simulate. So is a physically accurate version of pressure. 

// So, how can we simulate something that looks close? 

// One way to do it is through the idea of torque. This idea is explained further in the physics
// forum post I sent earlier, but basically torque is rotational force and we can apply it towards a rig of rods -- 
// each parametrized by a start and end pointmass and a pivot. 

// If you look at a video of a tube man. You'll notice that there's a fold that pulses through different parts
// of the tube. So we're essentially trying to model that pulse with torque.

// The following logic is where I dont think Im completely correct but just an idea -- 

// As the fold moves upward, we can say that the fold is defined by two rods with a creasing angle

// This creasing angle acts as a force on all of the tube man .. through torque. Torque is proportional
// to the angle between the axis and the pivot -- in this case a pinned rod and a non pinned rod. (even 
//     though we still have forces like wind and gravity acting on the pinned rod, I know the terminology
//     might be confusing)

// So, if the "crease" is moving up [or down] and a crease is defined by a pinned and non pinned rod, then we 
// can (maybe lol) simulate the tube man motion by something like this: 

//     note -- notice how after a "Crease" has moved up the tube, it essentially flips and comes back down again
//         and the cycle repeats

// - find the torque force at each time step
//     - iterate over a list of rods (because that is the "rig" of the internal forces motion)
//         - calculate the external forces (eg gravity, ig wind)
//         - update them 
//     - iterate over list of rods again
//         - check direction -- up or down? 
//         - based on direction find the first unpinned rod
//         - this is gonna be the pivot of the "torque" stuff 
//         - find the forces acting on it
//             - should already be in pivot point's .forces variable
//         - calculate torque force using F * sin theta where theta is the angle between the unpinned rod and the pinned rod 
//     - iterate over point masses 
//         - add torque force to point mass


//also while we're here -- updates on the pacman hole. So there's a bottom of the tube. there is no top. The bottom looks mostly okay 
//except for pacman hole. So I asked on piazza and a TA said: 

//I had a similar issue when trying to use the Project 4 codebase last year for my final project.The fix was to change some of the hardcoded
//codebase so that it would render numTriangles.size - 1 instead of numTriangles.size and changing some other parts in drawWireframe so that
//the number of springs is less since you want the cloth to wrap around on itself.I would recommend taking a look at 
//void ClothSimulator::drawWireframe(GLShader & shader), void Cloth::buildClothMesh(), void ClothSimulator::drawPhong(GLShader & shader), 
//void ClothSimulator::drawNormals(GLShader & shader), and similar functions.I'm not sure exactly what fixed it since this may be specific to
//how you indexed things, but it is related to these areas of the code not stuff that your group wrote necessarily.

//I tried to play around with things but changing it to numtriangles.size-1 didnt seem to do anything. tbh to me it looked more like a structural
//spring was missing but adding one in didn't work either. the lines that handle creating the relevant springs are 121-129 in cloth.cpp buildgrid. 
//but playing around with that part didnt work either lol. right now since ive added the rod stuff and there are a few errors you cant yet you can
//render the pacman holed cylinder thing by ctrl f "rod" and deleting those sections of code that I've also labelled with comments


#ifndef ROD_H
#define ROD_H

#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"
#include "pointMass.h"
using namespace CGL;

// Forward declarations

struct Rod {
    Rod(PointMass p0, PointMass p1, double diam) :
        pivotPt(p0), point0(p0), point1(p1)
    {}

    bool pinned; //is it moving?

    // dynamic values
    Vector3D last_position;
    PointMass pivotPt;
    Vector3D torque;
    Vector3D forceTorque;
    Vector3D prev;
    Vector3D next;
    PointMass point0;
    PointMass point1;
    double angle(bool goingup, Rod rod) {
        if (goingup)
            return dot(point1.position, rod.point0.position) / (point1.position.norm() * rod.point0.position.norm());
        return dot(point0.position, rod.point1.position) / (point0.position.norm() * rod.point1.position.norm());
        ;
    }
    Vector3D torqueWeight() {
        return forceTorque * sin(angle());
    }
    void changePivot() {
        if (pivotPt.position == point0.position) {
            pivotPt = point1;
        }
        else {
            pivotPt = point0;
        }
    }

    bool isPointMassInside(PointMass* p) {
        //idk if we need this -- maybe only apply forces if pointmass is inside? but i feel like apply forces regardless
    }

    // linked list of rods
};

#endif /* ROD_H */
