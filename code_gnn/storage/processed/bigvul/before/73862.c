static pngquant_error rwpng_write_image_init(rwpng_png_image *mainprog_ptr, png_structpp png_ptr_p, png_infopp info_ptr_p, int fast_compression)
{
     

    *png_ptr_p = png_create_write_struct(PNG_LIBPNG_VER_STRING, mainprog_ptr, rwpng_error_handler, NULL);

    if (!(*png_ptr_p)) {
        return LIBPNG_INIT_ERROR;    
    }

    *info_ptr_p = png_create_info_struct(*png_ptr_p);
    if (!(*info_ptr_p)) {
        png_destroy_write_struct(png_ptr_p, NULL);
        return LIBPNG_INIT_ERROR;    
    }

     

    if (setjmp(mainprog_ptr->jmpbuf)) {
        png_destroy_write_struct(png_ptr_p, info_ptr_p);
        return LIBPNG_INIT_ERROR;    
    }

    png_set_compression_level(*png_ptr_p, fast_compression ? Z_BEST_SPEED : Z_BEST_COMPRESSION);
    png_set_compression_mem_level(*png_ptr_p, fast_compression ? 9 : 5);  

    return SUCCESS;
}
