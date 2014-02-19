#ifndef __CIMAGE_H__
#define __CIMAGE_H__

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/**
  * @typedef Define structure representing a RGB picture.
  */ 
typedef struct {
    int nx;             /**< Taille horizontale    */
    int ny;             /**< Taille verticale      */
    unsigned char **r;  /**< Plan de couleur rouge */
    unsigned char **g;  /**< Plan de couleur verte */
    unsigned char **b;  /**< Plan de couleur bleue */
} CIMAGE;

/**
  * @brief Allocate a new image to fullfill.
  *
  * @param[in] nx Horizontal size of the image.
  * @param[in] ny Vertical size of the image.
  * @return A pointer to a newly allocated CIMAGE structure
  */
CIMAGE* createCImage(int nx, int ny);
/**
  * @brief Destroy a new image. After this call, img will point to NULL.
  *
  * @param[in-out] img CIMAGE structure to destroy.
  */
void destroyCImage(CIMAGE** img);

/**
  * @brief Fill the image from the jpeg file name
  *
  * @param[in] name Filename of the file to load
  * @return A pointer to a newly allocated and filled CIMAGE structure
  */
CIMAGE* loadCImage(char *name);
/**
  * @brief Save an image as PPM file.
  *
  * @param[in] cim Image to save
  * @param[in] name Filename of the file to create
  */
void saveCImage(CIMAGE const* const cim, char* name);

#endif
