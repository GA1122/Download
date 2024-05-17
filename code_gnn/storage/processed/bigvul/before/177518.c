 static int read_frame(FILE *f, vpx_image_t *img) {
 size_t nbytes, to_read;
 int    res = 1;

    to_read = img->w*img->h*3/2;
    nbytes = fread(img->planes[0], 1, to_read, f);
 if(nbytes != to_read) {
        res = 0;
 if(nbytes > 0)
            printf("Warning: Read partial frame. Check your width & height!\n");
 }
 return res;
}
