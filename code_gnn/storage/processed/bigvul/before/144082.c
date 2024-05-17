png_set_keep_unknown_chunks(png_structp png_ptr, int keep, png_bytep
   chunk_list, int num_chunks)
{
   png_bytep new_list, p;
   int i, old_num_chunks;
   if (png_ptr == NULL)
      return;
   if (num_chunks == 0)
   {
      if (keep == PNG_HANDLE_CHUNK_ALWAYS || keep == PNG_HANDLE_CHUNK_IF_SAFE)
         png_ptr->flags |= PNG_FLAG_KEEP_UNKNOWN_CHUNKS;
      else
         png_ptr->flags &= ~PNG_FLAG_KEEP_UNKNOWN_CHUNKS;

      if (keep == PNG_HANDLE_CHUNK_ALWAYS)
         png_ptr->flags |= PNG_FLAG_KEEP_UNSAFE_CHUNKS;
      else
         png_ptr->flags &= ~PNG_FLAG_KEEP_UNSAFE_CHUNKS;
      return;
   }
   if (chunk_list == NULL)
      return;
   old_num_chunks = png_ptr->num_chunk_list;
   new_list=(png_bytep)png_malloc(png_ptr,
      (png_uint_32)
       (5*(num_chunks + old_num_chunks)));
   if (png_ptr->chunk_list != NULL)
   {
      png_memcpy(new_list, png_ptr->chunk_list,
          (png_size_t)(5*old_num_chunks));
      png_free(png_ptr, png_ptr->chunk_list);
      png_ptr->chunk_list=NULL;
   }
   png_memcpy(new_list + 5*old_num_chunks, chunk_list,
       (png_size_t)(5*num_chunks));
   for (p = new_list + 5*old_num_chunks + 4, i = 0; i<num_chunks; i++, p += 5)
      *p=(png_byte)keep;
   png_ptr->num_chunk_list = old_num_chunks + num_chunks;
   png_ptr->chunk_list = new_list;
#ifdef PNG_FREE_ME_SUPPORTED
   png_ptr->free_me |= PNG_FREE_LIST;
#endif
}
