 zlib_advance(struct zlib *zlib, png_uint_32 nbytes)
   
 #  define ZLIB_TOO_FAR_BACK (-1)
 #  define ZLIB_OK           0
 #  define ZLIB_STREAM_END   1
 #  define ZLIB_FATAL        2
 {
  int state = zlib->state;
  int endrc = ZLIB_OK;
    png_uint_32 in_bytes = 0;
  struct file *file = zlib->file;
 
    assert(state >= 0);
 
  while (in_bytes < nbytes && endrc == ZLIB_OK)
  {
       png_uint_32 out_bytes;
  int flush;
       png_byte bIn = reread_byte(file);
       png_byte bOut;
 
  switch (state)
  {
  case 0:  
  {
  int file_bits = 8+(bIn >> 4);
  int new_bits = zlib->window_bits;
 
 
                 zlib->file_bits = file_bits;
  
                  
                 if (new_bits == 0)  
                  zlib->window_bits = file_bits;
//                   zlib->window_bits = ((file_bits > 15) ? 15 : file_bits);
  
                 else if (new_bits != file_bits)  
                    bIn = (png_byte)((bIn & 0xf) + ((new_bits-8) << 4));
  }
 
             zlib->header[0] = bIn;
             zlib->state = state = 1;
  break;
 
  case 1:  
  {
  int b2 = bIn & 0xe0;  
 
   
                b2 += 0x1f - ((zlib->header[0] << 8) + b2) % 0x1f;
 
   
 
                 if (bIn != b2)
                 {
                     
                    if (zlib->file_bits == zlib->window_bits)
                       zlib->cksum = 1;
 
                   bIn = (png_byte)b2;
  }
  }
 
             zlib->header[1] = bIn;
             zlib->state = state = 2;
  break;
 
  default:  
  break;
  }
 
   
       zlib->z.next_in = &bIn;
       zlib->z.avail_in = 1;
       zlib->z.next_out = &bOut;
       zlib->z.avail_out = 0;  
 
   
       flush = Z_NO_FLUSH;
       out_bytes = 0;
 
   
  for (;endrc == ZLIB_OK;
          flush = Z_SYNC_FLUSH,
          zlib->z.next_out = &bOut,
          zlib->z.avail_out = 1,
  ++out_bytes)
  {
          zlib->rc = inflate(&zlib->z, flush);
          out_bytes -= zlib->z.avail_out;
 
  switch (zlib->rc)
  {
  case Z_BUF_ERROR:
  if (zlib->z.avail_out == 0)
  continue;  
 
  if (zlib->z.avail_in == 0)
  break;  
 
   
                zlib_message(zlib, 1 );
                endrc = ZLIB_FATAL;  
  break;
 
  case Z_OK:
   
                assert(zlib->z.avail_out == 0 || zlib->z.avail_in == 0);
  continue;
 
  case Z_STREAM_END:
   
                zlib->state = 3;  
                endrc = ZLIB_STREAM_END;
  break;
 
  case Z_NEED_DICT:
                zlib_message(zlib, 0 );
                endrc = ZLIB_FATAL;
  break;
 
  case Z_DATA_ERROR:
   
  if (zlib->z.msg != NULL &&
                   strcmp(zlib->z.msg, "invalid distance too far back") == 0)
  {
                   endrc = ZLIB_TOO_FAR_BACK;
  break;
  }
   
 
  default:
                zlib_message(zlib, 0 );
                endrc = ZLIB_FATAL;
  break;
  }  
 
   
  break;
  }  
 
   
       zlib->uncompressed_digits = uarb_add32(zlib->uncompressed_bytes,
          zlib->uncompressed_digits, out_bytes);
 
   
       assert(zlib->z.avail_in == 0 || endrc != ZLIB_OK);
 
       in_bytes += 1 - zlib->z.avail_in;
  }  
 
    assert(in_bytes == nbytes || endrc != ZLIB_OK);
 
   
    zlib->compressed_digits = uarb_add32(zlib->compressed_bytes,
       zlib->compressed_digits, in_bytes - zlib->z.avail_in);
 
   
  if (endrc == ZLIB_STREAM_END && zlib->window_bits < zlib->ok_bits)
  {
  struct chunk *chunk = zlib->chunk;
 
       chunk->uncompressed_digits = uarb_copy(chunk->uncompressed_bytes,
          zlib->uncompressed_bytes, zlib->uncompressed_digits);
       chunk->compressed_digits = uarb_copy(chunk->compressed_bytes,
          zlib->compressed_bytes, zlib->compressed_digits);
       chunk->rewrite_buffer[0] = zlib->header[0];
       chunk->rewrite_buffer[1] = zlib->header[1];
 
  if (zlib->window_bits != zlib->file_bits || zlib->cksum)
  {
   
          chunk->rewrite_offset = zlib->rewrite_offset;
          chunk->rewrite_length = 2;
  }
 
  else
  {
          chunk->rewrite_offset = 0;
          chunk->rewrite_length = 0;
  }
 
  if (in_bytes < nbytes)
          chunk_message(chunk, "extra compressed data");
 
       zlib->extra_bytes = nbytes - in_bytes;
       zlib->ok_bits = zlib->window_bits;
  }
 
  return endrc;
 }