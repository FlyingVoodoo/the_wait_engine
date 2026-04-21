#include "rover.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/physics_material.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>

using namespace godot;

void Rover::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_speed", "new_speed"), &Rover::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &Rover::get_speed);
    
    ClassDB::bind_method(D_METHOD("set_rotation_speed", "new_rotation_speed"), &Rover::set_rotation_speed);
    ClassDB::bind_method(D_METHOD("get_rotation_speed"), &Rover::get_rotation_speed);

    ClassDB::bind_method(D_METHOD("set_integrity", "new_integrity"), &Rover::set_integrity);
    ClassDB::bind_method(D_METHOD("get_integrity"), &Rover::get_integrity);

    ClassDB::bind_method(D_METHOD("set_spring_strength", "value"), &Rover::set_spring_strength);
    ClassDB::bind_method(D_METHOD("get_spring_strength"), &Rover::get_spring_strength);
    
    ClassDB::bind_method(D_METHOD("set_spring_damper", "value"), &Rover::set_spring_damper);
    ClassDB::bind_method(D_METHOD("get_spring_damper"), &Rover::get_spring_damper);

    ADD_GROUP("Movement", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed"), "set_rotation_speed", "get_rotation_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "integrity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_integrity", "get_integrity");

    ADD_GROUP("Suspension", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_strength"), "set_spring_strength", "get_spring_strength");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_damper"), "set_spring_damper", "get_spring_damper");
}
Rover::Rover() {
    // wheels positions relative to the center(tbh comment to remember the order of this things)
    wheel_positions.push_back(Vector3(1, 0, 1)); // front left
    wheel_positions.push_back(Vector3(-1, 0, 1)); // front right
    wheel_positions.push_back(Vector3(1, 0, -1)); // rear left
    wheel_positions.push_back(Vector3(-1, 0, -1)); // rear right

}

Rover::~Rover() {
    UtilityFunctions::print("Rover destroyed");
}

void Rover::_physics_process(double delta) {
    for (size_t i = 0; i < wheel_positions.size(); ++i) {
        _apply_suspension_force(i, delta);
    }


    Input *input = Input::get_singleton();
    double forward_input = input->get_axis("ui_down", "ui_up");
    double steer_input = input->get_axis("ui_left", "ui_right");

    Vector3 forward_direction = get_global_transform().get_basis().get_column(2);

    apply_central_force(forward_direction * speed * forward_input * integrity * 100.0);

    apply_torque(Vector3(0, steer_input * rotation_speed * integrity * 100.0, 0));
}

void Rover::_apply_suspension_force(int wheel_idx, double delta) {
    PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();

    Vector3 wheel_global_pos = get_global_transform().xform(wheel_positions[wheel_idx]);
    Vector3 ray_start = wheel_global_pos;
    Vector3 ray_end = wheel_global_pos + get_global_transform().get_basis().get_column(1) * -suspension_rest_length * 2.0;

    auto query = PhysicsRayQueryParameters3D::create(ray_start, ray_end);
    query->set_exclude({get_rid()});

    Dictionary result = space_state->intersect_ray(query);

    if (!result.is_empty()) {
        float hit_distance = ray_start.distance_to(result["position"]);

        // Law of Hooke: F = -kx - bv
        float offset = suspension_rest_length - hit_distance;

        Vector3 up_direction = get_global_transform().get_basis().get_column(1);
        float velocity_at_wheel = get_linear_velocity().dot(up_direction);

        float spring_force = offset * spring_strength;
        float damping_force = -velocity_at_wheel * spring_damping;
        float total_force = Math::max(spring_force + damping_force, 0.0f); // Prevent downward force

        Vector3 force_vector = up_direction * total_force;
        apply_force(force_vector, wheel_global_pos - get_global_transform().origin);
    }

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

double Rover::get_integrity() const {
    return integrity;
}

double Rover::set_integrity(double new_integrity) {
    integrity = new_integrity;
    return integrity;
}

double Rover::get_acceleration() const {
    return acceleration;
}

double Rover::set_acceleration(double new_acceleration) {
    acceleration = new_acceleration;
    return acceleration;
}

void Rover::set_spring_strength(double value) {
    spring_strength = value;
}

double Rover::get_spring_strength() const {
    return spring_strength;
}

void Rover::set_spring_damper(double value) {
    spring_damping = value;
}


double Rover::get_spring_damper() const {
    return spring_damping;
}

