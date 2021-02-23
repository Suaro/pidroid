//
// Created by adrian on 21/02/21.
//

#include "commons.hpp"

namespace pidroidlib {

    int Commons::buffToInteger(std::vector<char> buffer) {
        int a = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 24 |
                                 static_cast<unsigned char>(buffer[1]) << 16 |
                                 static_cast<unsigned char>(buffer[2]) << 8 |
                                 static_cast<unsigned char>(buffer[3]));
        return a;
    }


    float Commons::bytesToFloatLittleEndian(std::vector<char> buffer) {
        float output;

        *((unsigned char *) (&output) + 3) = buffer[0];
        *((unsigned char *) (&output) + 2) = buffer[1];
        *((unsigned char *) (&output) + 1) = buffer[2];
        *((unsigned char *) (&output) + 0) = buffer[3];

        return output;
    }


    float Commons::bytesToFloatBigEndian(std::vector<char> buffer) {
        float output;

        *((unsigned char *) (&output) + 3) = buffer[3];
        *((unsigned char *) (&output) + 2) = buffer[2];
        *((unsigned char *) (&output) + 1) = buffer[1];
        *((unsigned char *) (&output) + 0) = buffer[0];

        return output;
    }
}