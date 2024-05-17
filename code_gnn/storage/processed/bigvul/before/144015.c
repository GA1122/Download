png_read_push_finish_row(png_structp png_ptr)
{
#ifdef PNG_USE_LOCAL_ARRAYS
    

    
   PNG_CONST int FARDATA png_pass_start[] = {0, 4, 0, 2, 0, 1, 0};

    
   PNG_CONST int FARDATA png_pass_inc[] = {8, 8, 4, 4, 2, 2, 1};

    
   PNG_CONST int FARDATA png_pass_ystart[] = {0, 0, 4, 0, 2, 0, 1};

    
   PNG_CONST int FARDATA png_pass_yinc[] = {8, 8, 8, 4, 4, 2, 2};

    
#endif

   png_ptr->row_number++;
   if (png_ptr->row_number < png_ptr->num_rows)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (png_ptr->interlaced)
   {
      png_ptr->row_number = 0;
      png_memset_check(png_ptr, png_ptr->prev_row, 0,
         png_ptr->rowbytes + 1);
      do
      {
         png_ptr->pass++;
         if ((png_ptr->pass == 1 && png_ptr->width < 5) ||
             (png_ptr->pass == 3 && png_ptr->width < 3) ||
             (png_ptr->pass == 5 && png_ptr->width < 2))
           png_ptr->pass++;

         if (png_ptr->pass > 7)
            png_ptr->pass--;

         if (png_ptr->pass >= 7)
            break;

         png_ptr->iwidth = (png_ptr->width +
            png_pass_inc[png_ptr->pass] - 1 -
            png_pass_start[png_ptr->pass]) /
            png_pass_inc[png_ptr->pass];

         if (png_ptr->transformations & PNG_INTERLACE)
            break;

         png_ptr->num_rows = (png_ptr->height +
            png_pass_yinc[png_ptr->pass] - 1 -
            png_pass_ystart[png_ptr->pass]) /
            png_pass_yinc[png_ptr->pass];

      } while (png_ptr->iwidth == 0 || png_ptr->num_rows == 0);
   }
#endif  
}
