/**
 * Basic example of how to run an agent that floors the gas.
 */

#include <iostream>
#include "deepdrive_client.hpp"

int main() {
    deepdrive::DeepdriveClient env;
    auto done = false;
    while (true) {
        while ( ! done) {
            auto observation = env.step(env.get_action(0, 1, 0, 0, true));
            std::cout << observation["speed"].GetInt() << std::endl;
            done = observation["done"].GetBool();
        }
        env.reset();
        std::cout << "Episode finished" << std::endl;
        done = false;
    }
}