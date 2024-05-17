 zlib_init(struct zlib *zlib, struct IDAT *idat, struct chunk *chunk,
  int window_bits, png_uint_32 offset)
   
 {
    CLEAR(*zlib);
 
    zlib->idat = idat;
    zlib->chunk = chunk;
    zlib->file = chunk->file;
    zlib->global = chunk->global;
    zlib->rewrite_offset = offset;  
 
   
    zlib->z.next_in = Z_NULL;
    zlib->z.avail_in = 0;
    zlib->z.zalloc = Z_NULL;
    zlib->z.zfree = Z_NULL;
    zlib->z.opaque = Z_NULL;
 
    zlib->state = -1;
    zlib->window_bits = window_bits;
 
    zlib->compressed_digits = 0;
    zlib->uncompressed_digits = 0;
 
 
      
   zlib->file_bits = 16;
//    zlib->file_bits = 24;
     zlib->ok_bits = 16;  
     zlib->cksum = 0;  
  
   
    zlib->rc = inflateInit2(&zlib->z, 0);
  if (zlib->rc != Z_OK)
  {
       zlib_message(zlib, 1 );
  return 0;
  }
 
  else
  {
       zlib->state = 0;  
  return 1;
  }
 }