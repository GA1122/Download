png_write_compressed_data_out(png_structp png_ptr, compression_state *comp)
{
   int i;

    
   if (comp->input)
   {
      png_write_chunk_data(png_ptr, (png_bytep)comp->input,
                            (png_size_t)comp->input_len);
      return;
   }

    
   for (i = 0; i < comp->num_output_ptr; i++)
   {
      png_write_chunk_data(png_ptr, (png_bytep)comp->output_ptr[i],
         (png_size_t)png_ptr->zbuf_size);
      png_free(png_ptr, comp->output_ptr[i]);
       comp->output_ptr[i]=NULL;
   }
   if (comp->max_output_ptr != 0)
      png_free(png_ptr, comp->output_ptr);
       comp->output_ptr=NULL;
    
   if (png_ptr->zstream.avail_out < (png_uint_32)png_ptr->zbuf_size)
      png_write_chunk_data(png_ptr, png_ptr->zbuf,
         (png_size_t)(png_ptr->zbuf_size - png_ptr->zstream.avail_out));

    
   deflateReset(&png_ptr->zstream);
   png_ptr->zstream.data_type = Z_BINARY;
}
