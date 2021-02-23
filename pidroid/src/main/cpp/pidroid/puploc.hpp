//
// Created by adrian on 24/01/21.
//
#pragma once
#include <vector>
#include <random>
#include <cstdio>
#include "commons.hpp"

using namespace std;

namespace pidroidlib {
    struct PuplocDetection {
        int row;
        int col;
        int scale;
        int perturbs;
    } ;

    struct PuplocCascade {
        int stages;
        float scales;
        int trees;
        int treeDepth;
        int* treeCodes;
        float* treePreds;
    } ;

    class Puploc {
    private:
        PuplocCascade cascade{};
    public:
        Puploc();
        ~Puploc();
        void unpackCascade(std::vector<char> packet);
        void classifyRegion(int r, int c, int s, int nrows, int ncols, const uint8_t* pixels, int dim, bool flipV, int *res) const;
        void classifyRotatedRegion( int r, int c, int s, float angle, int nrows, int ncols, const uint8_t* pixels, int dim, bool flipV, int *res) const;
        PuplocDetection runDetector(PuplocDetection pl, int rows, int cols, const uint8_t* pixels, int dim, float angle, bool flipV) const;
    };



}
