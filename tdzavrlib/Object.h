//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef MINECRAFT_3DZAVR_OBJECT_H
#define MINECRAFT_3DZAVR_OBJECT_H

#include <vector>
#include "utils/Point4D.h"

class Object {
protected:
    std::vector<Object*> v_attached;

    Point4D p_position;
    Point4D p_angle;
public:
    Object() = default;

    virtual void translate(const Point4D& dv) {}
    virtual void rotate(const Point4D& r) {}

    void attach(Object* mesh) {
        v_attached.push_back(mesh);
    }
};


#endif //MINECRAFT_3DZAVR_OBJECT_H
