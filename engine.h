#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <cmath>
#include <time.h>
#include <string>
#include <algorithm>

enum rule{velocity_halt, time_control};

class engine { // values set to long double for precesion
protected:
    long unsigned int t; // counter, always ++
    float a;
    long double v = 0.0; // final velocity
    long double u = 0.0;
    long double s = 0.0; // distance
    long double prev_s = 0.0;
    int b_t = 0; // brake time
    float n_a; // deceleration, entered position
    int tick = 1;
    bool b_flag = true;
    long unsigned int t_start = 0;   // Time when current phase began
    long double u_start = 0;   // Velocity when current phase began
    long double s_start = 0;   // Distance when current phase began
    bool tar_flag = false; //check if random target set
    bool no_deccel = false; //check for decceleration flag, if user has ENTERED or not. if enetered, 0 is acceptable!
    int tar_dis = 0;
public:
    long unsigned int get_time();
    float get_velocity();
    float get_dis();
    void calc_dis();
    void calc_accel();
    void tick_time();
    void calc_velocity();
    void set_time(float time);
    void reset();
    void set_values(bool deccel_maneuver=true, bool acceleration=true, bool velocity=true, bool decceleration=true);
    void target();
    void log();
    bool sim_obj(rule state_rule);
    engine(float acceleration, float initial_velocity, float brake_t, float n_a);
    engine(){a = 0; u = 0; u_start = 0; b_t = 0; n_a = 0;};

};

inline engine::engine(float acceleration, float initial_velocity, float brake_t, float decceleration) {
    a = acceleration;
    u = initial_velocity;
    u_start = initial_velocity;
    b_t = brake_t;
    float max_decel = 980;
    n_a = decceleration;
    n_a = std::min(n_a, max_decel);
}

inline void engine::calc_accel() {
    if (b_t == 0) a = n_a;
    if (t == b_t*1000 && b_flag) {
        u_start = u + (a * (long double)b_t); 
        s_start = (u * b_t) + (0.5L * a * b_t * b_t);
        
        t_start = b_t*1000;
        
        a = n_a;
        b_flag = false;
    }
}

inline void engine::calc_velocity() {
    long unsigned int t_phase = t - t_start; 
    v = u_start + (a * t_phase/1000.0L); 
    if (v <= 0.0L) v = 0;
}

inline void engine::calc_dis() {
    long unsigned int t_phase = t - t_start;
    s = s_start + (u_start * t_phase/1000.0L + (0.5L * a * t_phase/1000.0L * t_phase/1000.0L));
}

inline long unsigned int engine::get_time() {
    return t;
}

inline float engine::get_dis() {
    return s;
}

inline void engine::tick_time() {
    t+=tick;
}

inline float engine::get_velocity() {
    return v;
}

inline void engine::set_time(float time) {
    t = time;
}

inline void engine::reset(){
    t = s = v = 0;
    b_flag = true;
    no_deccel = false;
    t_start = s_start = 0;
    u_start = u;
}

inline void engine::set_values(bool deccel_maneuver, bool acceleration, bool velocity, bool decceleration) { //asks user to input various motion variables
    reset();
    std::string input;
    float U, A, N_a;
    int B_t;

    auto get_safe_float = [&](std::string prompt) {
        float val;
        while (true) {
            std::cout << prompt;
            std::cin >> input;
            try {
                val = stof(input);
                return val;
            } catch (...) {
                std::cout << "> Invalid input. Please enter a number.\n";
            }
        }
    };
    
    if (velocity) {U = get_safe_float( "> Enter the initial velocity (m/s)       : ");} else U = 0;
    if (acceleration) {A = get_safe_float( "> Enter the acceleration (m/s²)          : ");} else A = 0;
    
    // For the integer Brake Time
    if (deccel_maneuver) {
        while (true) {
            std::cout << "> Enter the maneuver time (seconds)      : ";
            std::cin>>input;
            try {
                B_t = stoi(input);
                break;
            } catch (...) {
                std::cout << "> Invalid input. Please enter an integer.\n";
            }
        }
    }else B_t = 0;

    if(decceleration) {N_a = get_safe_float("> Enter the decceleration (+ve)          : "); no_deccel = false;}
    else {N_a = 0; no_deccel = true;}

    u = U;
    u_start = U; // Set the starting phase velocity
    a = A;
    b_t = B_t;
    n_a = -std::fabs(N_a); // Ensure it's negative for the math

    std::cout <<
    "\nParameters:\n " <<
    "\nInitial Velocity   = " << u <<
    "\nAcceleration       = " << a <<
    "\nDecceleration time = " << b_t <<
    "\nDecceleration      = " << n_a << "\n\n";

}

inline void engine::target(){
    int TIME = time(0);
    srand(TIME);
    tar_dis = 1 + rand() % 10000;

    std::cout << "\n-- Target distance: " << tar_dis << " m --\n\n";
    tar_flag = true;
}

inline void engine::log() {    
    float velocity;
    float distance;
    int print_accumulator = 0;

    do {
        tick_time();
        calc_accel();
        calc_velocity();
        calc_dis();    

        distance = get_dis();
        velocity = get_velocity();

        print_accumulator++;

        if (print_accumulator == 1000 || velocity == 0) {
            print_accumulator = 0;
            std::cout << " sec " << get_time()/1000.0L << "| metres " << distance << "| velocity: " << velocity << "m/s";

            if (t == b_t*1000 && !no_deccel) {
                std::cout << "  *DECCELERATION MANEUVER SUCCESSFUL!*" << std::endl;
            } else {
                std::cout << std::endl;
            }

            if (!(sim_obj(velocity_halt) && sim_obj(time_control))) {
                std::cout << " *STOPPED*"<< " @ " << get_dis() << "m\n";
                if(tar_flag) 
                {
                    tar_flag = false;
                    std::cout << " Distance from destination: " << fabsl(tar_dis - get_dis()) << " m\n";
                    if (fabsl(tar_dis - get_dis()) <= 20) 
                    {
                        std::cout << " *** WELL DONE!! ***\n";
                    }
                }
            }
        }
    }while (sim_obj(velocity_halt) && sim_obj(time_control));
    reset();
}

inline bool engine::sim_obj(rule state_rule){
    switch(state_rule){
        case velocity_halt:
            return (v != 0); break;
        case time_control:
            return !(t >= b_t*1000 && no_deccel);
    }
    return true;
}

#endif