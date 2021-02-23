//
// Created by adrian on 24/01/21.
//
#pragma once
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include "puploc.hpp"
using namespace std;

namespace pidroidlib {
    struct FlpCascade {
        std::map<std::string, Puploc> puplocCascade;
    };

    struct FlpDetection {
        std::vector<PuplocDetection> detections;
    };

    class Flploc {
    private:
        FlpCascade cascade;
        std::vector<std::string> eyeCascades;
        std::vector<std::string> mouthCascades;
    public:
        Flploc();
        ~Flploc();
        void addCascade(std::string cascadeName, std::vector<char> packet);
        static PuplocDetection getLandmarkPoint(const Puploc& detector, PuplocDetection leftEye, PuplocDetection rightEye, int nrows, int ncols, const uint8_t* pixels, int dim, int perturb, bool flipV);
        FlpDetection getLandmarkPoints(PuplocDetection leftEye, PuplocDetection rightEye, int nrows, int ncols, const uint8_t* pixels, int dim, int perturb);
    };
}