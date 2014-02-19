
#include <assert.h>
#include "histogram.h"

Histogram* histogramCreate(int bin)
{
	Histogram* histo = NULL;
	
	assert(bin > 0);

	histo = (Histogram*) malloc(sizeof(Histogram));
	histogramInit(histo, bin);	
	

	return histo;
}

void histogramInit(Histogram * const histo, int bin)
{
	int i, j;

	assert(histo != NULL);
	assert(bin > 0);

	histo->values = (float***) calloc(bin, sizeof(float**));
	for (i = 0; i < bin; i++)
	{
		histo->values[i] = (float**) calloc(bin, sizeof(float*));
		for (j = 0; j < bin; j++)
		{
			histo->values[i][j] = (float*) calloc(bin, sizeof(float));
		}
	}
	histo->bin = bin;
	histo->nbPixel = 0;
	histo->normalized = 0;
}

void histogramRelease(Histogram* histo)
{
	int i,j;
	
	assert(histo != NULL);

	for (i = 0; i < (*histo)->bin; i++)
	{
		for (j = 0; j < (*histo)->bin; j++)
		{
			free((*histo)->values[i][j]);
		}
		free((*histo)->values[i]);
	}
}

void histogramDestroy(Histogram** histo)
{
	assert(histo != NULL);

	histogramRelease(*histo);
	free(*histo);
	histo = NULL;
}

void histogramCompute(CIMAGE const * const img, Histogram * const histo)
{
	int i, j;
	int binDiv = 0;

	assert(img != NULL);
	assert(histo != NULL);

	binDiv = 256/histo->bin;
	histo->nbPixel = img->nx*img->ny;
	histo->normalized = 0;

	/* For each pixel : */
	for (i = 0; i < img->nx; i++)
	{
		for (j = 0; j < img->ny; j++)
		{
			/* Get the color */
			unsigned char r = img->r[i][j];
			unsigned char g = img->g[i][j];
			unsigned char b = img->b[i][j];
			/* Get the associated bin */
			int rbin = r/binDiv;
			int gbin = g/binDiv;
			int bbin = b/binDiv;

			/* Add this pixel to the bin */
			histo->values[rbin][gbin][bbin]++;
		}
	}
}

void histogramNormalize(Histogram * const histo)
{
	int i, j, k;

	assert(histo != NULL);

	for (i = 0; i < histo->bin; i++)
	{
		for (j = 0; j < histo->bin; j++)
		{
			for (k = 0; k < histo->bin; k++)
			{
				/* For each value of the histogram, divide by the total number
				of pixels */
				histo->values[i][j][k] /= histo->nbPixel;
			}
		}
	}

	histo->normalized = 1;
}

void histogramDisplay(Histogram const * const histo, FILE* outputFile)
{
	int i, j, k;

	for (i = 0; i < histo->bin; i++)
	{
		for (j = 0; j < histo->bin; j++)
		{
			for (k = 0; k < histo->bin; k++)
			{
				fprintf(outputFile, "%f ", histo->values[k][j][i]);
			}
			fprintf(outputFile, "\n");
		}
		fprintf(outputFile, "\n");
	}
}

void histogramSaveAsSVM(Histogram const * const histo, FILE* SVMFile)
{
    int i, j, k;
    int counter = 0;

    assert(histo != NULL);
    assert(SVMFile != NULL);

	fprintf(SVMFile, "0 ");

	for (i = 0; i < histo->bin; i++)
	{
		for (j = 0; j < histo->bin; j++)
		{
			for (k = 0; k < histo->bin; k++)
			{
                counter++;
                if (histo->values[k][j][i] != 0)
                {
                    fprintf(SVMFile, "%d:%f ", counter, histo->values[k][j][i]);
                }
			}
		}
	}
	fprintf(SVMFile, "\n");
}
