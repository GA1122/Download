IDAT_init(struct IDAT * const idat, struct file * const file)
  
{
   assert(file->chunk == NULL);
   assert(file->idat == NULL);

   CLEAR(*idat);

   idat->file = file;
   idat->global = file->global;

  
   idat->global->idat_cache.count = 0;
   idat->idat_list_head = idat->idat_list_tail = &idat->global->idat_cache;

  
   file->alloc(file, 0 );
   assert(file->chunk != NULL);

  
   file->idat = idat;
}
