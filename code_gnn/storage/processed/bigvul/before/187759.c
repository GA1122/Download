 process_IDAT(struct file *file)
   
 {
  struct IDAT_list *list;
 
    assert(file->idat != NULL && file->chunk != NULL);
 
   
  list = file->idat->idat_list_tail;
 
  if (list->count == list->length)
  {
  list = IDAT_list_extend(list);
 
  if (list == NULL)
          stop(file, READ_ERROR_CODE, "out of memory");
 
   
 
        list->count = 0;
        file->idat->idat_list_tail = list;
     }
// 
      
     list->lengths[(list->count)++] = file->chunk->chunk_length;
  
   
  if (file->type == png_IDAT)
  return 0;  
 
   
    setpos(file->chunk);
 
  if (zlib_check(file, 0))
  {
  struct IDAT *idat;
  int cmp;
 
   
       cmp = uarb_cmp(file->image_bytes, file->image_digits,
          file->chunk->uncompressed_bytes, file->chunk->uncompressed_digits);
 
  if (cmp < 0)
          type_message(file, png_IDAT, "extra uncompressed data");
 
  else if (cmp > 0)
          stop(file, LIBPNG_ERROR_CODE, "IDAT: uncompressed data too small");
 
   
       setpos(file->chunk);
 
       idat = file->idat;
       idat->idat_cur = idat->idat_list_head;
       idat->idat_length = idat->idat_cur->lengths[0];
       idat->idat_count = 0;  
       idat->idat_index = 0;  
 
   
       file->chunk->chunk_length = rechunk_length(idat);
 
   
       file->state = STATE_IDAT;
 
  return 1;
  }
 
  else  
       stop(file, ZLIB_ERROR_CODE, "could not uncompress IDAT");
 }