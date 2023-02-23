#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#include "grid.h"

// A format string for printing a complex number, compatible with numpy
#define CMPLX_FORMAT_STR "(%e%+ej)"


void apply_func_eltwise(double _Complex (*func) (double _Complex, double _Complex), Grid *g1, Grid *g2);
double _Complex func_mult(double _Complex, double _Complex);
double _Complex func_add(double _Complex, double _Complex);


// ------------- PUBLIC FUNCTIONS ----------------------------------------------

Grid *grid_create(int res, double size, double _Complex init)
{
	Grid *g = malloc(sizeof(Grid));
	do
	{
		// Create grid struct
		if (g == NULL) break;

		// Create fields, allocate memory for first array dimension
		g->res = res;
		g->size = size;
		size_t arr_size = sizeof(**g->values) * g->res;
		g->values = malloc(arr_size);
		g->ruler = malloc(arr_size);
		if (g->values == NULL || g->ruler == NULL) break;

		// Allocate memory for second dimension of array
		for (int i = 0; i < g->res; i++)
		{
			g->values[i] = malloc(arr_size);
			if (g->values[i] == NULL) break;

			for (int j = 0; j < g->res; j++)
			{
				g->values[i][j] = init;
			}

			g->ruler[i] = (double)i * g->size / (double)g->res - 0.5 * g->size;
		}
		return g;
	} while (0);

	fprintf(stderr, "Failed to allocate memory for grid\n");
	grid_free(g);
	return NULL;
}

void grid_free(Grid *g)
{
	if (g == NULL) return;
	if (g->values != NULL)
	{
		for (int i = 0; i < g->res; i++)
		{
			if (g->values[i] != NULL)
				free(g->values[i]);
		}
		free(g->values);
	}
	free(g);
}

void grid_print(Grid *g, FILE *f)
{
	for (int i = 0; i < g->res; i++)
	{
		for (int j = 0; j < g->res - 1; j++)
		{
			double _Complex val = grid_get(g, i, j);
			fprintf(f, CMPLX_FORMAT_STR ", ", creal(val), cimag(val));
		}
		double _Complex val = grid_get(g, i, g->res - 1);
		fprintf(f, CMPLX_FORMAT_STR "\n", creal(val), cimag(val));
	}
}

double _Complex grid_get(Grid *g, int i, int j)
{
	if (i >= g->res || i < 0 || j >= g->res || j < 0)
	{
		fprintf(
			stderr,
			"Invalid indices (%d, %d) for grid of resolution %d\n",
			i, j, g->res
		);
		exit(1);
	}
	return g->values[i][j];
}

void grid_set(Grid *g, int i, int j, double _Complex value)
{
	if (i >= g->res || i < 0 || j >= g->res || j < 0)
	{
		fprintf(
			stderr,
			"Invalid indices (%d, %d) for grid of resolution %d\n",
			i, j, g->res
		);
		exit(1);
	}
	g->values[i][j] = value;
}

double grid_coord(Grid *g, int i)
{
	if (i >= g->res || i < 0)
	{
		fprintf(
			stderr,
			"Invalid index (%d) for grid of resolution %d\n",
			i, g->res
		);
		exit(1);
	}
	return g->ruler[i];
}

void grid_mult_eltwise(Grid *g1, Grid *g2)
{
	apply_func_eltwise(func_mult, g1, g2);
}

void grid_add(Grid *g1, Grid *g2)
{
	apply_func_eltwise(func_add, g1, g2);
}


// ------------- PRIVATE FUNCTIONS ---------------------------------------------

void apply_func_eltwise(
	double _Complex (*func) (double _Complex, double _Complex),
	Grid *g1, Grid *g2)
{
	if (g1->res != g2->res) {
		fprintf(
			stderr,
			"Attempt to multiply grids of different resolutions (%d and %d)\n",
			g1->res, g2->res
		);
		exit(1);
	}
	for (int i = 0; i < g1->res; i++)
	{
		for (int j = 0; j < g1->res; j++)
		{
			double _Complex v1 = grid_get(g1, i, j);
			double _Complex v2 = grid_get(g2, i, j);
			grid_set(g1, i, j, func(v1, v2));
		}
	}
}

double _Complex func_mult(double _Complex x, double _Complex y)
{
	return x * y;
}

double _Complex func_add(double _Complex x, double _Complex y)
{
	return x + y;
}
