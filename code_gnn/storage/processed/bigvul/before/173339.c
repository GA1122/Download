zlib_check(struct file *file, png_uint_32 offset)
  
{
 fpos_t start_pos;
 struct zlib zlib;

  
   file_getpos(file, &start_pos);

  
 if (zlib_init(&zlib, file->idat, file->chunk, 0 , offset))
 {
 int min_bits, max_bits, rc;

  
      rc = zlib_run(&zlib);

 switch (rc)
 {
 case ZLIB_TOO_FAR_BACK:
  
            file->status_code |= TOO_FAR_BACK;
            min_bits = zlib.window_bits + 1;
            max_bits = 15;
 break;

 case ZLIB_STREAM_END:
 if (!zlib.global->optimize_zlib &&
               zlib.window_bits == zlib.file_bits && !zlib.cksum)
 {
  
               zlib_end(&zlib);
 return 1;
 }

            max_bits = max_window_bits(zlib.uncompressed_bytes,
               zlib.uncompressed_digits);
 if (zlib.ok_bits < max_bits)
               max_bits = zlib.ok_bits;
            min_bits = 8;

  
 if (zlib.cksum)
               chunk_message(zlib.chunk, "zlib checkum");
 break;


 case ZLIB_OK:
  
            zlib.z.msg = PNGZ_MSG_CAST("[truncated]");
            zlib_message(&zlib, 0 );
  

 default:
  
            zlib_end(&zlib);
 return 0;
 }

  
 while (min_bits < max_bits || max_bits < zlib.ok_bits )
 {
 int test_bits = (min_bits + max_bits) >> 1;

 if (zlib_reset(&zlib, test_bits))
 {
            file_setpos(file, &start_pos);
            rc = zlib_run(&zlib);

 switch (rc)
 {
 case ZLIB_TOO_FAR_BACK:
                  min_bits = test_bits+1;
 if (min_bits > max_bits)
 {
  
                     assert(test_bits == 15);

  
 if (zlib.z.msg == NULL)
                        zlib.z.msg = PNGZ_MSG_CAST(
 "invalid distance too far back");
                     zlib_message(&zlib, 0 );
                     zlib_end(&zlib);
 return 0;
 }
 break;

 case ZLIB_STREAM_END:  
                  max_bits = test_bits;
 break;

 default:
  
                  zlib_end(&zlib);
 return 0;
 }
 }

 else  
 {
            zlib_end(&zlib);
 return 0;
 }
 }

  
      assert(zlib.ok_bits == max_bits);
      zlib_end(&zlib);
 return 1;
 }

 else  
 {
      zlib_end(&zlib);
 return 0;
 }
}
