process_chunk(struct file *file, png_uint_32 file_crc, png_uint_32 next_length,
   png_uint_32 next_type)
  
{
 const png_uint_32 type = file->type;

 if (file->global->verbose > 1)
 {
      fputs("  ", stderr);
      type_name(file->type, stderr);
      fprintf(stderr, " %lu 0x%.8x 0x%.8x\n", (unsigned long)file->length,
         file->crc ^ 0xffffffff, file_crc);
 }

  
 if ((file->crc ^ 0xffffffff) != file_crc)
 {
  
      file->status_code |= CRC_ERROR;

  
 if (file->global->skip != SKIP_BAD_CRC)
         type_message(file, type, "bad CRC");

  
 else if (CRITICAL(type))
         stop(file, READ_ERROR_CODE, "bad CRC in critical chunk");

 else
 {
         type_message(file, type, "skipped: bad CRC");

  
 goto skip_chunk;
 }
 }

  
 if (skip_chunk_type(file->global, type))
 goto skip_chunk;

  
 if (type != png_IDAT)
      file->alloc(file, 0 );

 else if (file->idat == NULL)
      file->alloc(file, 1 );

 else
 {
  
      assert(file->chunk != NULL);
      assert(file->chunk->chunk_type == png_IDAT);
      file->chunk->chunk_length = file->length;
 }

  
   file->length = next_length;
   file->type = next_type;
   getpos(file);

  
   file->chunk->rewrite_length = 0;
   file->chunk->rewrite_offset = 0;
 switch (type)
 {
 default:
 return;

 case png_IHDR:
  
 {
 struct chunk *chunk = file->chunk;

 if (chunk->chunk_length != 13)
               stop_invalid(file, "IHDR length");

  
            setpos(chunk);
            file->width = reread_4(file);
            file->height = reread_4(file);
            file->bit_depth = reread_byte(file);
            file->color_type = reread_byte(file);
            file->compression_method = reread_byte(file);
            file->filter_method = reread_byte(file);
            file->interlace_method = reread_byte(file);

  
            calc_image_size(file);
 }
 return;

  
 case png_zTXt: case png_iCCP:
 if (process_zTXt_iCCP(file))
 return;
         chunk_end(&file->chunk);
         file_setpos(file, &file->data_pos);
 break;

 case png_iTXt:
 if (process_iTXt(file))
 return;
         chunk_end(&file->chunk);
         file_setpos(file, &file->data_pos);
 break;

 case png_IDAT:
 if (process_IDAT(file))
 return;
  
         assert(next_type == png_IDAT);
 break;
 }

  
   read_chunk(file);
 return;

  
skip_chunk:
   file->length = next_length;
   file->type = next_type;
   getpos(file);
   read_chunk(file);
}
