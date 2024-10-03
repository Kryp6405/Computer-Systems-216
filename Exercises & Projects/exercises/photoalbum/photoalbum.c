#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoalbum.h"

Photo *create_photo(int id, const char *description){
    Photo *photo = (Photo *)malloc(sizeof(Photo));
    
    if(photo == NULL){
        return NULL;
    }

    photo->id = id;
    if(description != NULL){
        photo->description = (char*)malloc(strlen(description) + 1);
        if (photo->description == NULL) {
            free(photo);
            return NULL;
        }

        strncpy(photo->description, description, strlen(description) + 1);
    } else {
        photo->description = NULL;
    }

    return photo;   
}

void print_photo(Photo *photo){
    if(photo != NULL){
        printf("Photo ID: %d, Description: %s\n", photo->id, (photo->description != NULL) ? photo->description : "None");
    }
}

void destroy_photo(Photo *photo){
    if(photo != NULL){
        free(photo->description);
        free(photo);
    }
}

void initialize_album(Album *album){
    if(album != NULL){
        album->size = 0;
    }
}

void print_album(const Album *album){
    if(album != NULL){
        int i = 0;
        if(album->size == 0){
            printf("Album has no photos.\n");
        }
        else{
            for(i = 0; i < album->size; i++){
                print_photo(album->all_photos[i]);
            }
        }
    }
}

void destroy_album(Album *album){
    if(album != NULL){
        int i = 0;
        while(i < album->size){
            destroy_photo(album->all_photos[i++]);
        }
        album->size = 0;
    }
}

void add_photo_to_album(Album *album, int id, const char *description){
    if(album != NULL && album->size < MAX_ALBUM_SIZE) {
        Photo *new_photo = create_photo(id, description);
        if (new_photo != NULL) {
            album->all_photos[album->size++] = new_photo;
        }
    }
}