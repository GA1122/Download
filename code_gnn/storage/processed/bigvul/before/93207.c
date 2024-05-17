GLOBAL(cjpeg_source_ptr)
jinit_read_ppm(j_compress_ptr cinfo)
{
  ppm_source_ptr source;

   
  source = (ppm_source_ptr)
    (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_IMAGE,
                                sizeof(ppm_source_struct));
   
  source->pub.start_input = start_input_ppm;
  source->pub.finish_input = finish_input_ppm;

  return (cjpeg_source_ptr)source;
}
