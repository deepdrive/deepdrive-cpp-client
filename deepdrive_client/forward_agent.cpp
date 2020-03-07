/**
 * Basic example of how to run an agent that floors the gas.
 */

#include <iostream>
#include "deepdrive_client.hpp"

int main(int argc, char* argv[]) {
    bool verbose = false;
    if(argc == 2) {
        std::string arg1 = argv[1];
        if(arg1 == "-v") {
            verbose = true;
        }
    }

    deepdrive::DeepdriveClient env(verbose);
    auto done = false;
    for (int i = 0; i < 2; i ++) {
        while ( ! done) {
            auto step_info = env.step(0, 1, 0, 0, true);
            if(verbose) {
                std::cout << step_info["reward"].GetFloat() << std::endl;
                if(step_info.HasMember("observation") && step_info["observation"].IsObject()) {
                    std::cout << "speed " << step_info["observation"]["speed"].GetFloat() / 100 << std::endl;
                }
            }
            done = step_info["done"].GetBool();
        }
        env.reset();
        std::cout << "Episode finished" << std::endl;
        done = false;
    }
    std::cout << "Done" << std::endl;
}
