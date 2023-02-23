#include <math.h>
#include <complex.h>

#include "mask.h"
#include "grid.h"

void mask_spokes_sin(Grid *g, int num_spokes)
{
	for (int i = 0; i < g->res; i++)
	{
		for (int j = 0; j < g->res; j++)
		{
			double x = grid_coord(g, i);
			double y = grid_coord(g, j);
			double theta = atan2(y, x);
			double factor = 0.5 * (1 + cos(theta * (double)num_spokes));
			grid_set(g, i, j, grid_get(g, i, j) * factor);
		}
	}
}

void mask_circle(Grid *g, double radius)
{
	for (int i = 0; i < g->res; i++)
	{
		for (int j = 0; j < g->res; j++)
		{
			double x = grid_coord(g, i);
			double y = grid_coord(g, j);
			double r = hypot(x, y);
			if (r > radius)
				grid_set(g, i, j, 0.0 + 0.0 * I);
		}
	}
}
