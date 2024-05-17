static void TIFFSetEXIFProperties(TIFF *tiff,Image *image,
  ExceptionInfo *exception)
{
#if defined(MAGICKCORE_HAVE_TIFFREADEXIFDIRECTORY)
  const char
    *value;

  register ssize_t
    i;

  uint32
    offset;

   
  offset=0;
  (void) TIFFSetField(tiff,TIFFTAG_SUBIFD,1,&offset);
  for (i=0; exif_info[i].tag != 0; i++)
  {
    value=GetImageProperty(image,exif_info[i].property,exception);
    if (value == (const char *) NULL)
      continue;
    switch (exif_info[i].type)
    {
      case TIFF_ASCII:
      {
        (void) TIFFSetField(tiff,exif_info[i].tag,value);
        break;
      }
      case TIFF_SHORT:
      {
        uint16
          field;

        field=(uint16) StringToLong(value);
        (void) TIFFSetField(tiff,exif_info[i].tag,field);
        break;
      }
      case TIFF_LONG:
      {
        uint16
          field;

        field=(uint16) StringToLong(value);
        (void) TIFFSetField(tiff,exif_info[i].tag,field);
        break;
      }
      case TIFF_RATIONAL:
      case TIFF_SRATIONAL:
      {
        float
          field;

        field=StringToDouble(value,(char **) NULL);
        (void) TIFFSetField(tiff,exif_info[i].tag,field);
        break;
      }
      default:
        break;
    }
  }
   
#else
  (void) tiff;
  (void) image;
#endif
}