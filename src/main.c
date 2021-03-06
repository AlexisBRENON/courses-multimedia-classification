#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>

#include "cimage.h"
#include "histogram.h"

void printUsage (char const* const execFile)
{
    fprintf(stdout, "\
Usage : %s -i <filename> [-h] [-m <value>] [-d <dir>] [-b <value>]\n\
    -i, --input <filename> : Define the file containg pathes (local or\n\
distant) to images.\n\
    -a, --annotation <filename> : Path to the file containing annotations\n\
    -h, --help : Display this help and quit.\n\
    -m, --max-file <value> : Treat only <value> files from the list\n\
    -d, --output-dir <dir> : Directory where to generate new files\n\
    -b, --bin <value> : Define number of bin for histograms\n",
    execFile);
}


int main (int argc, char* argv[])
{
    int result = EXIT_SUCCESS;
    int flagExit = 0;
    int opt = 0;
    int flagInputSet = 0;
    int flagAnnotationSet = 0;
    char inputListFileName[1024] = {0};
    char annotationFileName[1024] = {0};
    char outputDirName[1024] = "./";
    int bin = 4;
    unsigned int maxFiles = INT_MAX;
    static struct option longopts[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"input", required_argument, NULL, 'i'},
        {"annotation", required_argument, NULL, 'a'},
        {"max-file", required_argument, NULL, 'm'},
        {"output-dir", required_argument, NULL, 'd'},
        {"bin", required_argument, NULL, 'b'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "hi:a:m:d:b:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                printUsage(argv[0]);
                flagExit = 1;
                break;
            case 'i':
                flagInputSet = 1;
                strncpy(inputListFileName, optarg, 1023);
                break;
            case 'a':
                flagAnnotationSet = 1;
                strncpy(annotationFileName, optarg, 1023);
                break;
            case 'm':
                maxFiles = atoi(optarg);
                break;
            case 'd':
                strncpy(outputDirName, optarg, 1023);
                break;
            case 'b':
                bin = atoi(optarg);
                break;
            case '?':
                break;
        }
    }

    if (flagExit == 0)
    {
        if (flagInputSet)
        {
            FILE *inputListFile = fopen(inputListFileName, "r");
            if (inputListFile == NULL)
            {
                fprintf(stderr, "Unable to open %s\n", argv[1]);
                result = EXIT_FAILURE;
            }
            else
            {
                char* outputFileName = (char*) malloc(
                    (strlen(outputDirName)+16)*sizeof(char));
                strcpy(outputFileName, outputDirName);
                FILE *SVMFile = fopen(strcat(outputFileName, "histo.svm"), "w");
                if (SVMFile == NULL)
                {
                    fprintf(stderr, "Unable to create %s\n", outputFileName);
                }
                else
                {
                    char url[1024] = {0};
                    CIMAGE *img = NULL;
                    Histogram *histo = NULL;
                    int counter = 0;
                    while (feof(inputListFile) == 0 && counter < maxFiles)
                    {
                        counter++;
                        if (fscanf(inputListFile, "%s\n", url) > 0)
                        {
                            img = loadCImage(url);
                            histo = histogramCreate(bin);
                            histogramCompute(img, histo);
                            histogramNormalize(histo);
                            histogramSaveAsSVM(histo, SVMFile);
                            histogramDestroy(&histo);
                            destroyCImage(&img);
                        }
                        if (counter % 50 == 0)
                        {
                            fprintf(stderr, "Images done : %d\n", counter);
                        }
                    }
                    fclose(SVMFile);
                }
                free(outputFileName);
                fclose(inputListFile);
            }
        }
        if (flagAnnotationSet)
        {
            char* SVMFileName = (char*) malloc(
                    (strlen(outputDirName)+16)*sizeof(char));
            strcpy(SVMFileName, outputDirName);
            FILE *SVMFile = fopen(strcat(SVMFileName, "histo.svm"), "r");
            if (SVMFile != NULL)
            {
                FILE *annotationFile = fopen(annotationFileName, "r");
                if (annotationFile != NULL)
                {
                    char* outputFileName = (char*) malloc(
                        (strlen(outputDirName)+strlen("annotated.svm")+1)*sizeof(char));
                        strcpy(outputFileName, outputDirName);
                    FILE *resultFile = fopen(strcat(outputFileName, "annotated.svm"), "w");   
                    if (resultFile != NULL)
                    {
                        int imgClass;
                        int counter = 0;
                        while (feof(annotationFile) == 0 && counter < maxFiles)
                        {
                            char strHisto[1024] = {0};
                            counter++;

                            if(fscanf(annotationFile, "%*d_%*d %d\n", &imgClass) > 0)
                            {
                                imgClass = imgClass > 0 ? 1 : -1;
                                if (fscanf(SVMFile, "0 %[0123456789.: ]\n", strHisto) > 0)
                                {
                                    fprintf(resultFile, "%d %s\n", imgClass, strHisto);
                                }
                            }
                            
                        }
                        fclose(resultFile);
                    }
                    else
                    {
                        fprintf(stderr, "Unable to create the annotated histograms file.\n");
                    }
                    fclose(annotationFile);
                }
                else
                {
                    fprintf(stderr, "Annotation file unreadable (does it exist ?)\n");
                }
                fclose(SVMFile);
            }
            else
            {
                fprintf(stderr, "SVMFile unreadable (does it exist ?)\n");
            }
        }
    }

    return result;
}
