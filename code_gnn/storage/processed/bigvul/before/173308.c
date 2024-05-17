process_zTXt_iCCP(struct file *file)
  
{
 struct chunk *chunk = file->chunk;
   png_uint_32 length;
   png_uint_32 index = 0;

   assert(chunk != NULL && file->idat == NULL);
   length = chunk->chunk_length;
   setpos(chunk);

 while (length >= 9)
 {
 --length;
 ++index;
 if (reread_byte(file) == 0)  
 {
 --length;
 ++index;
 (void)reread_byte(file);  
 return zlib_check(file, index);
 }
 }

   chunk_message(chunk, "too short");
 return 0;  
}
