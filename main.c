
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

//cell that has x and y position of a pixel
double org_entropy;
double new_entropy;


// Computes the entropy of the given image
double compute_entropy(unsigned char *img, int width, int height, int channels) {
    // Compute the probability of each gray level
    int counts[256] = {0};
    for(int i = 0; i < width*height*channels; i+=3) {
        unsigned char gray = (img[i] + img[i+1] + img[i+2])/3;
        counts[gray]++;
        //printf("%u\n", gray);
    }

    // Compute the entropy
    double entropy = 0;
    for(int i = 0; i < 256; i++) {
        if(counts[i] == 0) {
            continue;
        }
        double p = (double)counts[i] / (double)(width*height);
        entropy -= p*log2(p);
    }
    return entropy;
}

int main(void) {
while(1){
    int width, height, channels;
    char img_name[50] ;
    int iterations  ;

    //enter image name or path
    printf("Enter the image's name(if in project's folder) else the path : ") ;
    scanf("%s" , &img_name) ;


    //load the image
    unsigned char *img = stbi_load(img_name, &width, &height, &channels, 0);


    //check if the image is loaded successfully
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    //set the number of iterations
    printf("Set the number of iterations : ")  ;
    scanf("%i",  &iterations) ;

    //put the orginal image in a matrix
    unsigned char **image_matrix = malloc(sizeof(unsigned char *)*height);
    for(int i = 0; i < height; i++)
        image_matrix[i] = malloc(sizeof(unsigned char)*width*channels);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width*channels; j++)
            image_matrix[i][j] = img[i*width*channels + j];
    }
    //Compute original image entropy
    org_entropy=compute_entropy(img,width,height,channels);
    //freeing the img
    free(img);



    //the new scattered matrix
    unsigned char **image_new_matrix = malloc(sizeof(unsigned char *)*height);
    for(int i = 0; i < height; i++)
        image_new_matrix[i] = malloc(sizeof(unsigned char)*width*channels);


    //les changements avec les itérations
    for(int w=0;w<iterations;w++) {
       for(int i = 0; i < height; i++){
           for(int j = 0; j < width; j++){

                for(int k = 0; k < channels ; k++){
                    image_new_matrix[(i + j)   % height][((i + 2*j) % width)*channels+k]=image_matrix[i][j*channels+k];
                }
            }
        }
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width*channels; j++)
                image_matrix[i][j] = image_new_matrix[i][j];


        }

    }


    //liberer une matrice de la memoire
    for(int i = height-1 ; i = 0 ; i--)
        free(image_matrix[i]);
    free(image_matrix);




    //creation du tableau dynamique pour qu'on puisse afficher l'image avec la biblio stb_image
    unsigned char *nimg = malloc(sizeof(unsigned char)*height*width*channels);


    //changement de la matrice en un tableau dynamique
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width*channels; j++)
            nimg[i*width*channels + j]=image_new_matrix[i][j] ;
    }

    //save encrypted image as image_arnold in project folder

    stbi_write_jpg("image_arnold.jpg", width, height, channels, nimg, 100);
    //load new parameters of new image with stbi load
    nimg = stbi_load("image_arnold.jpg", &width, &height, &channels, 0);
    //compute new image entropy
    new_entropy=compute_entropy(nimg,width,height,channels);
    //liberer le tableau et la matrice
    free(nimg);
    for(int i = height-1 ; i = 0 ; i--)
        free(image_new_matrix[i]);
    free(image_new_matrix);
    printf("Entropy of the orginal image is:  %f\n ",org_entropy);
    printf("Entropy of the encrypted image is:  %f\n ",new_entropy);

}

return 0;


}
