static MagickBooleanType IsMVG(const unsigned char *magick,const size_t length)
{
  if (length < 20)
    return(MagickFalse);
  if (LocaleNCompare((const char *) magick,"push graphic-context",20) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
