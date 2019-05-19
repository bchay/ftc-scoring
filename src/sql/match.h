#pragma once

#include <string>

class Match {
    public:
    int tournament_id;
    int red1;
    int red2;
    int blue1;
    int blue2;
    bool red1_surrogate;
    bool red2_surrogate;
    bool blue1_surrogate;
    bool blue2_surrogate;

    Match(int tournament_id);
    Match(int tournament_id, int red1, int red2, int blue1, int blue2);
    Match(int tournament_id, int red1, int red2, int blue1, int blue2, bool red1_surrogate, bool red2_surrogate, bool blue1_surrogate, bool blue2_surrogate);
};