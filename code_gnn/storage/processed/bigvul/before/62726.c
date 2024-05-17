MagickExport MagickBooleanType RemapImage(const QuantizeInfo *quantize_info,
  Image *image,const Image *remap_image)
{
  CubeInfo
    *cube_info;

  MagickBooleanType
    status;

   
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(remap_image != (Image *) NULL);
  assert(remap_image->signature == MagickSignature);
  cube_info=GetCubeInfo(quantize_info,MaxTreeDepth,
    quantize_info->number_colors);
  if (cube_info == (CubeInfo *) NULL)
    ThrowBinaryException(ResourceLimitError,"MemoryAllocationFailed",
      image->filename);
  status=ClassifyImageColors(cube_info,remap_image,&image->exception);
  if (status != MagickFalse)
    {
       
      cube_info->quantize_info->number_colors=cube_info->colors;
      status=AssignImageColors(image,cube_info);
    }
  DestroyCubeInfo(cube_info);
  return(status);
}