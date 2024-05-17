int writepng_encode_row(mainprog_info *mainprog_ptr)  
{
    png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
    png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;


  

 if (setjmp(mainprog_ptr->jmpbuf)) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        mainprog_ptr->png_ptr = NULL;
        mainprog_ptr->info_ptr = NULL;
 return 2;
 }


  

    png_write_row(png_ptr, mainprog_ptr->image_data);

 return 0;
}
