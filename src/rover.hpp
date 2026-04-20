#pragma once

#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {
    class Rover : public CharacterBody3D {
        GDCLASS(Rover, CharacterBody3D);

    protected:
        static void _bind_methods();

    public:
        Rover();
        ~Rover();

        void _process(double delta) override;

        void set_speed(double new_speed);
        double get_speed() const;

        void set_rotation_speed(double new_rotation_speed);
        double get_rotation_speed() const;

    private:
        double speed{};
        double rotation_speed{10.0};
    };
}