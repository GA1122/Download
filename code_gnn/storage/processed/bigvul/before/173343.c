zlib_reset(struct zlib *zlib, int window_bits)
  
{
   assert(zlib->state >= 0);  

   zlib->z.next_in = Z_NULL;
   zlib->z.avail_in = 0;
   zlib->z.next_out = Z_NULL;
   zlib->z.avail_out = 0;

   zlib->window_bits = window_bits;
   zlib->compressed_digits = 0;
   zlib->uncompressed_digits = 0;

   zlib->state = 0;  
   zlib->rc = inflateReset2(&zlib->z, 0);
 if (zlib->rc != Z_OK)
 {
      zlib_message(zlib, 1 );
 return 0;
 }

 return 1;
}
