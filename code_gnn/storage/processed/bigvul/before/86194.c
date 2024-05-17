read_image_data(Gif_Context *gfc, Gif_Reader *grr)
{
   
  uint8_t buffer[GIF_MAX_BLOCK + 5];
  int i;
  uint32_t accum;

  int bit_position;
  int bit_length;

  Gif_Code code;
  Gif_Code old_code;
  Gif_Code clear_code;
  Gif_Code eoi_code;
  Gif_Code next_code;
#define CUR_BUMP_CODE (1 << bits_needed)
#define CUR_CODE_MASK ((1 << bits_needed) - 1)

  int min_code_size;
  int bits_needed;

  gfc->decodepos = 0;

  min_code_size = gifgetbyte(grr);
  GIF_DEBUG(("\n\nmin_code_size(%d) ", min_code_size));
  if (min_code_size >= GIF_MAX_CODE_BITS) {
    gif_read_error(gfc, 1, "image corrupted, min_code_size too big");
    min_code_size = GIF_MAX_CODE_BITS - 1;
  } else if (min_code_size < 2) {
    gif_read_error(gfc, 1, "image corrupted, min_code_size too small");
    min_code_size = 2;
  }
  clear_code = 1 << min_code_size;
  for (code = 0; code < clear_code; code++) {
    gfc->prefix[code] = 49428;
    gfc->suffix[code] = (uint8_t)code;
    gfc->length[code] = 1;
  }
  eoi_code = clear_code + 1;

  next_code = eoi_code;
  bits_needed = min_code_size + 1;

  code = clear_code;

  bit_length = bit_position = 0;
   

  while (1) {

    old_code = code;

     

    if (bit_position + bits_needed > bit_length)
       
      if (!read_image_block(grr, buffer, &bit_position, &bit_length,
			    bits_needed))
	goto zero_length_block;

    i = bit_position / 8;
    accum = buffer[i] + (buffer[i+1] << 8);
    if (bits_needed >= 8)
      accum |= (buffer[i+2]) << 16;
    code = (Gif_Code)((accum >> (bit_position % 8)) & CUR_CODE_MASK);
    bit_position += bits_needed;

    GIF_DEBUG(("%d ", code));

     
    if (code == clear_code) {
      GIF_DEBUG(("clear "));
      bits_needed = min_code_size + 1;
      next_code = eoi_code;
      continue;

    } else if (code == eoi_code)
      break;

    else if (code > next_code && next_code && next_code != clear_code) {
       
      if (gfc->errors[1] < 20)
          gif_read_error(gfc, 1, "image corrupted, code out of range");
      else if (gfc->errors[1] == 20)
          gif_read_error(gfc, 1, "(not reporting more errors)");
      code = 0;
    }

     

     
    gfc->prefix[next_code] = old_code;
    gfc->length[next_code] = gfc->length[old_code] + 1;

     
    gfc->suffix[next_code] = one_code(gfc, code);

     
     
    if (code == next_code && gfc->image + gfc->decodepos <= gfc->maximage)
      gfc->image[gfc->decodepos - 1] = gfc->suffix[next_code];

     
    if (next_code != clear_code) {
      next_code++;
      if (next_code == CUR_BUMP_CODE) {
	if (bits_needed < GIF_MAX_CODE_BITS)
	  bits_needed++;
	else
	  next_code = clear_code;
      }
    }

  }

   
  i = gifgetbyte(grr);
  GIF_DEBUG(("\nafter_image(%d)\n", i));
  while (i > 0) {
    gifgetblock(buffer, i, grr);
    i = gifgetbyte(grr);
    GIF_DEBUG(("\nafter_image(%d)\n", i));
  }

   
 zero_length_block: {
      long delta = (long) (gfc->maximage - gfc->image) - (long) gfc->decodepos;
      char buf[BUFSIZ];
      if (delta > 0) {
          sprintf(buf, "missing %ld %s of image data", delta,
                  delta == 1 ? "pixel" : "pixels");
          gif_read_error(gfc, 1, buf);
          memset(&gfc->image[gfc->decodepos], 0, delta);
      } else if (delta < -1) {
           
          sprintf(buf, "%ld superfluous pixels of image data", -delta);
          gif_read_error(gfc, 0, buf);
      }
  }
}
