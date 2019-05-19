#include "match.h"

Match::Match(int tournament_id) {
    this->tournament_id = tournament_id;
    this->red1 = 0;
    this->red2 = 0;
    this->blue1 = 0;
    this->blue2 = 0;
    this->red1_surrogate = false;
    this->red2_surrogate = false;
    this->blue1_surrogate = false;
    this->blue2_surrogate = false;
}

Match::Match(int tournament_id, int red1, int red2, int blue1, int blue2) {
    this->tournament_id = tournament_id;
    this->red1 = red1;
    this->red2 = red2;
    this->blue1 = blue1;
    this->blue2 = blue2;
    this->red1_surrogate = false;
    this->red2_surrogate = false;
    this->blue1_surrogate = false;
    this->blue2_surrogate = false;
}

Match::Match(int tournament_id, int red1, int red2, int blue1, int blue2, bool red1_surrogate, bool red2_surrogate, bool blue1_surrogate, bool blue2_surrogate) {
    this->tournament_id = tournament_id;
    this->red1 = red1;
    this->red2 = red2;
    this->blue1 = blue1;
    this->blue2 = blue2;
    this->red1_surrogate = red1_surrogate;
    this->red2_surrogate = red2_surrogate;
    this->blue1_surrogate = blue1_surrogate;
    this->blue2_surrogate = blue2_surrogate;
}