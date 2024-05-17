  png_write_finish_row(png_structp png_ptr)
  {
  #ifdef PNG_WRITE_INTERLACING_SUPPORTED
// #ifndef PNG_USE_GLOBAL_ARRAYS
      
  
      
    int png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};
 
     
    int png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};
 
     
    int png_pass_ystart[7] = {0, 0, 4, 0, 2, 0, 1};
  
      
     int png_pass_yinc[7] = {8, 8, 8, 4, 4, 2, 2};
// #endif
  #endif
  
     int ret;
 
    png_debug(1, "in png_write_finish_row");
 
     
    png_ptr->row_number++;
 
     
    if (png_ptr->row_number < png_ptr->num_rows)
       return;
 
 #ifdef PNG_WRITE_INTERLACING_SUPPORTED
     
    if (png_ptr->interlaced)
    {
       png_ptr->row_number = 0;
       if (png_ptr->transformations & PNG_INTERLACE)
       {
          png_ptr->pass++;
       }
       else
       {
           
          do
          {
             png_ptr->pass++;
             if (png_ptr->pass >= 7)
                break;
             png_ptr->usr_width = (png_ptr->width +
                png_pass_inc[png_ptr->pass] - 1 -
                png_pass_start[png_ptr->pass]) /
                png_pass_inc[png_ptr->pass];
             png_ptr->num_rows = (png_ptr->height +
                png_pass_yinc[png_ptr->pass] - 1 -
                png_pass_ystart[png_ptr->pass]) /
                png_pass_yinc[png_ptr->pass];
             if (png_ptr->transformations & PNG_INTERLACE)
                break;
          } while (png_ptr->usr_width == 0 || png_ptr->num_rows == 0);
 
       }
 
        
       if (png_ptr->pass < 7)
       {
          if (png_ptr->prev_row != NULL)
             png_memset(png_ptr->prev_row, 0,
                (png_size_t)(PNG_ROWBYTES(png_ptr->usr_channels*
                png_ptr->usr_bit_depth, png_ptr->width)) + 1);
          return;
       }
    }
 #endif
 
     
    do
    {
        
       ret = deflate(&png_ptr->zstream, Z_FINISH);
        
       if (ret == Z_OK)
       {
           
          if (!(png_ptr->zstream.avail_out))
          {
             png_write_IDAT(png_ptr, png_ptr->zbuf, png_ptr->zbuf_size);
             png_ptr->zstream.next_out = png_ptr->zbuf;
             png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
          }
       }
       else if (ret != Z_STREAM_END)
       {
          if (png_ptr->zstream.msg != NULL)
             png_error(png_ptr, png_ptr->zstream.msg);
          else
             png_error(png_ptr, "zlib error");
       }
    } while (ret != Z_STREAM_END);
 
     
    if (png_ptr->zstream.avail_out < png_ptr->zbuf_size)
    {
       png_write_IDAT(png_ptr, png_ptr->zbuf, png_ptr->zbuf_size -
          png_ptr->zstream.avail_out);
    }
 
    deflateReset(&png_ptr->zstream);
    png_ptr->zstream.data_type = Z_BINARY;
 }