//
// Created by adrian on 24/01/21.
//

#include "pico.hpp"

namespace pidroidlib {


    Pico::Pico() {
        this->cascadeParams = {
                .minSize = 100,
                .maxSize = 1024,
                .shiftFactor = 0.1,
                .scaleFactor = 1.1,
                .angle = 0,
                .minThreshold = 5
        };
        this->slot = 0;
    }


    Pico::Pico(CascadeParams cascadeParams) {
        this->cascadeParams = cascadeParams;
        this->slot = 0;
    }

    Pico::~Pico() = default;

    void Pico::unpackPicoCascade(std::vector<char> packet) {
        int treeNum;
        int treeDepth;
        std::vector<int> treeCodes;
        std::vector<float> treePreds;
        std::vector<float> treeThreshold;
        //((int*)cascade)[2]
        int pos = 8;
        std::vector<char> buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1],
                                  (packet)[pos + 0]};
        treeDepth = Commons::buffToInteger(buff);
        pos += 4;

        buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};

        treeNum = Commons::buffToInteger(buff);
        pos += 4;

        for (int t = 0; t < treeNum; t++) {
            std::vector<int> empty = {0, 0, 0, 0};
            treeCodes.insert(treeCodes.end(), empty.data(), empty.data() + 4);

            std::vector<signed char> v2 = std::vector<signed char>(packet.begin() + pos,
                                                                   packet.begin() + pos + int(4 *
                                                                                              pow(2,
                                                                                                  float(treeDepth)) -
                                                                                              4));
            treeCodes.insert(treeCodes.end(), v2.data(),
                             v2.data() + int(4 * pow(2, float(treeDepth)) - 4));
            pos += int(4 * pow(2, float(treeDepth)) - 4);

            for (int i = 0; i < int(pow(2, float(treeDepth))); i++) {
                buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};
                treePreds.push_back(Commons::bytesToFloatLittleEndian(buff));
                pos += 4;
            }

            buff = {(packet)[pos + 3], (packet)[pos + 2], (packet)[pos + 1], (packet)[pos + 0]};
            treeThreshold.push_back(Commons::bytesToFloatLittleEndian(buff));
            pos += 4;
        }


        this->cascade.treeCodes =  new int[treeCodes.size()];
        this->cascade.treePreds =  new float[treePreds.size()];
        this->cascade.treeThreshold =  new float[treeThreshold.size()];

        this->cascade.treeNum = treeNum;
        this->cascade.treeDepth = treeDepth;

        std::copy(treeCodes.begin(), treeCodes.end(), this->cascade.treeCodes);
        std::copy(treePreds.begin(), treePreds.end(), this->cascade.treePreds);
        std::copy(treeThreshold.begin(), treeThreshold.end(), this->cascade.treeThreshold);
    }



    float calculateIoU(PicoDetection det1, PicoDetection det2) {
        auto r1 = float(det1.row), c1 = float(det1.col), s1 = float(det1.scale);
        auto r2 = float(det2.row), c2 = float(det2.col), s2 = float(det2.scale);

        float overRow = max(0.0f, min(r1 + s1 / 2, r2 + s2 / 2) - max(r1 - s1 / 2, r2 - s2 / 2));
        float overCol = max(0.0f, min(c1 + s1 / 2, c2 + s2 / 2) - max(c1 - s1 / 2, c2 - s2 / 2));

        float iOu = overRow * overCol / (s1 * s1 + s2 * s2 - overRow * overCol);

        return iOu;
    }

    bool detcompare(PicoDetection det1, PicoDetection det2) {
        return det1.threshold < det2.threshold;
    }

    PicoResult Pico::clusterDetections(PicoResult pl, float iouThreshold) {
        int r, c, s, n;
        float q;


        bool assignments[pl.ndetections];
        PicoResult out = {};

        std::sort(pl.detections, pl.detections + pl.ndetections, detcompare);

        for (int i = 0; i < pl.ndetections; i++) {
            if (!assignments[i]) {
                r = 0, c = 0, s = 0, n = 0;
                q = 0.0f;
                for (int j = 0; j < pl.ndetections; j++) {
                    if (calculateIoU(pl.detections[i], pl.detections[j]) > iouThreshold) {
                        assignments[j] = true;
                        r += pl.detections[j].row;
                        c += pl.detections[j].col;
                        s += pl.detections[j].scale;
                        q += pl.detections[j].threshold;
                        n++;
                    }
                }
                if (n > 1) {
                    out.detections[out.ndetections] = {
                            .row = r / n,
                            .col = c / n,
                            .scale = s / n,
                            .threshold = q / float(n)
                    };
                    out.ndetections++;
                }
            }
        }

        return out;
    }

    float Pico::classifyRegion(int r, int c, int s, int nrows, int ncols, const uint8_t *pixels,
                               int dim) const {

        int root = 0, i, j, idx, treeDepth = int(pow(2, float(cascade.treeDepth)));
        float out = 0;

        r = r * 256;
        c = c * 256;

        if ((r + 128 * s) / 256 >= nrows || (r - 128 * s) / 256 < 0 ||
            (c + 128 * s) / 256 >= ncols || (c - 128 * s) / 256 < 0)
            return -1;

        // const reference improve speed
        int tdepth = cascade.treeDepth;
        int ntrees = cascade.treeNum;
        const auto treeCodes = cascade.treeCodes;
        const auto treePreds = cascade.treePreds;
        const auto treeThreshold = cascade.treeThreshold;

        int offset = 4 * treeDepth;

        for (i = 0; i < ntrees; ++i) {
            idx = 1;

            for (j = 0; j < tdepth; ++j)
                idx = 2 * idx + (pixels[((r + (treeCodes[root + 4 * idx + 0]) * s) >> 8) * dim +((c + (treeCodes[root + 4 * idx + 1]) * s) >> 8)] <=  pixels[((r + (treeCodes[root + 4 * idx + 2]) * s) >> 8) * dim + ((c + (treeCodes[root + 4 * idx + 3]) * s) >> 8)]);

            out += treePreds[treeDepth * i + idx - treeDepth];

            if (out <= treeThreshold[i]) {
                return -1;
            }
            root += offset;
        }

        return out - treeThreshold[ntrees - 1];
    }

    float
    Pico::classifyRotatedRegion(int r, int c, int s, float angle, int nrows,
                                int ncols, const uint8_t *pixels, int dim) const {

        float out = 0;
        int c1 = 0, c2 = 0, root = 0, idx = 0, r1 = 0, r2 = 0, lutIdx = 0, row1 = 0, col1 = 0, row2 = 0, col2 = 0;
        float dr = 0.0, dc = 0.0;

        int qCosTable[] = {256, 251, 236, 212, 181, 142, 97, 49, 0, -49, -97, -142, -181, -212,
                             -236, -251, -256, -251, -236, -212, -181, -142, -97, -49, 0, 49, 97,
                             142, 181, 212, 236, 251, 256};
        int qSinTable[] = {0, 49, 97, 142, 181, 212, 236, 251, 256, 251, 236, 212, 181, 142, 97,
                             49, 0, -49, -97, -142, -181, -212, -236, -251, -256, -251, -236, -212,
                             -181, -142, -97, -49, 0};

        int qsin = s * qSinTable[int(32.0 * angle)]; //s*(256.0*math.Sin(2*math.Pi*a))
        int qcos = s * qCosTable[int(32.0 * angle)]; //s*(256.0*math.Cos(2*math.Pi*a))

        int tdepth = cascade.treeDepth;
        int ntrees = cascade.treeNum;
        const auto treeCodes = cascade.treeCodes;
        const auto treePreds = cascade.treePreds;
        const auto treeThreshold = cascade.treeThreshold;

        int treeDepth = pow(2, tdepth);

        for (int i = 0; i < ntrees; i++) {
            idx = 1;
            for (int j = 0; j < tdepth; j++) {
                r1 = abs(min(nrows - 1, max(0, int(65536 * r + qcos * treeCodes[root +4 *idx +0] -qsin * treeCodes[root + 4 * idx + 1]))>> 16));
                c1 = abs(min(nrows - 1, max(0, int(65536 * c + qsin * treeCodes[root +4 *idx +0] +qcos *treeCodes[root + 4 * idx + 1]))>> 16));

                r2 = abs(min(nrows - 1, max(0, int(65536 * r + qcos * treeCodes[root +4 *idx +2] - qsin *treeCodes[root + 4 * idx + 3]))>> 16));
                c2 = abs(min(nrows - 1, max(0, int(65536 * c + qsin * treeCodes[root +4 *idx +2] +qcos * treeCodes[root + 4 * idx + 3]))>> 16));


                idx = 2 * idx + (pixels[r1 * dim + c1] > pixels[r2 * dim + c2]);
            }
            out += treePreds[treeDepth * i + idx - treeDepth];

            if (out <= treeThreshold[i]) {
                return -1;
            }
            root += 4 * treeDepth;
        }
        return out - treeThreshold[ntrees - 1];
    }

    PicoResult
    Pico::RunCascade(int rows, int cols, const uint8_t *pixels, int dim) const {

        PicoResult result = {};

        int step, row, col, offset;
        float q;

        auto scale = cascadeParams.minSize;
        const auto maxsizef = float(cascadeParams.maxSize);
        const float shiftFactor = cascadeParams.shiftFactor;
        const float angle = cascadeParams.angle > 1 ? 1 : cascadeParams.angle;
        const float scaleFactor = cascadeParams.scaleFactor;

        while (scale <= maxsizef) {
            step = max(int(shiftFactor * scale), 1);
            offset = scale / 2 + 1;

            for (row = offset; row <= rows - offset; row += step) {
                for (col = offset; col <= cols - offset; col += step) {
                    if (angle > 0) {
                        q = classifyRotatedRegion(row, col, scale, angle, rows,
                                                  cols, pixels, dim);

                    } else {
                        q = classifyRegion(row, col, scale, rows, cols, pixels, dim);
                    }

                    if (q > 0) {
                        result.detections[result.ndetections] = {.row = row,
                                .col = col,
                                .scale = scale,
                                .threshold = q};
                        result.ndetections += 1;
                    }
                }
            }
            scale *= scaleFactor;
        }

        return result;
    }

    const CascadeParams &Pico::getCascadeParams() const {
        return cascadeParams;
    }

    void Pico::setCascadeParams(const CascadeParams &newCascadeParams) {
        Pico::cascadeParams = newCascadeParams;
    }

    PicoResult Pico::updateMemory(PicoResult pl) {
        int i, j;
        PicoResult out = {};

        //
        counts[this->slot] = pl.ndetections;

        for (i = 0; i < counts[slot]; ++i) {
            memory[slot * 4 * maxslotsize + 4 * i + 0] = float(pl.detections[i].row);
            memory[slot * 4 * maxslotsize + 4 * i + 1] = float(pl.detections[i].col);
            memory[slot * 4 * maxslotsize + 4 * i + 2] = float(pl.detections[i].scale);
            memory[slot * 4 * maxslotsize + 4 * i + 3] = pl.detections[i].threshold;
        }

        slot = (slot + 1) % nmemslots;


        for (i = 0; i < nmemslots; ++i)
            for (j = 0; j < counts[i]; ++j) {
                if (out.ndetections >= maxslotsize)
                    return out;

                out.detections[out.ndetections] = {
                        .row = int(memory[i * 4 * maxslotsize + 4 * j + 0]),
                        .col = int(memory[i * 4 * maxslotsize + 4 * j + 1]),
                        .scale = int(memory[i * 4 * maxslotsize + 4 * j + 2]),
                        .threshold = memory[i * 4 * maxslotsize + 4 * j + 3]
                };
                ++out.ndetections;
            }

        return out;
    }

    PicoResult Pico::getProminentFace(PicoResult result) {
        int index = 0, i = 0;
        int maxRadius = 0;
        PicoResult out = {};

        for(i = 0; i < result.ndetections; i++) {
            if(result.detections[i].scale >  maxRadius) {
                index = i;
                maxRadius = result.detections[i].scale;
            }
        }

        out.detections[0] = result.detections[index];
        out.ndetections = 1;

        return out;
    }

}

