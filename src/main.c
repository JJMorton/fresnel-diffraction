#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#include "grid.h"
#include "mask.h"


const int aper_grid_res = 100;
const double aper_grid_size = 30.0;

const int screen_grid_res = 100;
const double screen_grid_size = 40.0;
const double screen_z = 10.0;

const double mask_circle_radius = 10.0;
const int mask_spokes_n = 10;

const double source_lambda = 1.0;


double _Complex fresnel_integrand(double z_screen, double r, double _Complex E, double lambda);


int main(int argc, const char *argv[])
{
	Grid *aper_grid = grid_create(aper_grid_res, aper_grid_size, 1.0 + 0.0*I);
	if (aper_grid == NULL) return 1;

	Grid *screen_grid = grid_create(screen_grid_res, screen_grid_size, 0.0 + 0.0*I);
	if (screen_grid == NULL)
	{
		grid_free(aper_grid);
		return 1;
	}

	// Apply the mask to the light coming through the aperture
	// mask_circle(aper_grid, mask_circle_radius);
	mask_spokes_sin(aper_grid, mask_spokes_n);

	FILE *aper_fout = fopen("out/aperture.csv", "w");
	if (aper_fout != NULL)
	{
		grid_print(aper_grid, aper_fout);
		fclose(aper_fout);
	}
	else
	{
		fprintf(stderr, "Failed to open aperture file\n");
	}

	// Integrate!
	for (int screen_ix = 0; screen_ix < screen_grid->res; screen_ix++)
	{
		printf("Row %d / %d", screen_ix + 1, screen_grid->res);
		for (int screen_iy = 0; screen_iy < screen_grid->res; screen_iy++)
		{
			for (int aper_ix = 0; aper_ix < aper_grid->res; aper_ix++)
			{
				for (int aper_iy = 0; aper_iy < aper_grid->res; aper_iy++)
				{
					double aper_x = grid_coord(aper_grid, aper_ix);
					double aper_y = grid_coord(aper_grid, aper_iy);
					double screen_x = grid_coord(screen_grid, screen_ix);
					double screen_y = grid_coord(screen_grid, screen_iy);
					double delta_x = screen_x - aper_x;
					double delta_y = screen_y - aper_y;
					double r = sqrt(delta_x*delta_x + delta_y*delta_y + screen_z*screen_z);
					double _Complex aper_E = grid_get(aper_grid, aper_ix, aper_iy);
					double _Complex integrand = fresnel_integrand(screen_z, r, aper_E, source_lambda);

					double _Complex screen_E = grid_get(screen_grid, screen_ix, screen_iy);
					grid_set(screen_grid, screen_ix, screen_iy, screen_E + integrand);
				}
			}
			printf(".");
		}
		printf("\n");
	}

	FILE *screen_fout = fopen("out/screen.csv", "w");
	if (screen_fout != NULL)
	{
		grid_print(screen_grid, screen_fout);
		fclose(screen_fout);
	}
	else
	{
		fprintf(stderr, "Failed to open CSV file\n");
	}

	return 0;
}

double _Complex fresnel_integrand(double z_screen, double r, double _Complex E, double lambda)
{
	double k = M_PI_2 / lambda;
	return 1.0 / (lambda * I) * E * cexp(k * r * I) * z_screen / (r * r);
}
