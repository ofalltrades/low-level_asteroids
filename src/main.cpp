#include <SDL2/SDL.h>
#include <math.h>

#define internal      static
#define local_persist static
#define global_var    static
#define readonly      const

typedef   char                int8;
typedef   short               int16;
typedef   int                 int32;
typedef   double              int64;

typedef   unsigned char       uint8;
typedef   unsigned short      uint16;
typedef   unsigned int        uint32;
typedef   unsigned long long  uint64;

typedef   float               real32;
typedef   double              real64;

#define BLACK     0b00000000'00000000'00000000'00000000
#define WHITE     0b00000000'11111111'11111111'11111111
#define MAGENTA   0b00000000'11111111'00000000'11111111

// The initial position of the ship is facing up (PI/2rad).
// Ship details: width       = 18
//               height      = 30
//               body height = 25
//               body width  = 12
//
// Note: The above measurements should only be relied on with respect
//       to this initial state
global_var real32 shipScalars[10] = {  0.0f,  15.0f,    // nose
                                      -9.0f, -15.0f,    // left wing tip
                                       9.0f, -15.0f,    // right wing tip
                                      -6.0f, -10.0f,    // left base scalar
                                       6.0f, -10.0f };  // right base scalar

global_var int32 shipScalarsMemPosCache[10] = {};

global_var struct {
  const int32 width              = 640;
  const int32 height             = 480;
  const int32 bufferSize         = width * height;
  const int32 bufferSizeInBytes  = bufferSize * 4;
} screenData;

uint32 *
screenBuffer_ptr = ( uint32 * )calloc( ( screenData.width * screenData.height ),
                                       screenData.bufferSizeInBytes );

/*internal int32 abs( int32 val ) {*/
  //printf("asdf");
  //return val < 0.0f ? -val : val;
/*}*/

/*internal int32 factorial( int32 n ) {*/
  //if ( n == 0 ) return 0;

  //for ( int32 x = n - 1; x > 0; x-- ) {
    //n *= x;
  //}

  //return n;
/*}*/

/*internal int32 pow( real32 n, int32 exp ) {*/
  //real32 result = n;

  //for ( int32 x = 0; x < exp - 1; ++x  ) {
    //result *= n;
  //}

  //return result;
/*}*/

internal void
convertToScreenCoordinates( int32 x, int32 y, real32 * scalars ) {
  for ( int32 i = 0; i < 10; i += 2 ) {
    scalars[i + 1] =  -scalars[i + 1]; // Invert Y
    scalars[i]     += x;
    scalars[i + 1] -=  y;
  }
  printf("%f  ", scalars[0]);
  printf("%f\n", scalars[1]);
  printf("%f  ", scalars[2]);
  printf("%f\n", scalars[3]);
  printf("%f  ", scalars[4]);
  printf("%f\n", scalars[5]);
  printf("%f  ", scalars[6]);
  printf("%f\n", scalars[7]);
  printf("%f  ", scalars[8]);
  printf("%f\n", scalars[9]);
}

internal void plotPixel( int32    x,
                         int32    y,
                         uint32 * pixels_ptr,
                         uint32   color ) {
  int32    posInMemory   = ( screenData.width * y ) + x;
  uint32 * pixel         = pixels_ptr + posInMemory;

  *pixel = color;
}

internal void drawLine( int32    x1,
                        int32    y1,
                        int32    x2,
                        int32    y2,
                        uint32 * pixels_ptr,
                        uint32   color ) {
  int32 dx  = abs( x2 - x1 ),
        sx  = x1 < x2 ? 1 : -1,           // X Direction (sign of X)
        dy  = abs( y2 - y1 ),
        sy  = y1 < y2 ? 1 : -1,           // Y Direction (sign of Y)
        err = ( dx > dy ? dx : -dy ) / 2,
        prevErr;

  // Plot first pixel manually.
  plotPixel( x1, y1, pixels_ptr, color );
  while ( !( x1 == x2 && y1 == y2 ) ) {
    prevErr = err;

    if ( prevErr > -dx ) {
      err -= dy;
      x1  += sx;
    }

    if ( prevErr < dy ) {
      err += dx;
      y1  += sy;
    }

    plotPixel( x1, y1, pixels_ptr, color );
  }
}

internal void eraseObj( int32 *  scalars,
                        uint32 * pixels_ptr,
                        uint32   color ) {
  for ( int32 i = 0; i < 10; i += 2 ) {
    drawLine( scalars[i],
              scalars[i + 1],
              scalars[i + 2],
              scalars[i + 3],
              pixels_ptr,
              MAGENTA );
  }
}

// 'scalars' is of the form [X0, Y0, X1, Y1, ... Xn, Yn].
internal void rotateScalarsLeft( real32 * scalars, real32 rad ) {
  real32 prevX;
  int32  yIdx;

  for ( int32 x = 0; x < 10; x += 2 ) {
    prevX = scalars[x];
    yIdx = x + 1;

    scalars[x]    = ( prevX * cos( rad ) ) - ( scalars[yIdx] * sin( rad ) );
    scalars[yIdx] = ( prevX * sin( rad ) ) + ( scalars[yIdx] * cos( rad ) );
  }
}

/*internal real32 sin( real32 n ) {*/
  //real32 num  = pow( n, 3 ) / ( real32 )factorial( 3 );
  //real32  sign = 1.0f;

  //for ( int32 x = 5; x <= 55; x += 2 ) {
    //num += sign * (pow( n, x ) / ( real32 )factorial( x ));
    //sign = -sign;
  //}

  //return n - num;
//}

//internal real32 cos( real32 rad ) {
  //return 0;
//}

//internal real32 tan( real32 rad ) {
  //return 0;
/*}*/

/*internal real32 calcRotation( int32 direction ) {*/

/*}*/

/*internal void rotateObj( obj ) {*/

/*}*/

internal void drawShip( int32 x, int32 y, uint32 * pixels_ptr ) {
  shipScalarsMemPosCache[0] = x + ( int32 )shipScalars[0];
  shipScalarsMemPosCache[1] = y - ( int32 )shipScalars[1];
  shipScalarsMemPosCache[2] = x + ( int32 )shipScalars[2];
  shipScalarsMemPosCache[3] = y - ( int32 )shipScalars[3];
  shipScalarsMemPosCache[4] = x + ( int32 )shipScalars[4];
  shipScalarsMemPosCache[5] = y - ( int32 )shipScalars[5];
  shipScalarsMemPosCache[6] = x + ( int32 )shipScalars[6];
  shipScalarsMemPosCache[7] = y + ( int32 )abs( shipScalars[7] );
  shipScalarsMemPosCache[8] = x + ( int32 )shipScalars[8];
  shipScalarsMemPosCache[9] = y + ( int32 )abs( shipScalars[9] );

  // Left side
  drawLine( shipScalarsMemPosCache[0],       // nose X pos
            shipScalarsMemPosCache[1],       // nose Y pos
            shipScalarsMemPosCache[2],       // left wing tip X pos
            shipScalarsMemPosCache[3],       // left wing tip Y pos
            pixels_ptr,
            WHITE );

  // Right side
  drawLine( shipScalarsMemPosCache[0],       // nose X pos
            shipScalarsMemPosCache[1],       // nose Y pos
            shipScalarsMemPosCache[4],       // right wing tip X pos
            shipScalarsMemPosCache[5],       // right wing tip Y pos
            pixels_ptr,
            WHITE );

  // Base
  // Use absolute value here to translate Cartesian coordinates
  //   into right-handed Cartesian.
  drawLine( shipScalarsMemPosCache[6],           // left base X pos
            shipScalarsMemPosCache[7],    // left base Y pos
            shipScalarsMemPosCache[8],           // right base X pos
            shipScalarsMemPosCache[9],    // right base Y pos
            pixels_ptr,
            WHITE );
}

int32 main() {
  real32 rad = 0.7f;

  //convertToScreenCoordinates( 100, 100, shipScalars );

  // Testing ship rotation
  drawShip( 50, 50, screenBuffer_ptr );
  eraseObj( shipScalarsMemPosCache, screenBuffer_ptr, MAGENTA );

  for ( int32 i = 0; i < 1; ++i ) {
    rotateScalarsLeft( shipScalars, rad );
    drawShip( 50, 50, screenBuffer_ptr );
  }

  SDL_Window *  window = 0;
  SDL_Surface * screen = 0;

  SDL_Init( SDL_INIT_VIDEO );

  window = SDL_CreateWindow( "SDL2 Asteroids",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             screenData.width,
                             screenData.height,
                             SDL_WINDOW_BORDERLESS );
  if ( !window ) {
    printf( "Could not create window: %s\n", SDL_GetError() );
    exit( EXIT_FAILURE );
  }

  screen = SDL_GetWindowSurface( window );

  // Fill screen with black pixels.
  uint32 * pixels_ptr = (uint32 *)screen->pixels;
  memcpy( pixels_ptr, screenBuffer_ptr, screenData.bufferSizeInBytes );

  SDL_UpdateWindowSurface( window );

  SDL_Delay( 5000 );

  return 0;
}
