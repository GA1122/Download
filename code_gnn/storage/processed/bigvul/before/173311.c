read_callback(png_structp png_ptr, png_bytep buffer, size_t count)
  
{
 struct file *file = get_control(png_ptr);
   png_uint_32 type, length;  
 struct chunk *chunk;

  
 if (count == 0)
      stop(file, UNEXPECTED_ERROR_CODE, "read callback for 0 bytes");

  
 if (file->read_count < 8)
 {
      assert(file->read_count == 0);
      assert((file->status_code & TRUNCATED) == 0);

 (void)read_4(file, &file->length);

 if (file->read_count == 4)
 (void)read_4(file, &file->type);

 if (file->read_count < 8)
 {
         assert((file->status_code & TRUNCATED) != 0);
         stop(file, READ_ERROR_CODE, "not a PNG (too short)");
 }

 if (file->state == STATE_SIGNATURE)
 {
 if (file->length != sig1 || file->type != sig2)
            stop(file, LIBPNG_ERROR_CODE, "not a PNG (signature)");

  
         file->write_count = 0;
 }

 else
 {
         assert(file->state == STATE_CHUNKS);

  
 if (file->length != 13 || file->type != png_IHDR)
            stop(file, LIBPNG_ERROR_CODE, "not a PNG (IHDR)");

  
         getpos(file);
 }
 }

  
   chunk = file->chunk;

 if (chunk != NULL)
 {
      length = chunk->chunk_length;
      type = chunk->chunk_type;
 }

 else
 {
  
      length = file->length;
      type = file->type;
 }

 do
 {
      png_uint_32 b;

  
 if (file->state != STATE_SIGNATURE && chunk == NULL)
 {
         assert(file->read_count == 8);
         assert(file->idat == NULL);
         read_chunk(file);
         chunk = file->chunk;
         assert(chunk != NULL);

  
         length = chunk->chunk_length;
         type = chunk->chunk_type;

  
         file->write_count = 0;
 }

  
 switch (file->write_count)
 {
 case 0: b = length >> 24; break;
 case 1: b = length >> 16; break;
 case 2: b = length >> 8; break;
 case 3: b = length      ; break;

 case 4: b = type >> 24; break;
 case 5: b = type >> 16; break;
 case 6: b = type >> 8; break;
 case 7: b = type      ; break;

 case 8:
  
 if (file->state == STATE_SIGNATURE)
 {
  
               assert(length == sig1 && type == sig2);
               file->read_count = 0;  
               file->state = STATE_CHUNKS;  
               read_callback(png_ptr, buffer, count);
 return;
 }

 else
 {
               assert(chunk != NULL);

  
               chunk->write_crc = crc_init_4(type);
 if (file->state != STATE_IDAT && length > 0)
                  setpos(chunk);
 }
  

 default:
            assert(chunk != NULL);

  
 switch (file->write_count - length)
 {
  
 default:
 if (file->state == STATE_IDAT)
 {
 struct IDAT *idat = file->idat;

                     assert(idat != NULL);

  
 while (idat->idat_index >= idat->idat_length)
 {
  
 struct IDAT_list *cur = idat->idat_cur;

                        assert(idat->idat_index == idat->idat_length);
                        assert(cur != NULL && cur->count > 0);

  
 if (++(idat->idat_count) >= cur->count)
 {
                           assert(idat->idat_count == cur->count);

  
                           cur = cur->next;

  
 if (cur == NULL || cur->count == 0)
                              stop(file, UNEXPECTED_ERROR_CODE,
 "read beyond end of IDAT");

                           idat->idat_count = 0;
                           idat->idat_cur = cur;
 }

                        idat->idat_index = 0;
  
                        idat->idat_length = cur->lengths[idat->idat_count];

  
                        skip_12(file);
 }

  
 ++(idat->idat_index);
 }

  
                  b = reread_byte(file);

  
 if (chunk->rewrite_length > 0)
 {
 if (chunk->rewrite_offset > 0)
 --(chunk->rewrite_offset);

 else
 {
                        b = chunk->rewrite_buffer[0];
                        memmove(chunk->rewrite_buffer, chunk->rewrite_buffer+1,
 (sizeof chunk->rewrite_buffer)-
 (sizeof chunk->rewrite_buffer[0]));

 --(chunk->rewrite_length);
 }
 }

                  chunk->write_crc = crc_one_byte(chunk->write_crc, b);
 break;

  
 case 8: b = chunk->write_crc >> 24; goto write_crc;
 case 9: b = chunk->write_crc >> 16; goto write_crc;
 case 10: b = chunk->write_crc >> 8; goto write_crc;
 case 11:
  
                  b = chunk->write_crc;

 if (file->global->verbose > 2)
 {
                     fputs("   ", stderr);
                     type_name(type, stderr);
                     fprintf(stderr, " %lu 0x%.8x\n", (unsigned long)length,
                        chunk->write_crc ^ 0xffffffff);
 }

  
 if (file->state == STATE_IDAT &&
 (file->idat->idat_index < file->idat->idat_length ||
 1+file->idat->idat_count < file->idat->idat_cur->count ||
                      file->idat->idat_cur != file->idat->idat_list_tail))
 {
  
                     length = chunk->chunk_length = rechunk_length(file->idat);
                     assert(type == png_IDAT);
                     file->write_count = 0;  
 --(file->write_count);  
 }

 else
 {
  
 if (chunk->rewrite_length > 0 || chunk->rewrite_offset > 0)
                        stop(file, UNEXPECTED_ERROR_CODE, "pending rewrite");

  
                     file->read_count = 8;
                     file_setpos(file, &file->data_pos);

 if (file->idat == NULL)
                        chunk_end(&file->chunk);

 else
                        IDAT_end(&file->idat);
 }

               write_crc:
                  b ^= 0xff;  
 break;
 }
 break;
 }

  
      b &= 0xff;
 *buffer++ = (png_byte)b;
 --count;
      write_byte(file, (png_byte)b);  
 }
 while (count > 0);
}
