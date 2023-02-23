#ifndef FRESNEL_MASK_H
#define FRESNEL_MASK_H

#include "grid.h"

/**
 * Masks the grid `g` with a filter varying sinusoidally in theta
 */
void mask_spokes_sin(Grid *g, int num_spokes);

/**
 * Masks the grid `g` with a circle of radius `radius`
 */
void mask_circle(Grid *g, double radius);

#endif // FRESNEL_MASK_H