#include <iostream>
#include <math.h>
#include <random>
#include <vector>

#include "cloth.h"
#include "collision/plane.h"
#include "collision/sphere.h"

using namespace std;

Cloth::Cloth(double width, double height, int num_width_points,
    int num_height_points, float thickness) {
    this->width = width;
    this->height = height;
    this->num_width_points = num_width_points;
    this->num_height_points = num_height_points;
    this->thickness = thickness;

    buildGrid();
    buildClothMesh();
}

Cloth::~Cloth() {
    point_masses.clear();
    springs.clear();
    //rods.clear();
    if (clothMesh) {
        delete clothMesh;
    }
}

void Cloth::buildGrid() {
    // TODO (Part 1): Build a grid of masses and springs.

    vector<vector<int>>::iterator start = pinned.begin();
    vector<int>::iterator start_pt;
    double w_offset = width / num_width_points;
    double h_offset = height / num_height_points;

    Vector3D origin = Vector3D();
    double r = width / 4;

    double incr_amt = (double)(2.0 * PI) / num_width_points;
    //bottom
    for (double r2 = 0; r2 <= r; r2 += .03) {
        for (double angle = 0; angle <= 2.0 * PI; angle += incr_amt) {
            Vector3D pos;
            double x = r2 * sin(angle);
            double z = r2 * cos(angle);
            if (angle >= 2 * PI - incr_amt) {
                x = r2 * sin(0);
                z = r2 * cos(0);
            }

            pos = Vector3D(x, 0, z);

            //fix the bottom of the tube, center point only i think? this is anchor
            if (r2 == 0)
                point_masses.emplace_back(PointMass(pos, true));

            else {
                point_masses.emplace_back(PointMass(pos, false));
            }

        }
    }
    //tube
    for (int h = 1; h < num_height_points; h++) {
        for (double angle = 0; angle <= 2 * PI; angle += incr_amt) {
            Vector3D pos;
            double x = r * sin(angle);
            double z = r * cos(angle);

            if (angle >= 2 * PI - incr_amt) {
                x = r * sin(0) + 0.0001;
                z = r * cos(0) + 0.0001;
            }
            //do the same thing for th
            pos = Vector3D(x, h_offset * h, z);
            point_masses.emplace_back(PointMass(pos, false));
            //if top or bottom fill with points
        }
    }
    //top
    for (double r2 = 0; r2 <= r; r2 += .03) {
        for (double angle = 0; angle <= 2 * PI; angle += incr_amt) {
            std::cout << "e" << "\n";
            Vector3D pos;
            double x = r2 * sin(angle);
            double z = r2 * cos(angle);
            if (angle >= 2 * PI - incr_amt) {
                x = r * sin(0) + 0.0001;
                z = r * cos(0) + 0.0001;
            }

            pos = Vector3D(x, (num_height_points)*h_offset, z);

            //fix the bottom of the tube, center point only i think?
            point_masses.emplace_back(PointMass(pos, false));
        }
    }


    for (int i = 0; i < pinned.size(); i++) {
        vector<int> xy = pinned[i];
        int index = xy[0] + xy[1] * num_width_points;
        point_masses[index].pinned = true;
    }



    //structural, shear, and bending constraints between point masses
    for (int h_count = 0; h_count < num_height_points; h_count += 1) {
        for (int w_count = 0; w_count < num_width_points; w_count += 1) {
            int index = w_count + (h_count * num_width_points);
            PointMass* m1 = &point_masses[index];

            // structural constraint check
            //left
            if (w_count > 0) {
                PointMass* m2 = &point_masses[index - 1];
                springs.emplace_back(Spring(m1, m2, STRUCTURAL));
            }
            //above
            if (h_count > 0) {
                PointMass* m2 = &point_masses[index - num_width_points];
                springs.emplace_back(Spring(m1, m2, STRUCTURAL));
            }

            //shearing constraint check
            if (h_count > 0) {
                //left upper
                if (w_count > 0) {
                    PointMass* m2 = &point_masses[index - num_width_points - 1];
                    springs.emplace_back(Spring(m1, m2, SHEARING));
                }
                //right upper
                if (w_count < num_width_points - 1) {
                    PointMass* m2 = &point_masses[index - num_width_points + 1];
                    springs.emplace_back(Spring(m1, m2, SHEARING));
                }
            }

            //bending constraint check
            if (w_count > 1) {
                PointMass* m2 = &point_masses[index - 2];
                springs.emplace_back(Spring(m1, m2, BENDING));
            }
            //above
            if (h_count > 1) {
                PointMass* m2 = &point_masses[index - (num_width_points * 2)];
                springs.emplace_back(Spring(m1, m2, BENDING));
            }
        }
    }
    //rod stuff
    Vector3D currP = origin; 
    for (int i = 0; i <= num_height_points; i += h_offset) {
        //create new rod for hoffset length
        rods.emplace_back(Rod(currP, currP + Vector3D(0, currP.y + h_offset, 0)));
        currP.y += h_offset;
    }
}

void Cloth::simulate(double frames_per_sec, double simulation_steps, ClothParameters* cp,
    vector<Vector3D> external_accelerations,
    vector<CollisionObject*>* collision_objects) {
    double mass = width * height * cp->density / num_width_points / num_height_points;
    double delta_t = 1.0f / frames_per_sec / simulation_steps;

    // TODO (Part 2): Compute total force acting on each point mass.
    Vector3D externalForce;
    for (Vector3D& accel : external_accelerations) {
        externalForce += (mass * accel);
    }
    for (PointMass& pointMass : point_masses) {
        pointMass.forces = externalForce;
    }
    for (Rod rod : rods) {
        rod.point0.forces = externalForce;
        rod.point1.forces = externalForce;
    }

    //Next, apply the spring correction forces. For each spring, skip over the spring if that spring's constraint type is currently disabled. You can check this using cp, which has boolean values such as enable_structural_constraints. Otherwise, compute the force applied to the two masses on its ends using Hooke's law:
    for (Spring s : springs) {
        if ((s.spring_type == STRUCTURAL && cp->enable_structural_constraints)
            || (s.spring_type == SHEARING && cp->enable_shearing_constraints)
            || (s.spring_type == BENDING && cp->enable_bending_constraints)) {
            Vector3D ab = s.pm_b->position - s.pm_a->position;
            double springMag = cp->ks * (ab.norm() - s.rest_length);
            if (s.spring_type == BENDING) {
                springMag *= 0.2;
            }
            Vector3D springForce = ab * springMag / ab.norm();
            s.pm_a->forces += springForce;
            s.pm_b->forces -= springForce;
        }
    }
    //ROD STUFF
    //find the forces acting on the rods (just wind for now) 
    for (Rod rod: rods) {
        float consC = 0.5;
        Vector3D windDirV = Vector3D(1, 1, 1);
        Vector3D v0 = (1 - cp->damping / 100) * (rod.point0.position - rod.point0.last_position);
        Vector3D normal0 = rod.point0.normal();
        Vector3D inside0 = dot(normal0, windDirV - v0) * normal0;
        Vector3D windForce0 = consC * inside0;
        rod.point0.forces += windForce0;
        rod.point0.last_position = rod.point0.position;
        rod.point0.position = rod.point0.position + v0 + rod.point0.forces * delta_t * delta_t / mass;


        Vector3D v1 = (1 - cp->damping / 100) * (rod.point1.position - rod.point1.last_position);
        Vector3D normal1 = rod.point1.normal();
        Vector3D inside1 = dot(normal1, windDirV - v1) * normal1;
        Vector3D windForce1 = consC * inside1;
        rod.point1.forces += windForce1;
        rod.point1.last_position = rod.point1.position;
        rod.point1.position = rod.point1.position + v1 + rod.point1.forces * delta_t * delta_t / mass;
    }
    bool unpinExists = false;
    Vector3D torqueForceAdd = Vector3D(0,0,0);
    int step = 0; 
    int start = -1;
    int end = -1;
    if (directionUp) {
        step = 1; 
        start = 0;
        end = rods.size();
    }
    else {
        step = -1; 
        start = rods.size() - 1;
        end = 0;
    }
    int ind = start; 
    for (; ind < end; ind += step) {
        Rod curr = rods[ind];
        //find last pinned rod
        //first
        if (!curr.pinned) {
            unpinExists = true;
            Vector3D fT;
            int next = -1;
            if (ind >= 1 && ind < rods.size() - 1 && directionUp) {
                curr.pivotPt = curr.point1;
                fT = curr.point1.forces;
                next = ind + 1;
            }
            else if (ind >= 1) {
                fT = curr.point0.forces;
                curr.pivotPt = curr.point0;
                next = ind - 1;
            }

            torqueForceAdd = curr.forceTorque(rods[next]) * fT;
            break; 
        }
    }
    if (!unpinExists) {
        //the crease has traversed the whole way, now flip
        rods[ind].pinned = false;
        directionUp = !directionUp;
    }

    for (PointMass& pointMass : point_masses) {
        if (!pointMass.pinned) {
            Vector3D v = (1 - cp->damping / 100) * (pointMass.position - pointMass.last_position);
            Vector3D normal = pointMass.normal();
            //represents strength of wind
            float consC = 0.5;
            //can change this, maybe put in cloth params
            Vector3D windDirV = Vector3D(1, 1, 1);
            Vector3D inside = dot(normal, windDirV - v) * normal;
            Vector3D windForce = consC * inside;
            pointMass.forces += windForce;
            //ROD STUFF 
            if (!(torqueForceAdd==Vector3D(0,0,0))){
                pointMass.forces += torqueForceAdd;
            }
            pointMass.last_position = pointMass.position;
            pointMass.position = pointMass.position + v + pointMass.forces * delta_t * delta_t / mass;
        }
    }




    // TODO (Part 4): Handle self-collisions.
    build_spatial_map();
    for (PointMass& pm : point_masses) {
        self_collide(pm, 3);
    }



    // TODO (Part 3): Handle collisions with other primitives.
    for (PointMass& pm : point_masses) {
        for (CollisionObject* co : *collision_objects) {
            co->collide(pm);
        }
    }


    // TODO (Part 2): Constrain the changes to be such that the spring does not change
    // in length more than 10% per timestep [Provot 1995].
    for (Spring& spring : springs) {
        Vector3D connect = spring.pm_b->position - spring.pm_a->position;
        double length = connect.norm();
        if (length > 1.1 * spring.rest_length) {
            if (!spring.pm_a->pinned && !spring.pm_b->pinned) {
                //Perform half of the correction to each point mass
                double fraction = 0.5 * (1 - 1.1 * spring.rest_length / length);
                spring.pm_a->position += fraction * connect;
                spring.pm_b->position -= fraction * connect;
            }
            //one is pinned do entirely on that one
            else if (spring.pm_a->pinned && !spring.pm_b->pinned) {
                double fraction = 1 - 1.1 * spring.rest_length / length;
                spring.pm_b->position -= fraction * connect;
            }
            else if (!spring.pm_a->pinned && spring.pm_b->pinned) {
                double fraction = 1 - 1.1 * spring.rest_length / length;
                spring.pm_a->position += fraction * connect;
            }
        }
    }
}

void Cloth::build_spatial_map() {
    for (const auto& entry : map) {
        delete(entry.second);
    }
    map.clear();
    // TODO (Part 4): Build a spatial map out of all of the point masses.
    for (PointMass& p : point_masses) {
        float key = hash_position(p.position);
        if (map.count(key) > 0) {
            vector<PointMass*>* val = map.at(key);
            val->push_back(&p);
        }
        else {
            vector<PointMass*>* val = new vector<PointMass*>;
            val->push_back(&p);
            map.insert({ key, val });
        }
    }
}

void Cloth::self_collide(PointMass& pm, double simulation_steps) {
    // TODO (Part 4): Handle self-collision for a given point mass.
    float key = hash_position(pm.position);
    Vector3D correction_vector = Vector3D(0, 0, 0);
    int count = 0;

    if (map.count(key) != 0) {
        for (PointMass* p : *map[key]) {
            if (&pm != p) {
                Vector3D diff = pm.position - p->position;
                if (diff.norm() < 2 * thickness) {
                    correction_vector = correction_vector + ((2 * thickness - diff.norm()) * diff.unit());
                    count++;
                }
            }
        }
        if (count > 0) {
            Vector3D correction = ((correction_vector / count) / simulation_steps);
            pm.position = pm.position + correction;
        }
    }
}

float Cloth::hash_position(Vector3D pos) {
    // TODO (Part 4): Hash a 3D position into a unique float identifier that represents membership in some 3D box volume.

    //uniquely mash to position –> 3 chosen for spacing element
    double w = 3 * width / num_width_points;
    double h = 3 * height / num_height_points;
    double t = max(w, h);

    return (float)((pos.x - fmod(pos.x, w)) / w) + ((pos.y - fmod(pos.y, h)) / h) * 10000 + ((pos.z - fmod(pos.z, t)) / t) * 1000000000;
    //  return floor((pos.x/w*3) + (pos.y/h*3 * pow(3, 2)) + (pos.z/t*3 * pow(3,3)));
}

///////////////////////////////////////////////////////
/// YOU DO NOT NEED TO REFER TO ANY CODE BELOW THIS ///
///////////////////////////////////////////////////////

void Cloth::reset() {
    PointMass* pm = &point_masses[0];
    for (int i = 0; i < point_masses.size(); i++) {
        pm->position = pm->start_position;
        pm->last_position = pm->start_position;
        pm++;
    }
}

void Cloth::buildClothMesh() {
    if (point_masses.size() == 0) return;

    ClothMesh* clothMesh = new ClothMesh();
    vector<Triangle*> triangles;

    // Create vector of triangles
    for (int y = 0; y < num_height_points - 1; y++) {
        for (int x = 0; x < num_width_points - 1; x++) {
            PointMass* pm = &point_masses[y * num_width_points + x];
            // Get neighboring point masses:
            /*                      *
             * pm_A -------- pm_B   *
             *             /        *
             *  |         /   |     *
             *  |        /    |     *
             *  |       /     |     *
             *  |      /      |     *
             *  |     /       |     *
             *  |    /        |     *
             *      /               *
             * pm_C -------- pm_D   *
             *                      *
             */

            float u_min = x;
            u_min /= num_width_points - 1;
            float u_max = x + 1;
            u_max /= num_width_points - 1;
            float v_min = y;
            v_min /= num_height_points - 1;
            float v_max = y + 1;
            v_max /= num_height_points - 1;

            PointMass* pm_A = pm;
            PointMass* pm_B = pm + 1;
            PointMass* pm_C = pm + num_width_points;
            PointMass* pm_D = pm + num_width_points + 1;

            Vector3D uv_A = Vector3D(u_min, v_min, 0);
            Vector3D uv_B = Vector3D(u_max, v_min, 0);
            Vector3D uv_C = Vector3D(u_min, v_max, 0);
            Vector3D uv_D = Vector3D(u_max, v_max, 0);


            // Both triangles defined by vertices in counter-clockwise orientation
            triangles.push_back(new Triangle(pm_A, pm_C, pm_B,
                uv_A, uv_C, uv_B));
            triangles.push_back(new Triangle(pm_B, pm_C, pm_D,
                uv_B, uv_C, uv_D));
        }
    }

    // For each triangle in row-order, create 3 edges and 3 internal halfedges
    for (int i = 0; i < triangles.size(); i++) {
        Triangle* t = triangles[i];

        // Allocate new halfedges on heap
        Halfedge* h1 = new Halfedge();
        Halfedge* h2 = new Halfedge();
        Halfedge* h3 = new Halfedge();

        // Allocate new edges on heap
        Edge* e1 = new Edge();
        Edge* e2 = new Edge();
        Edge* e3 = new Edge();

        // Assign a halfedge pointer to the triangle
        t->halfedge = h1;

        // Assign halfedge pointers to point masses
        t->pm1->halfedge = h1;
        t->pm2->halfedge = h2;
        t->pm3->halfedge = h3;

        // Update all halfedge pointers
        h1->edge = e1;
        h1->next = h2;
        h1->pm = t->pm1;
        h1->triangle = t;

        h2->edge = e2;
        h2->next = h3;
        h2->pm = t->pm2;
        h2->triangle = t;

        h3->edge = e3;
        h3->next = h1;
        h3->pm = t->pm3;
        h3->triangle = t;
    }

    // Go back through the cloth mesh and link triangles together using halfedge
    // twin pointers

    // Convenient variables for math
    int num_height_tris = (num_height_points - 1) * 2;
    int num_width_tris = (num_width_points - 1) * 2;

    bool topLeft = true;
    for (int i = 0; i < triangles.size(); i++) {
        Triangle* t = triangles[i];

        if (topLeft) {
            // Get left triangle, if it exists
            if (i % num_width_tris != 0) { // Not a left-most triangle
                Triangle* temp = triangles[i - 1];
                t->pm1->halfedge->twin = temp->pm3->halfedge;
            }
            else {
                t->pm1->halfedge->twin = nullptr;
            }

            // Get triangle above, if it exists
            if (i >= num_width_tris) { // Not a top-most triangle
                Triangle* temp = triangles[i - num_width_tris + 1];
                t->pm3->halfedge->twin = temp->pm2->halfedge;
            }
            else {
                t->pm3->halfedge->twin = nullptr;
            }

            // Get triangle to bottom right; guaranteed to exist
            Triangle* temp = triangles[i + 1];
            t->pm2->halfedge->twin = temp->pm1->halfedge;
        }
        else {
            // Get right triangle, if it exists
            if (i % num_width_tris != num_width_tris - 1) { // Not a right-most triangle
                Triangle* temp = triangles[i + 1];
                t->pm3->halfedge->twin = temp->pm1->halfedge;
            }
            else {
                t->pm3->halfedge->twin = nullptr;
            }

            // Get triangle below, if it exists
            if (i + num_width_tris - 1 < 1.0f * num_width_tris * num_height_tris / 2.0f) { // Not a bottom-most triangle
                Triangle* temp = triangles[i + num_width_tris - 1];
                t->pm2->halfedge->twin = temp->pm3->halfedge;
            }
            else {
                t->pm2->halfedge->twin = nullptr;
            }

            // Get triangle to top left; guaranteed to exist
            Triangle* temp = triangles[i - 1];
            t->pm1->halfedge->twin = temp->pm2->halfedge;
        }

        topLeft = !topLeft;
    }

    clothMesh->triangles = triangles;
    this->clothMesh = clothMesh;
}
