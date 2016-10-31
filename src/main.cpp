/*
 * TODO: - Fill out this todo list
 *       - Put my math function back and get rid of math.h
 */

#include <SDL2/SDL.h>
#include <math.h>         // Remove after implementing sine
#include "commonplace.h"
#include "commonlib.h"

#define PI32 = 3.14159265359

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
global_var r32 shipPoints[10] = {  0.0f,  15.0f,    // nose
                                  -9.0f, -15.0f,    // left wing tip
                                   9.0f, -15.0f,    // right wing tip
                                  -6.0f, -10.0f,    // left base coordinate
                                   6.0f, -10.0f };  // right base coordinate

global_var struct {
  const i32 width              = 640;
  const i32 height             = 480;
  const i32 bufferSize         = width * height;
  const i32 bufferSizeInBytes  = bufferSize * 4;
} screenData;


// I'm using calloc here rather than mmap or vm_allocate because
//   it is only called once and is more portable. (Is this true?)
global_var u32 *
screenBuffer_ptr = ( u32 * )calloc( ( screenData.width * screenData.height ),
                                      screenData.bufferSizeInBytes );

internal void
convertToScreenCoordinates( i32 x, i32 y, r32 * points ) {
  for ( i32 i = 0; i < 10; i += 2 ) {
    points[i + 1] =  -points[i + 1];     // invert Y
    points[i]     += x;
    points[i + 1] += y;
  }
}

internal void plotPixel( i32   x,
                         i32   y,
                         u32 * pixels_ptr,
                         u32   color ) {
  i32   posInMemory   = ( screenData.width * y ) + x;
  u32 * pixel         = pixels_ptr + posInMemory;

  *pixel = color;
}

internal void drawLine( i32    x1,
                        i32    y1,
                        i32    x2,
                        i32    y2,
                        u32 *  pixels_ptr,
                        u32    color ) {
  i32 dx  = abs( x2 - x1 );
  i32 sx  = x1 < x2 ? 1 : -1;           // X Direction (sign of X)
  i32 dy  = abs( y2 - y1 );
  i32 sy  = y1 < y2 ? 1 : -1;           // Y Direction (sign of Y)
  i32 err = ( dx > dy ? dx : -dy ) / 2;
  i32 prevErr;

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

// 'points' is of the form [X0, Y0, X1, Y1, ... Xn, Yn].
internal void rotatePointsLeft( r32 * points, r32 rad ) {
  r32 prevX;
  i32 yIdx;

  for ( i32 x = 0; x < 10; x += 2 ) {
    prevX = points[x];
    yIdx = x + 1;

    points[x]    = ( prevX * cos( rad ) ) - ( points[yIdx] * sin( rad ) );
    points[yIdx] = ( prevX * sin( rad ) ) + ( points[yIdx] * cos( rad ) );
  }
}

internal void drawShip( i32 x, i32 y, u32 * pixels_ptr, u32 color ) {
  // Left side
  drawLine( x + ( i32 )shipPoints[0],       // nose X pos
            y - ( i32 )shipPoints[1],       // nose Y pos
            x + ( i32 )shipPoints[2],       // left wing tip X pos
            y - ( i32 )shipPoints[3],       // left wing tip Y pos
            pixels_ptr,
            color );

  // Right side
  drawLine( x + ( i32 )shipPoints[0],       // nose X pos
            y - ( i32 )shipPoints[1],       // nose Y pos
            x + ( i32 )shipPoints[4],       // right wing tip X pos
            y - ( i32 )shipPoints[5],       // right wing tip Y pos
            pixels_ptr,
            color );

  // Base
  // Use absolute value here to translate Cartesian coordinates
  //   into right-handed Cartesian.
  drawLine( x + ( i32 )shipPoints[6],           // left base X pos
            y + ( i32 )abs( shipPoints[7] ),    // left base Y pos
            x + ( i32 )shipPoints[8],           // right base X pos
            y + ( i32 )abs( shipPoints[9]),     // right base Y pos
            pixels_ptr,
            color );
}

void gameUpdateAndRender() {

}

i32 main() {
  r32 rad = 0.7f;

  //convertToScreenCoordinates( 100, 100, shipPoints );

  // Testing ship rotation
  drawShip( 50, 50, screenBuffer_ptr, WHITE );
  drawShip( 50, 50, screenBuffer_ptr, MAGENTA );

  for ( i32 i = 0; i < 1; ++i ) {
    rotatePointsLeft( shipPoints, rad );
    drawShip( 50, 50, screenBuffer_ptr, WHITE );
    drawShip( 50, 50, screenBuffer_ptr, BLACK );
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
  u32 * pixels_ptr = (u32 *)screen->pixels;
  memcpy( pixels_ptr, screenBuffer_ptr, screenData.bufferSizeInBytes );

  SDL_UpdateWindowSurface( window );

  SDL_Delay( 5000 );

  return 0;
}
