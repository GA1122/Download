file_init(struct file *file, struct global *global, const char *file_name,
 const char *out_name, void *alloc_ptr, void (*alloc)(struct file*,int))
  
{
   CLEAR(*file);
   file->global = global;

   file->file_name = file_name;
   file->out_name = out_name;
   file->status_code = 0;
   file->read_errno = 0;
   file->write_errno = 0;

   file->file = NULL;
   file->out = NULL;
  

   file->read_count = 0;
   file->state = STATE_SIGNATURE;

   file->chunk = NULL;
   file->idat = NULL;

   file->alloc_ptr = alloc_ptr;
   file->alloc = alloc;

  
   assert(file_name != NULL);
   file->file = fopen(file_name, "rb");

 if (file->file == NULL)
 {
      file->read_errno = errno;
      file->status_code |= FILE_ERROR;
  
      perror(file_name);
 return FILE_ERROR;
 }

 if (out_name != NULL)
 {
      file->out = fopen(out_name, "wb");

 if (file->out == NULL)
 {
         file->write_errno = errno;
         file->status_code |= WRITE_ERROR;
         perror(out_name);
 return WRITE_ERROR;
 }
 }

 return 0;
}
