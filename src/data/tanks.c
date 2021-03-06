/* The structure in which all the properties of the various tanks are stored */

#include <tank.h>

tankinfotype tank[num_tanks]={

{TANK,
 {1E3, 1E5, 2E3, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,-2}, MAIN_TURRET, 0},
 {1, 10, 30, 30000},
 {1, TRUE, "STANDARD"},
 {HT_MAIN, TRUE, 0,
  {30, 1500, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 0,-12},
 {2, -4,-8, 4,-8, 2,-10},
 {-6,10, 6,10},
 {0,-10},
 {1, LI_LARGE,-2,1}},

{TANK,
 {1E3, 1E5, 2E3, 9E2, 3E5, 0, 0.4, 0, 7, 10},
 {{0,0}, MAIN_STATIC, 10},
 {1, 10, 30, 30000},
 {1, TRUE, "DEVASTATOR"},
 {HT_MAIN, TRUE, 0,
  {30, 1500, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {2, -8,-6, 9,-6},
 {2, -2,-9, 3,-9, 0,-13},
 {-2,10, 3,10},
 {0,0},
 {1, LI_LARGE,-4,-3}},

{TANK,
 {5E2, 1E5, 2E3, 6E2, 3E5, 0, 0.6, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 6, 20, 20000},
 {1, TRUE, "MOUSE"},
 {HT_MAIN, TRUE, 0,
  {20, 1000, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {80, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, -5,-8},
 {2, -2,-6, 2,-6, -4,-2},
 {-2,10, 3,10},
 {6,-2},
 {1, LI_LARGE,4,-2}},

{TANK,
 {2E3, 2E5, 2E4, 2.5E3, 1E6, 0, 0.2, 0, 7, 1},
 {{0,0}, MAIN_TURRET, 0},
 {1, 13, 40, 30000},
 {4, TRUE, "CROCODILE"},
 {HT_MAIN, TRUE, 0,
  {60, 3000, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {30, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {2, -4,-11, 4,-11},
 {2, -7,-14, 7,-14, 2,-10},
 {-6,10, 6,10},
 {0,-10},
 {1, LI_LARGE,-2,-1}},

{PLANE,
 {5E2, 6E4, 3E3, 4E2, 1E5, 0, 0.9, 0.15, 20, 0},
 {{0,0}, MAIN_STATIC, 0},
 {12, 8, 40, 30000},
 {0, TRUE, "THUNDERBOLT"},
 {HT_SROCK, FALSE, 0,
  {100, 3000, 0, 0, 0, 7, 0, 0, 0, 1, 1, 1},
  {40, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {200},
 {2, -6,-5, 6,-5},
 {2, 1,-15, 2,-15},
 {-5,10, 5,10},
 {6,-5},
 {0}},

{CHOPPER,
 {5E2, 1E5, 2E3, 6E2, 1E5, 0, 0.9, 0, 15, 0},
 {{0,-2}, MAIN_ROTATE, 0},
 {7, 7, 30, 30000},
 {0, TRUE, "CHOPPER"},
 {HT_SROCK, FALSE, 0,
  {100, 1500, 0, 0, 3, 0, 0, 0, 0, 1, 1, 1},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {200},
 {2, -4,-7, 4,-7},
 {1, 0,-13},
 {0,16, 0,16},
 {4,-9},
 {1, LI_LARGE,1,-9}},

{TANK,
 {5E2, 1E5, 2E3, 4.5E2, 2E5, 0, 0.6, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 7, 15, 30000},
 {1, TRUE, "RACE TANK"},
 {HT_MAIN, FALSE, 1,
  {0, 1000, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0},
  {100, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {2, -4,-1, 4,-1},
 {2, -4,-1, 4,-1, 0,-5},
 {-2,11, -2,11},
 {7,-3},
 {1, LI_MEDIUM,1,-1}},

{TANK,
 {1E3, 1E5, 2E3, 1E3, 5E5, 0, 0.2, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 11, 50, 30000},
 {7, TRUE, "LOSERS TANK"},
 {HT_MAIN, TRUE, 0,
  {50, 3000, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {30, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {4, -8,-2, -4,-9, 4,-9, 8,-3},
 {2, -2,-9, 2,-9, -4,-9},
 {1,12, -1,12},
 {-8,-2},
 {1, LI_LARGE,-6,1}},

{TANK,
 {2E3, 1E5, 1E4, 3E3, 1E6, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_TURRET, 0},
 {1, 13, 30, 30000},
 {4, TRUE, "LIZZARD"},
 {HT_MAIN, TRUE, 0,
  {60, 3000, 0, 7, 3, 3, 2, 5, 5, 0, 0, 0},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, -1,-14},
 {2, -3,-11, 3,-11, 0,-11},
 {-2,8, 2,8},
 {-1,-14},
 {1, LI_MEDIUM,-4,-1}},

{TANK,
 {5E2, 63E3, 13E2, 6E2, 3E5, 0, 0.6, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 4, 10, 30000},
 {1, FALSE, "COMPUTER DRONE"},
 {HT_MAIN, FALSE, 1,
  {0, 1000, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 0,-6},
 {1, 1,-3, -1,-3},
 {-2,6, -2,6},
 {0,0},
 {0}},

{TANK,
 {1E3, 4E4, 8E2, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,3}, MAIN_TURRET, 0},
 {1, 7, 20, 30000},
 {1, TRUE, "LAUNCHER"},
 {HT_SROCK, TRUE, 0,
  {100, 1500, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {3, -4,-6, 0,-6, 4,-6},
 {1, 3,-9, 3,-9},
 {-3,7, 3,7},
 {0,-5},
 {1, LI_MEDIUM,-2,-1}},

{TANK,
 {2E3, 1E5, 1E4, 23E2, 1E6, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_TURRET, 0},
 {1, 10, 30, 30000},
 {1, TRUE, "SIDE-ARM"},
 {HT_MAIN, TRUE, 0,
  {30, 1500, 0, 7, 3, 0, 2, 5, 5, 0, 0, 0},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {2, -5,-9, 6,-9},
 {2, -4,-10, -2,-10, 0,-10},
 {-4,10, -4,10},
 {4,-9},
 {1, LI_MEDIUM,2,-3}},

{PLANE,
 {5E2, 5E4, 2E3, 6E2, 1E5, 0, 0.9, 0.2, 20, 0},
 {{0,0}, MAIN_STATIC, 0},
 {12, 8, 20, 30000},
 {0, TRUE, "GREEN PLANE"},
 {HT_SROCK, FALSE, 1,
  {100, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {200},
 {4, -11,-7, -8,-7, 8,-7, 11,-7},
 {2, -11,-7, 10,-7},
 {-3,-8, 3,-8},
 {0,0},
 {0}},

{PLANE,
 {5E2, 6E4, 3E3, 4E2, 1E5, 0, 0.9, 0.2, 20, 0},
 {{0,0}, MAIN_STATIC, 0},
 {12, 8, 20, 30000},
 {0, TRUE, "JET"},
 {HT_SROCK, FALSE, 0,
  {100, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {40, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {200},
 {4, -12,-5, -2,-10, 2,-10, 12,-5},
 {2, -5,-5, 5,-5},
 {-3,6, 3,6},
 {6,-4},
 {0}},

{CAR,
 {1E3, 3E4, 7E2, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 10, 10, 30000},
 {0, FALSE, "CAR"},
 {HT_MAIN, FALSE, 1,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {200, 20, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, -5,-3},
 {1, -5,-3, -5,-3},
 {3,10, 3,10},
 {-5,-3},
 {2, LI_SMALL,4,-9, LI_SMALL,-3,-9}},

{CAR,
 {1E3, 3E4, 7E2, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 10, 10, 30000},
 {0, FALSE, "BOB'S BUS"},
 {HT_MAIN, FALSE, 1,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {200, 20, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, -6,-5},
 {1, -6,-5, -6,-5},
 {3,15, 3,15},
 {-6,-5},
 {2, LI_SMALL,4,-9, LI_SMALL,-4,-9}},

{CHOPPER,
 {1E3, 3E4, 1E3, 250, 1E5, 0, 0.6, 0, 12, 0},
 {{0,-3}, MAIN_ROTATE, 0},
 {7, 7, 30, 30000},
 {0, TRUE, "BATTLE CHOPPER"},
 {HT_SROCK, FALSE, 0,
  {100, 1500, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {4, -8,-12, 8,-12, -6,-11, 6,-11},
 {1, -2,-16},
 {0,16, 0,16},
 {6,-6},
 {0}},

{TANK,
 {1E3, 3E4, 7E2, 8E2, 5E5, 0, 0.3, 0, 7, 1},
 {{0,-6}, MAIN_TURRET, 0},
 {1, 9, 30, 30000},
 {1, TRUE, "BUG LAUNCHER"},
 {HT_SROCK, TRUE, 4,
  {100, 1500, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {2, -4,-6, 4,-6},
 {1, 5,-8, 7,-8},
 {-3,7, -3,7},
 {4,-6},
 {1, LI_MEDIUM,0,1}},

{CAR,
 {2E3, 3E4, 1E3, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 12, 10, 30000},
 {3, TRUE, "TRUCK"},
 {HT_MAIN, FALSE, 1,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {200, 20, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, -6,-9},
 {1, -6,-9, -6,-9},
 {-5,14, -5,14},
 {8,2},
 {2, LI_SMALL,-6,-12, LI_SMALL,6,-12}},

{TANK,
 {1E3, 3E4, 7E2, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,3}, MAIN_STATIC, 0},
 {1, 7, 20, 30000},
 {1, TRUE, "FLAMER"},
 {HT_MAIN, TRUE, 2,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {50, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 0,-14},
 {1, -3,-8, 3,-8},
 {-4,9, 4,9},
 {0,-14},
 {0}},

{TANK,
 {1E3, 1E5, 2E3, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,0}, MAIN_STATIC, 0},
 {1, 10, 40, 30000},
 {9, TRUE, "BULLDOZER"},
 {HT_MAIN, TRUE, 0,
  {30, 2000, 0, 7, 3, 3, 2, 5, 5, 1, 1, 1},
  {110, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 2,-4},
 {1, -1,2, 1,2},
 {3,13, 3,13},
 {5,0},
 {1, LI_SMALL,3,2}},

{TANK,
 {1E3, 2E4, 7E2, 1E3, 5E5, 0, 0.3, 0, 12, 1},
 {{0,3}, MAIN_STATIC, 0},
 {1, 13, 30, 30000},
 {1, TRUE, "ATOMIC TANK"},
 {HT_MAIN, TRUE, 2,
  {0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0},
  {50, 10, 15, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {5, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 0,-14},
 {1, -3,-8, 3,-8},
 {-4,9, 4,9},
 {0,-14},
 {0}},

{TANK,
 {1E3, 1E5, 2E3, 1E3, 5E5, 0, 0.3, 0, 7, 1},
 {{0,-2}, MAIN_TURRET, 0},
 {1, 10, 30, 30000},
 {1, FALSE, "SHOPPING CAR"},
 {HT_MAIN, FALSE, 0,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {70, 10, 6, 30, 200, 100, 50, 50, 50, 100, 100, 50},
  {10, 1, 1, 7, 10, 0, 0, 1, 1, 0, 0, 0}},
 {0},
 {1, 0,-12},
 {2, -4,-8, 4,-8, 2,-10},
 {-6,10, 6,10},
 {0,-10},
 {1, LI_LARGE,-2,1}}};
