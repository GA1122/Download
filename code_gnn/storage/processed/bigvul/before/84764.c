METHODDEF(JDIMENSION)
get_24bit_row(j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
 
{
  bmp_source_ptr source = (bmp_source_ptr)sinfo;
  JSAMPARRAY image_ptr;
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;

  if (source->use_inversion_array) {
     
    source->source_row--;
    image_ptr = (*cinfo->mem->access_virt_sarray)
      ((j_common_ptr)cinfo, source->whole_image,
       source->source_row, (JDIMENSION)1, FALSE);
    inptr = image_ptr[0];
  } else {
    if (!ReadOK(source->pub.input_file, source->iobuffer, source->row_width))
      ERREXIT(cinfo, JERR_INPUT_EOF);
    inptr = source->iobuffer;
  }

   
  outptr = source->pub.buffer[0];
  if (cinfo->in_color_space == JCS_EXT_BGR) {
    MEMCOPY(outptr, inptr, source->row_width);
  } else if (cinfo->in_color_space == JCS_CMYK) {
    for (col = cinfo->image_width; col > 0; col--) {
       
      JSAMPLE b = *inptr++, g = *inptr++, r = *inptr++;
      rgb_to_cmyk(r, g, b, outptr, outptr + 1, outptr + 2, outptr + 3);
      outptr += 4;
    }
  } else {
    register int rindex = rgb_red[cinfo->in_color_space];
    register int gindex = rgb_green[cinfo->in_color_space];
    register int bindex = rgb_blue[cinfo->in_color_space];
    register int aindex = alpha_index[cinfo->in_color_space];
    register int ps = rgb_pixelsize[cinfo->in_color_space];

    if (aindex >= 0) {
      for (col = cinfo->image_width; col > 0; col--) {
        outptr[bindex] = *inptr++;       
        outptr[gindex] = *inptr++;
        outptr[rindex] = *inptr++;
        outptr[aindex] = 0xFF;
        outptr += ps;
      }
    } else {
      for (col = cinfo->image_width; col > 0; col--) {
        outptr[bindex] = *inptr++;       
        outptr[gindex] = *inptr++;
        outptr[rindex] = *inptr++;
        outptr += ps;
      }
    }
  }

  return 1;
}