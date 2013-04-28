#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "sys/time.h"
#include "unistd.h"

//libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace std;

struct color {
	float r, g, b, a;
};

//game classes
#include "utils.h"
#include "Game.h"

//defines
#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 768
#define DEFAULT_WINDOW_NAME "ld26"
#define DEFAULT_WINDOW_X_POS 10
#define DEFAULT_WINDOW_Y_POS 10
#define IS_DEBUG_ENABLED false
#define PI 3.1415926535
#define DEGTORAD (PI / 180)
#define RADTODEG (180 / PI)
#define MAXFPS 50

#define TRACKTHICKNESS 10
#define SPARE 30
#define MAXMISSES -1000