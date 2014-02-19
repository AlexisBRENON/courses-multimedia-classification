#include <assert.h>

#include "cimage.h"

CIMAGE* createCImage(int nx, int ny)
{
	/* Allocate the memory */
	CIMAGE* img = (CIMAGE*) malloc(sizeof(CIMAGE));
	int i;
	/* Initialise values */
	img->nx = nx;
	img->ny = ny;
	img->r = (unsigned char **) malloc(nx*sizeof(unsigned char *));
	img->g = (unsigned char **) malloc(nx*sizeof(unsigned char *));
	img->b = (unsigned char **) malloc(nx*sizeof(unsigned char *));
  	for (i = 0; i < nx; i++) {
  		img->r[i] = (unsigned char*) calloc(ny, sizeof(unsigned char));
  		img->g[i] = (unsigned char*) calloc(ny, sizeof(unsigned char));
  		img->b[i] = (unsigned char*) calloc(ny, sizeof(unsigned char));
	}

	return img;
}

void destroyCImage(CIMAGE** img)
{
	assert(img != NULL);
	assert(*img != NULL);

	int i;

	for (i = 0; i < (*img)->nx; i++)
	{
		free((*img)->r[i]);
		free((*img)->g[i]);
		free((*img)->b[i]);
	}
  free((*img)->r);
  free((*img)->g);
  free((*img)->b);
	(*img)->nx = 0;
	(*img)->ny = 0;

  free(*img);
	*img = NULL;
}

CIMAGE* loadCImage(char *name)
{
  /*-----------------------------*/
  /* lecture d'une image couleur */
  /*-----------------------------*/
  CIMAGE* cim = NULL;
  int i,j,nx,ny,nc;
  char command[4096],head[4];
  FILE *fp;
  /*-------------------------------------------------*/
  /* commande conversion vers le format PPM          */
  /* djpeg envoie le résultat sur la sortie standard */
  /*-------------------------------------------------*/
  if (strstr(name,"http://") == name) {
    sprintf(command,"wget -O - \"%s\" -o /dev/null | djpeg ",name);
  } else {
    sprintf(command,"djpeg \"%s\"",name);
  }
  /*-----------------------------------*/
  /* lecture d'une image au format PPM */
  /*-----------------------------------*/
  if ((fp = popen(command,"r")) == NULL) {
    fprintf(stderr,"Can't open PBM file from \"%s\" command, exiting",command);
    exit(1);
  }
  fscanf(fp,"%s\n%d %d\n%d\n",head,&nx,&ny,&nc);
  cim = createCImage(nx, ny);
  if (!strcmp(head,"P6")) { /* image couleur */
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        cim->r[i][j] = (unsigned char) getc(fp);
        cim->g[i][j] = (unsigned char) getc(fp);
        cim->b[i][j] = (unsigned char) getc(fp);
      }
    }
  } else if (!strcmp(head,"P5")) { /* image noir et blanc */
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        cim->r[i][j] = cim->g[i][j] = cim->b[i][j] = (unsigned char) getc(fp);
      }
    }
  } else { /* format non supporté */
    exit(1);
  }
  pclose(fp);

  return cim;
}

void saveCImage(CIMAGE const* const cim, char* name)
{
	FILE* fImg = fopen(name, "w");

	if (fImg)
	{
		fprintf(fImg, "P3\n%d %d\n255\n", cim->nx, cim->ny);
		int i, j;
		for (j = 0; j < cim->ny; j++)
		{
			for (i = 0; i < cim->nx; i++)
			{
				fprintf(fImg, "%d %d %d\n", cim->r[i][j], cim->g[i][j], cim->b[i][j]);
			}
		}

		fclose(fImg);
	}
	else
	{
		fprintf(stderr, "Impossible de créer le fichier : %s\n", name);
	}
}
