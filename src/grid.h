#include <stdio.h>
#include <complex.h>

#ifndef FRESNEL_GRID_H
#define FRESNEL_GRID_H

typedef struct
{
	int res;
	double size;
	double _Complex **values;
	double *ruler;
} Grid;

/**
 * Creates a grid of resolution `res` and initialises all the values to `init`
 */
Grid *grid_create(int res, double size, double _Complex init);

/**
 * Frees the grid from memory
 */
void grid_free(Grid *g);

/**
 * Prints the contents of the grid as CSV to the file `f`
 */
void grid_print(Grid *g, FILE *f);

/**
 * Gets the value with index (`i`, `j`) in the grid
 */
double _Complex grid_get(Grid *g, int i, int j);

/**
 * Sets the value with index (`i`, `j`) in the grid
 */
void grid_set(Grid *g, int i, int j, double _Complex value);

/**
 * Return the physical position of the index `i` on the grid
 */
double grid_coord(Grid *g, int i);

/**
 * Multiplies `g1` by `g2`, element-wise
 */
void grid_mult_eltwise(Grid *g1, Grid *g2);

/**
 * Adds `g2` to `g1`, preserving shape
 */
void grid_add(Grid *g1, Grid *g2);

#endif // FRESNEL_GRID_H
