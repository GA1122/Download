png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr)
{
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
   png_free_ptr free_fn = NULL;
   png_voidp mem_ptr = NULL;
#endif

   png_debug(1, "in png_destroy_write_struct");

   if (png_ptr_ptr != NULL)
   {
      png_ptr = *png_ptr_ptr;
#ifdef PNG_USER_MEM_SUPPORTED
      free_fn = png_ptr->free_fn;
      mem_ptr = png_ptr->mem_ptr;
#endif
   }

#ifdef PNG_USER_MEM_SUPPORTED
   if (png_ptr != NULL)
   {
      free_fn = png_ptr->free_fn;
      mem_ptr = png_ptr->mem_ptr;
   }
#endif

   if (info_ptr_ptr != NULL)
      info_ptr = *info_ptr_ptr;

   if (info_ptr != NULL)
   {
      if (png_ptr != NULL)
      {
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
        if (png_ptr->num_chunk_list)
        {
           png_free(png_ptr, png_ptr->chunk_list);
           png_ptr->chunk_list = NULL;
           png_ptr->num_chunk_list = 0;
        }
#endif
      }

#ifdef PNG_USER_MEM_SUPPORTED
      png_destroy_struct_2((png_voidp)info_ptr, (png_free_ptr)free_fn,
         (png_voidp)mem_ptr);
#else
      png_destroy_struct((png_voidp)info_ptr);
#endif
      *info_ptr_ptr = NULL;
   }

   if (png_ptr != NULL)
   {
      png_write_destroy(png_ptr);
#ifdef PNG_USER_MEM_SUPPORTED
      png_destroy_struct_2((png_voidp)png_ptr, (png_free_ptr)free_fn,
         (png_voidp)mem_ptr);
#else
      png_destroy_struct((png_voidp)png_ptr);
#endif
      *png_ptr_ptr = NULL;
   }
}
