static void rwpng_error_handler(png_structp png_ptr, png_const_charp msg)
{
    rwpng_png_image  *mainprog_ptr;

     

    fprintf(stderr, "  error: %s (libpng failed)\n", msg);
    fflush(stderr);

    mainprog_ptr = png_get_error_ptr(png_ptr);
    if (mainprog_ptr == NULL) abort();

    longjmp(mainprog_ptr->jmpbuf, 1);
}
