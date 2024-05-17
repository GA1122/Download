crc_read_many(struct file *file, png_uint_32 length)
  
{
 if (length > 0)
 {
      png_uint_32 crc = file->crc;

 do
 {
 int ch = read_byte(file);

 if (ch == EOF)
 return 0;  

         crc = crc_one_byte(crc, ch);
 }
 while (--length > 0);

      file->crc = crc;
 }

 return 1;  
}
