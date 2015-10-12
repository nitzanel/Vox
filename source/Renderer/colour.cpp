#include "colour.h"

#include <cmath>

#ifdef _MSC_VER
#define _INFINITY (FLT_MAX+FLT_MAX)
#define _NAN (INFINITY-INFINITY)
#endif

void Colour::RGBToHSV(float r, float g, float b, float *h, float *s, float *v)
{
    float min, max, delta;

    min = r < g ? r : g;
    min = min  < b ? min : b;

    max = r > g ? r : g;
    max = max  > b ? max : b;

    *v = max;                                // v
    delta = max - min;
    if( max > 0.0f ) {
        *s = (delta / max);                  // s
    } else {
        // r = g = b = 0                        // s = 0, v is undefined
        *s = 0.0f;
        *h = 0.0f;//NAN;                            // its now undefined
        return;
    }
    if( r >= max )                           // > is bogus, just keeps compilor happy
        if(g - b == 0.0f) {
            *h = 0.0f;
        }
        else {
            *h = ( g - b ) / delta;        // between yellow & magenta
        }
    else
        if( g >= max )
            *h = 2.0f + ( b - r ) / delta;  // between cyan & yellow
        else
            *h = 4.0f + ( r - g ) / delta;  // between magenta & cyan

    *h *= 60.0f;                              // degrees

    if( *h < 0.0f )
        *h += 360.0f;
}

void Colour::HSVToRGB(float h, float s, float v, float *r, float *g, float *b)
{
    float      hh, p, q, t, ff;
    long        i;

    if(s <= 0.0f) {       // < is bogus, just shuts up warnings
        if(h == NAN) {   // h == NAN
            *r = v;
            *g = v;
            *b = v;
            return;
        }
        // error - should never happen
        *r = 0.0f;
        *g = 0.0f;
        *b = 0.0f;
        return;
    }
    hh = h;
    if(hh >= 360.0f) hh = 0.0f;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0f - s);
    q = v * (1.0f - (s * ff));
    t = v * (1.0f - (s * (1.0f - ff)));

    switch(i) {
    case 0:
        *r = v;
        *g = t;
        *b = p;
        break;
    case 1:
        *r = q;
        *g = v;
        *b = p;
        break;
    case 2:
        *r = p;
        *g = v;
        *b = t;
        break;
    case 3:
        *r = p;
        *g = q;
        *b = v;
        break;
    case 4:
        *r = t;
        *g = p;
        *b = v;
        break;
    case 5:
    default:
        *r = v;
        *g = p;
        *b = q;
        break;
    }
}
