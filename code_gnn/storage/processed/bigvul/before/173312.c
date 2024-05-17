read_chunk(struct file *file)
  
{
   png_uint_32 length = file->length;
   png_uint_32 type = file->type;

  
 if (type == 0)
      stop(file, UNEXPECTED_ERROR_CODE, "read beyond IEND");

 if (file->global->verbose > 2)
 {
      fputs("   ", stderr);
      type_name(type, stderr);
      fprintf(stderr, " %lu\n", (unsigned long)length);
 }

  
   file->crc = crc_init_4(type);
 if (crc_read_many(file, length))  
 {
      png_uint_32 file_crc;  
 unsigned int nread = read_4(file, &file_crc);

 if (nread == 4)
 {
 if (type != png_IEND)  
 {
            png_uint_32 next_length;

            nread += read_4(file, &next_length);
 if (nread == 8 && next_length <= 0x7fffffff)
 {
               png_uint_32 next_type;

               nread += read_4(file, &next_type);

 if (nread == 12 && chunk_type_valid(next_type))
 {
  
                  file->read_count -= 8;
                  process_chunk(file, file_crc, next_length, next_type);
 return;
 }
 }
 }

 else  
 {
            process_chunk(file, file_crc, 0, 0);
 return;
 }
 }
 }

  
   sync_stream(file);
}
