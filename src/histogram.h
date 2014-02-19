#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <stdio.h>

#include "cimage.h"

typedef struct
{
	float*** values;
	int bin;
	int nbPixel;
	int normalized;
} Histogram;


/**
  * @brief Create (allocate) and initialize a new histogram
  *
  * @param[in] bin Number of bins of the histogram
  * @return Newly allocated histogram
  */
Histogram* histogramCreate(int bin);

/**
  * @brief Initialize an already allocated Histogram.
  *
  * @param[in-out] histo Allocated histogram to initialize.
  * @param[in] bin Number of bins for the histogram
  */
void histogramInit(Histogram * const histo, int bin);

/**
  * @brief Release memory held by an histogram.
  *
  * @param[in-out] histo Histogeam to release
  */
void histogramRelease(Histogram* histo);

/**
  * @brief Destroy (deallocate) a histogram
  * After the call of this function, histo will point to NULL
  *
  * @param[in-out] histo Histogram to destroy
  */
void histogramDestroy(Histogram** histo);

/**
  * @brief Compute histogram from a CIMAGE.
  *
  * @param[in] img Pointer to the CIMAGE structure to get histogram from.
  * @param[in-out] histo Histogram to fullfill
  */
void histogramCompute(CIMAGE const * const img, Histogram * const histo);

/**
  * @brief Normalize the histogram.
  *
  * @param[in-out] histo Histogram to normalize
  */
void histogramNormalize(Histogram * const histo);

/**
  * @brief Display an histogram.
  *
  * @param[in] histo Histogram to display
  * @param[in-out] outputFile file handler to write
  */
void histogramDisplay(Histogram const * const histo, FILE* outputFile);

/**
  * @brief Save histogram as SVM to the already opened SVMFile.
  *
  * @param[in] histo Histogram to save
  * @param[in-out] SVMFile File where to write to.
  */
void histogramSaveAsSVM(Histogram const * const histo, FILE* SVMFile);

#endif
