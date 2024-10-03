#if !defined(PHOTOALBUM_H)
#define PHOTOALBUM_H

#define MAX_ALBUM_SIZE 8
#define SUCCESS 0
#define FAILURE -1

typedef struct photo {
   int id;
   char *description; /* dynamically-allocated string */
} Photo;

typedef struct album {
   int size;
   Photo *all_photos[MAX_ALBUM_SIZE];
} Album;

Photo *create_photo(int, const char *);
void print_photo(Photo *);
void destroy_photo(Photo *);
void initialize_album(Album *);
void print_album(const Album *);
void destroy_album(Album *);
void add_photo_to_album(Album *, int, const char *);

#endif
