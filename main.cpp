#include <iostream>
#include <cmath>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <limits>
#include "engine.h"

using namespace std;

void init() {
    cout
        << " ======================================= \n"
        << " Welcome to the Voyage - A Long Journey \n"
        << " ======================================= \n \n";
    cout << "> Tutorial (T)\n";
    cout << "> About (A) \n";
    cout << "> Enter (Any key)\n \n";
}

void tutorial() {
    cout
        << "\n=====================================================\n"
        << "                     THE VOYAGE DOCS\n"
        << "=====================================================\n\n"

        << "Lost in the silent void, your crew drifts toward the \n"
        << "unknown.\n"
        << "The control of the rover's engines now rests in your\n"
        << "hands.\n"
        << "Can you reach the target in a single shot?\n\n"

        << "Motion Model:\n"
        << "  • Velocity evolves continuously under acceleration.\n"
        << "  • Distance follows real Newtonian motion equations.\n"
        << "  • When deceleration begins, a new motion phase is \n"
        << "    created.\n\n"

        << "-----------------------------------------------------\n"
        << "PARAMETERS\n"
        << "-----------------------------------------------------\n\n"

        << "1 • Initial Velocity (m/s)\n"
        << "    Starting speed of your rover.\n\n"

        << "2 • Acceleration (m/s²)\n"
        << "    Rate at which velocity increases over time.\n\n"

        << "3 • Maneuver Time (seconds)\n"
        << "    Moment when the deceleration maneuver is \n"
        << "    initiated.\n"
        << "    A new motion phase begins at this instant.\n\n"

        << "4 • Deceleration (positive value)\n"
        << "    Strength of braking thrust.\n"
        << "    Maximum decceleration thruster capacity: 100g.\n\n"

        << "-----------------------------------------------------\n"
        << "ROLES\n"
        << "-----------------------------------------------------\n\n"

        << "1 • SANDBOX\n"
        << "    Full control over all parameters.\n"
        << "    Experiment freely and explore motion behavior.\n\n"

        << "2 • INTERN\n"
        << "    A target distance is generated.\n"
        << "    Only initial velocity and maneuver time are\n"
        << "    adjustable.\n\n"

        << "3 • HEAD PILOT\n"
        << "    Choose velocity and braking force.\n"
        << "    No maneuver delay — deceleration begins\n"
        << "    immediately.\n"
        << "    Tests precision stopping control.\n\n"

        << "4 • MISSION CONTROLLER\n"
        << "    Full trajectory planning.\n"
        << "    Accelerate first, then brake at the chosen time\n"
        << "    to reach the target distance accurately.\n\n"

        << "-----------------------------------------------------\n"
        << "GOAL\n"
        << "-----------------------------------------------------\n\n"

        << "Reach as close as possible to the target distance.\n"
        << "Stopping within ±20 metres counts as success.\n\n"

        << "Tip:\n"
        << "  Smooth motion planning beats brute force.\n"
        << "  Think in physics — not guesses.\n\n"

        << "=====================================================\n\n";
}


void credits() {
    cout
        << "=========================================================================================\n"
        << "This project is an attempt to implement simple Newtonian motion physics using a C++ based \n"
        << "simulation engine.\n"
        << "A reusable physics engine class was designed for this project.\n"
        << "Developed as an exercise in C++ fluency and system design.\n"
        << "=========================================================================================\n\n"

        << "Developed by : Agrim Sharma (CosmicDev404)\n"
        << "Repository   : https://github.com/cosmicdev404/voyage_sim\n\n";
}

int get_number()
{
    string buffer;

    while (true) {
        cin >> buffer;

        try {
            return stoi(buffer);
        }
        catch (...) {
            cout << ">> Invalid input. Please enter a number.\n";
        }
    }
}

int main() {

    string buffer;
    char login;
    char runner;
    int choice = 0;
    SetConsoleOutputCP(CP_UTF8);

    engine rover; // simulation subject

    init(); // welcome screen
    cin >> buffer;
    login = buffer[0];
    buffer = "";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if ((login == 't' || login == 'T')) { // tutorial checker
        tutorial();
    } else if ((login == 'a' || login == 'A')) { // credits
        credits();
    }

    do {
        cout << "\nChoose the mode from following options:\n" <<
            "1.  Sandbox\n" <<
            "2.  Intern\n"
            "3.  Head Pilot\n"
            "4   Mission Controller\n\n";
            //cout << "5.  Planet Diver\n\n";!
        
        choice = get_number();

        switch (choice) {
            case 1: cout << "\n* SANDBOX *\n\n"; rover.set_values(); rover.log(); break;
            case 2: cout << "\n* INTERN *\n";rover.target(); rover.set_values(true, false, true, false); rover.log(); break; 
            case 3: cout << "\n* HEAD PILOT *\n"; rover.target(); rover.set_values(false, false); rover.log(); break;
            case 4: cout << "\n * MISSION CONTROLLER *\n"; rover.target(); rover.set_values(true, true, false, true); rover.log(); break;
            default: cout << "Invalid choice!" << endl;
         }

        // runner conditions
        cout << "\n>Retry? (Y/N) "; cin >> buffer;
        runner = buffer[0];

    } while (runner != 'n' && runner != 'N');

    return 0;

}
