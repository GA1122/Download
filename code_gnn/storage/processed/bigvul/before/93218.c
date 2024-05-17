METHODDEF(JDIMENSION)
get_raw_row(j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
 
{
  ppm_source_ptr source = (ppm_source_ptr)sinfo;

  if (!ReadOK(source->pub.input_file, source->iobuffer, source->buffer_width))
    ERREXIT(cinfo, JERR_INPUT_EOF);
  return 1;
}
