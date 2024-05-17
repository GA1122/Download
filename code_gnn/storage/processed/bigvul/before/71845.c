int magick_progress_callback(void *context,float quantum)
{
  Image
    *image;

  MagickBooleanType
    status;

  image=(Image *) context;
  assert(image->signature == MagickSignature);
  status=SetImageProgress(image,LoadImagesTag,TellBlob(image),
    GetBlobSize(image));
  return(status != MagickFalse ? 0 : 1);
}
