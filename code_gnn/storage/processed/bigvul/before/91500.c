void gdImageGif (gdImagePtr im, FILE * outFile)
{
  gdIOCtx *out = gdNewFileCtx (outFile);
  gdImageGifCtx (im, out);
  out->gd_free (out);
 }