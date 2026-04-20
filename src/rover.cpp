#include "rover.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void Rover::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_speed", "new_speed"), &Rover::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &Rover::get_speed);
    ClassDB::bind_method(D_METHOD("set_rotation_speed", "new_rotation_speed"), &Rover::set_rotation_speed);
    ClassDB::bind_method(D_METHOD("get_rotation_speed"), &Rover::get_rotation_speed);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed"), "set_rotation_speed", "get_rotation_speed");
}

Rover::Rover() {
    UtilityFunctions::print("Rover created");
    speed = 10.0; // Default speed
}

Rover::~Rover() {
    UtilityFunctions::print("Rover destroyed");
}

void Rover::_process(double delta) {
    Input *input = Input::get_singleton();
    Vector2 input_direction = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");

    Vector3 direction = Vector3(input_direction.x, 0, input_direction.y).normalized();

    if (direction.length() > 0.1) {
        Basis target_basis = Basis::looking_at(direction, Vector3(0, 1, 0));

        Quaternion current_quaternion = get_quaternion();
        Quaternion target_quaternion = target_basis.get_quaternion();

        double weight = delta * rotation_speed;
        Quaternion slerped_quat = current_quaternion.slerp(target_quaternion, weight);

        set_quaternion(slerped_quat);

        set_velocity(direction * speed);
    } else {
        set_velocity(get_velocity().lerp(Vector3(0, 0, 0), 0.1));
    }

    move_and_slide();
}

void Rover::set_speed(double new_speed) {
    speed = new_speed;
}

double Rover::get_speed() const {
    return speed;
}

void Rover::set_rotation_speed(double new_rotation_speed) {
    rotation_speed = new_rotation_speed;
}

double Rover::get_rotation_speed() const {
    return rotation_speed;
}