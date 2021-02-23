//
// Created by adrian on 24/01/21.
//

#include "flploc.hpp"


namespace pidroidlib {

    void Flploc::addCascade(std::string cascadeName, std::vector<char> packet) {
        Puploc puploc = Puploc();
        puploc.unpackCascade(std::move(packet));
        this->cascade.puplocCascade.insert(std::make_pair(cascadeName, puploc));
    }

    FlpDetection
    Flploc::getLandmarkPoints(PuplocDetection leftEye, PuplocDetection rightEye, int nrows,
                              int ncols, const uint8_t *pixels, int dim, int perturb) {

        std::vector<PuplocDetection> landmarks = std::vector<PuplocDetection>();
        Puploc lpc;

        for (const auto &cascadeName : this->eyeCascades) {
            lpc = this->cascade.puplocCascade.at(cascadeName);
            landmarks.push_back(
                    pidroidlib::Flploc::getLandmarkPoint(lpc, leftEye, rightEye, nrows, ncols, pixels, dim,
                                           perturb, false));
            landmarks.push_back(
                    pidroidlib::Flploc::getLandmarkPoint(lpc, leftEye, rightEye, nrows, ncols, pixels, dim,
                                           perturb, true));
        }

        for (const auto &cascadeName : this->mouthCascades) {
            lpc = this->cascade.puplocCascade.at(cascadeName);
            landmarks.push_back(
                    pidroidlib::Flploc::getLandmarkPoint(lpc, leftEye, rightEye, nrows, ncols, pixels, dim,
                                           perturb, false));
        }

        lpc = this->cascade.puplocCascade.at("lp84");
        landmarks.push_back(
                pidroidlib::Flploc::getLandmarkPoint(lpc, leftEye, rightEye, nrows, ncols, pixels, dim, perturb,
                                       true));

        return {
                .detections = landmarks
        };
    }

    PuplocDetection Flploc::getLandmarkPoint(const Puploc& detector, PuplocDetection leftEye,
                                             PuplocDetection rightEye, int nrows,
                                             int ncols, const uint8_t *pixels, int dim, int perturb,
                                             bool flipV) {


        int dist1 = (leftEye.row - rightEye.row) * (leftEye.row - rightEye.row);
        int dist2 = (leftEye.col - rightEye.col) * (leftEye.col - rightEye.col);
        float dist = sqrt(float(dist1 + dist2));

        auto row = float((leftEye.row + rightEye.row) / 2.0 + 0.25 * dist);
        auto col = float((leftEye.col + rightEye.col) / 2.0 + 0.15 * dist);
        auto scale = 3.0f * dist;

        PuplocDetection flploc = {
                .row =      int(row),
                .col =    int(col),
                .scale =    int(scale),
                .perturbs = perturb
        };

        return detector.runDetector(flploc, nrows, ncols, pixels, dim, 0.0, flipV);
    }

    Flploc::Flploc() {
        this->eyeCascades = {"lp46", "lp44", "lp42", "lp38", "lp312"};
        this->mouthCascades = {"lp93", "lp84", "lp82", "lp81"};
    }

    Flploc::~Flploc() = default;
}

