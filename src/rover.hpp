#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

namespace godot {
    class Rover : public RigidBody3D {
        GDCLASS(Rover, RigidBody3D);

    protected:
        static void _bind_methods();

    public:
        Rover();
        ~Rover();

        void _physics_process(double delta) override;

        void set_speed(double new_speed);
        double get_speed() const;

        void set_rotation_speed(double new_rotation_speed);
        double get_rotation_speed() const;

        double get_integrity() const;
        double set_integrity(double new_integrity);

        double get_acceleration() const;
        double set_acceleration(double new_acceleration);

        void set_spring_strength(double value);
        double get_spring_strength() const;

        void set_spring_damper(double value);
        double get_spring_damper() const;
    private:
        double suspension_rest_length{0.5};
        double spring_strength{150.0};
        double spring_damping{10.0};

        double speed{20.0};
        double acceleration{2.0};
        double rotation_speed{1.5};
        double integrity{1.0};

        std::vector<Vector3> wheel_positions;

        void _apply_suspension_force(int wheel_idx, double delta);
    };
}