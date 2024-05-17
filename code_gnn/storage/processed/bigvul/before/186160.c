 png_text_compress(png_structp png_ptr,
         png_charp text, png_size_t text_len, int compression,
         compression_state *comp)
 {
    int ret;
 
    comp->num_output_ptr = 0;
    comp->max_output_ptr = 0;
    comp->output_ptr = NULL;
    comp->input = NULL;
    comp->input_len = 0;
 
     
    if (compression == PNG_TEXT_COMPRESSION_NONE)
    {
        comp->input = text;
        comp->input_len = text_len;
        return((int)text_len);
    }
 
    if (compression >= PNG_TEXT_COMPRESSION_LAST)
    {
 #if defined(PNG_STDIO_SUPPORTED) && !defined(_WIN32_WCE)
       char msg[50];
       png_snprintf(msg, 50, "Unknown compression type %d", compression);
       png_warning(png_ptr, msg);
 #else
       png_warning(png_ptr, "Unknown compression type");
 #endif
    }
 
     
 
     
    png_ptr->zstream.avail_in = (uInt)text_len;
    png_ptr->zstream.next_in = (Bytef *)text;
    png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
    png_ptr->zstream.next_out = (Bytef *)png_ptr->zbuf;
 
     
    do
    {
        
       ret = deflate(&png_ptr->zstream, Z_NO_FLUSH);
       if (ret != Z_OK)
       {
           
          if (png_ptr->zstream.msg != NULL)
             png_error(png_ptr, png_ptr->zstream.msg);
          else
             png_error(png_ptr, "zlib error");
       }
        
       if (!(png_ptr->zstream.avail_out))
       {
           
          if (comp->num_output_ptr >= comp->max_output_ptr)
          {
             int old_max;
 
             old_max = comp->max_output_ptr;
             comp->max_output_ptr = comp->num_output_ptr + 4;
             if (comp->output_ptr != NULL)
             {
                png_charpp old_ptr;
 
                 old_ptr = comp->output_ptr;
                 comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                    (png_uint_32)
                  (comp->max_output_ptr * png_sizeof(png_charpp)));
//                   (comp->max_output_ptr * png_sizeof(png_charp)));
                 png_memcpy(comp->output_ptr, old_ptr, old_max
                    * png_sizeof(png_charp));
                 png_free(png_ptr, old_ptr);
             }
             else
                comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                   (png_uint_32)
                   (comp->max_output_ptr * png_sizeof(png_charp)));
          }
 
           
          comp->output_ptr[comp->num_output_ptr] =
             (png_charp)png_malloc(png_ptr,
             (png_uint_32)png_ptr->zbuf_size);
          png_memcpy(comp->output_ptr[comp->num_output_ptr], png_ptr->zbuf,
             png_ptr->zbuf_size);
          comp->num_output_ptr++;
 
           
          png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
          png_ptr->zstream.next_out = png_ptr->zbuf;
       }
     
    } while (png_ptr->zstream.avail_in);
 
     
    do
    {
        
       ret = deflate(&png_ptr->zstream, Z_FINISH);
 
       if (ret == Z_OK)
       {
           
          if (!(png_ptr->zstream.avail_out))
          {
              
             if (comp->num_output_ptr >= comp->max_output_ptr)
             {
                int old_max;
 
                old_max = comp->max_output_ptr;
                comp->max_output_ptr = comp->num_output_ptr + 4;
                if (comp->output_ptr != NULL)
                {
                   png_charpp old_ptr;
 
                   old_ptr = comp->output_ptr;
                    
                   comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                      (png_uint_32)(comp->max_output_ptr *
                      png_sizeof(png_charp)));
                   png_memcpy(comp->output_ptr, old_ptr,
                      old_max * png_sizeof(png_charp));
                   png_free(png_ptr, old_ptr);
                }
                else
                   comp->output_ptr = (png_charpp)png_malloc(png_ptr,
                      (png_uint_32)(comp->max_output_ptr *
                      png_sizeof(png_charp)));
             }
 
              
             comp->output_ptr[comp->num_output_ptr] =
                (png_charp)png_malloc(png_ptr,
                (png_uint_32)png_ptr->zbuf_size);
             png_memcpy(comp->output_ptr[comp->num_output_ptr], png_ptr->zbuf,
                png_ptr->zbuf_size);
             comp->num_output_ptr++;
 
              
             png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
             png_ptr->zstream.next_out = png_ptr->zbuf;
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
 
     
    text_len = png_ptr->zbuf_size * comp->num_output_ptr;
    if (png_ptr->zstream.avail_out < png_ptr->zbuf_size)
       text_len += png_ptr->zbuf_size - (png_size_t)png_ptr->zstream.avail_out;
 
    return((int)text_len);
 }