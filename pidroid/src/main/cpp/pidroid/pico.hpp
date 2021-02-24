//
// Created by adrian on 24/01/21.
//
#pragma once
#include <vector>
#include "commons.hpp"

using namespace std;

#define nmemslots 5
#define maxslotsize 1024

namespace pidroidlib {

    struct PicoDetection {
        int row;
        int col;
        int scale;
        float threshold;
    };

    struct PicoResult {
        int ndetections;
        PicoDetection detections[2048];
    };

    struct PicoCascade {
        int treeNum;
        int treeDepth;
        int* treeCodes;
        float* treePreds;
        float* treeThreshold;
    } ;

    struct CascadeParams {
        int minSize;
        int maxSize;
        float shiftFactorWidth;
        float shiftFactorHeight;
        float scaleFactor;
        float angle;
        float minThreshold;
    } ;

    class Pico {
    private:
        CascadeParams cascadeParams{};
        PicoCascade cascade{};

        int slot;
        float memory[4*nmemslots*maxslotsize]{};
        int counts[nmemslots]{};
    public:
        Pico();
        Pico(CascadeParams cascadeParams);
        ~Pico();
        void unpackPicoCascade(std::vector<char> packet);
        float classifyRegion(int r, int c, int s, int nrows, int ncols, const uint8_t* pixels, int dim) const;
        float classifyRotatedRegion(int r, int c, int s, float angle, int nrows, int ncols, const uint8_t* pixels, int dim) const;
        PicoResult RunCascade(int rows, int cols, const uint8_t* pixels, int dim) const;
        static PicoResult clusterDetections(PicoResult pl, float iouThreshold);

        const CascadeParams &getCascadeParams() const;

        void setCascadeParams(const CascadeParams &cascadeParams);

        PicoResult updateMemory(PicoResult pl);

        static PicoResult getProminentFace(PicoResult result);
    };
}

