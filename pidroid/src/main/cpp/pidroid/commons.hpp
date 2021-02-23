//
// Created by adrian on 21/02/21.
//

#pragma once
#include <vector>

using namespace std;

namespace pidroidlib {
    class Commons {
    public:
        static int buffToInteger(std::vector<char> buffer);

        static float bytesToFloatLittleEndian(std::vector<char> buffer);

        static float bytesToFloatBigEndian(std::vector<char> buffer);
    };
}