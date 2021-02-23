//
// Created by adrian on 24/01/21.
//

#include "puploc.hpp"


namespace pidroidlib {

    void Puploc::unpackCascade(std::vector<char> packet) {
        int stages;
        float scales;
        int trees;
        int treeDepth;
        std::vector<int> treeCodes;
        std::vector<float> treePreds;
        //((int*)cascade)[2]
        int pos = 0;
        std::vector<char> buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1],
                                  (packet)[pos + 0]};
        stages = Commons::buffToInteger(buff);

        pos += 4;
        buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};

        scales = Commons::bytesToFloatLittleEndian(buff);

        pos += 4;
        buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};

        trees = Commons::buffToInteger(buff);

        pos += 4;
        buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};

        treeDepth = Commons::buffToInteger(buff);

        pos += 4;
        int depth = pow(2, treeDepth);

        for (int s = 0; s < stages; s++) {
            for (int t = 0; t < trees; t++) {
                std::vector<signed char> v2 = std::vector<signed char>(packet.begin() + pos,
                                                                       packet.begin() + pos +
                                                                       4 * depth - 4);
                treeCodes.insert(treeCodes.end(), v2.data(), v2.data() + 4 * depth - 4);
                pos += 4 * depth - 4;

                for (int i = 0; i < depth; i++) {
                    for (int l = 0; l < 2; l++) {
                        buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1],
                                (packet)[pos + 0]};

                        treePreds.push_back(Commons::bytesToFloatLittleEndian(buff));
                        pos += 4;
                    }
                }
            }
        }


        this->cascade = {.stages =  stages,
                .scales =  scales,
                .trees = trees,
                .treeDepth = treeDepth,
                .treeCodes = new int[treeCodes.size()],
                .treePreds = new float[treePreds.size()]
        };

        std::copy(treeCodes.begin(), treeCodes.end(), this->cascade.treeCodes);
        std::copy(treePreds.begin(), treePreds.end(), this->cascade.treePreds);

    }

    void
    Puploc::classifyRegion(int r, int c, int s, int nrows, int ncols, const uint8_t *pixels,
                           int dim, bool flipV, int *res) const {

        int c1, c2, root = 0, idx, r1, r2, lutIdx, i, j, k;
        float dr, dc;
        int treeDepth = pow(2, cascade.treeDepth);

        auto flipF = flipV ? -1 : 1;

        auto minR = nrows - 1;
        auto minC = ncols - 1;

        const auto stages = cascade.stages;
        const auto trees = cascade.trees;
        const auto scales = cascade.scales;
        const auto tdepth = cascade.treeDepth;
        const auto treeCodes = cascade.treeCodes;
        const auto treePreds = cascade.treePreds;

        for (i = 0; i < stages; ++i) {
            dr = 0.0, dc = 0.0;
            for (j = 0; j < trees; ++j) {
                idx = 0;
                for (k = 0; k < tdepth; ++k) {
                    r1 = min(minR,
                             max(0, (256 * r + treeCodes[root + 4 * idx] * s) >> 8));
                    c1 = min(minC, max(0, (256 * c + flipF * treeCodes[root + 4 * idx + 1] * s)  >> 8));
                    r2 = min(minR,
                             max(0, (256 * r + treeCodes[root + 4 * idx + 2] * s) >> 8));
                    c2 = min(minC, max(0, (256 * c + flipF * treeCodes[root + 4 * idx + 3] * s)
                            >> 8));

                    idx = 2 * idx + 1 + (pixels[r1 * dim + c1] > pixels[r2 * dim + c2]);
                }
                lutIdx =
                        2 * (trees * treeDepth * i + treeDepth * j + idx - (treeDepth - 1));

                dr += treePreds[lutIdx + 0];
                dc += flipF * treePreds[lutIdx + 1];
                root += 4 * treeDepth - 4;
            }
            r += int(dr * s);
            c += int(dc * s);
            s *= scales;
        }

        res[0] = r;
        res[1] = c;
        res[2] = s;
    }


    void Puploc::classifyRotatedRegion(int r, int c, int s, float angle, int nrows,
                                       int ncols, const uint8_t *pixels, int dim, bool flipV,
                                       int *res) const {
        int c1, c2, root = 0, idx, r1, r2, lutIdx, row1, col1, row2, col2;
        float dr, dc;
        int treeDepth = pow(2, cascade.treeDepth);

        int qCosTable[] = {256, 251, 236, 212, 181, 142, 97, 49, 0, -49, -97, -142, -181, -212,
                             -236, -251, -256, -251, -236, -212, -181, -142, -97, -49, 0, 49, 97,
                             142, 181, 212, 236, 251, 256};
        int qSinTable[] = {0, 49, 97, 142, 181, 212, 236, 251, 256, 251, 236, 212, 181, 142, 97,
                             49, 0, -49, -97, -142, -181, -212, -236, -251, -256, -251, -236, -212,
                             -181, -142, -97, -49, 0};

        int qsin = s * qSinTable[int(32.0 * angle)]; //s*(256.0*math.Sin(2*math.Pi*a))
        int qcos = s * qCosTable[int(32.0 * angle)]; //s*(256.0*math.Cos(2*math.Pi*a))

        auto flipF = flipV ? -1 : 1;

        const auto stages = cascade.stages;
        const auto trees = cascade.trees;
        const auto scales = cascade.scales;
        const auto tdepth = cascade.treeDepth;
        const auto treeCodes = cascade.treeCodes;
        const auto treePreds = cascade.treePreds;

        for (int i = 0; i < stages; i++) {
            dr = 0.0, dc = 0.0;
            for (int j = 0; j < trees; j++) {
                idx = 0;
                for (int k = 0; k < tdepth; k++) {
                    row1 = treeCodes[root + 4 * idx + 0];
                    row2 = treeCodes[root + 4 * idx + 2];

                    col1 = flipF*treeCodes[root + 4 * idx + 1];
                    col2 = flipF*treeCodes[root + 4 * idx + 3];

                    r1 = min(nrows - 1,
                             max(0, 65536 * int(r) + int(qcos) * row1 - int(qsin) * col1) >> 16);
                    c1 = min(ncols - 1,
                             max(0, 65536 * int(c) + int(qsin) * row1 + int(qcos) * col1) >> 16);
                    r2 = min(nrows - 1,
                             max(0, 65536 * int(r) + int(qcos) * row2 - int(qsin) * col2) >> 16);
                    c2 = min(ncols - 1,
                             max(0, 65536 * int(c) + int(qsin) * row2 + int(qcos) * col2) >> 16);
                    idx = 2 * idx + 1 + (pixels[r1 * dim + c1] > pixels[r2 * dim + c2]);
                }
                lutIdx = 2 * (trees * treeDepth * i + treeDepth * j + idx -
                              (treeDepth - 1));

                dr += treePreds[lutIdx + 0];
                dc += flipF*treePreds[lutIdx + 1];

                root += 4 * treeDepth - 4;
            }
            r += int(dr * s);
            c += int(dc * s);
            s *= scales;
        }

        res[0] = r;
        res[1] = c;
        res[2] = s;
    }

    PuplocDetection
    Puploc::runDetector(PuplocDetection pl, int imRows, int imCols, const uint8_t *pixels, int dim, float angle, bool flipV) const {
        int rows[pl.perturbs];
        int cols[pl.perturbs];
        int scale[pl.perturbs];
        int i = 0;
        int res[3];
        int row = 0, col = 0, sc = 0;

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (i = 0; i < pl.perturbs; ++i) {
            row = pl.row + float(pl.scale) * 0.15f * (0.5f - dist(mt));
            col = pl.col + float(pl.scale) * 0.15f * (0.5f - dist(mt));
            sc = pl.scale * (0.925f + 0.15f * dist(mt));

            if (angle > 0.0) {
                if (angle > 1.0) {
                    angle = 1.0;
                }
                classifyRotatedRegion(row, col, sc, angle, imRows, imCols, pixels, dim, flipV,
                                      &res[0]);
            } else {
                classifyRegion(row, col, sc, imRows, imCols, pixels, dim, flipV, &res[0]);
            }

            rows[i] = res[0];
            cols[i] = res[1];
            scale[i] = res[2];
        }

        std::sort(rows, rows + pl.perturbs, std::less<float>());
        std::sort(cols, cols + pl.perturbs, std::less<float>());
        std::sort(scale, scale + pl.perturbs, std::less<float>());
        int pos = int(round(pl.perturbs / 2));

        return {
                .row = rows[pos],
                .col = cols[pos],
                .scale = scale[pos],
                .perturbs = pl.perturbs
        };
    }

    Puploc::Puploc() = default;

    Puploc::~Puploc() = default;


}