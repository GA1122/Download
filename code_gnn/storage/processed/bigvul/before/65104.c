static MagickBooleanType ReadDXT3(Image *image, DDSInfo *dds_info,
  ExceptionInfo *exception)
{
  DDSColors
    colors;

  ssize_t
    j,
    y;

  PixelPacket
    *q;

  register ssize_t
    i,
    x;

  unsigned char
    alpha;

  size_t
    a0,
    a1,
    bits,
    code;

  unsigned short
    c0,
    c1;

  for (y = 0; y < (ssize_t) dds_info->height; y += 4)
  {
    for (x = 0; x < (ssize_t) dds_info->width; x += 4)
    {
       
      q = QueueAuthenticPixels(image, x, y, MagickMin(4, dds_info->width - x),
                         MagickMin(4, dds_info->height - y),exception);

      if (q == (PixelPacket *) NULL)
        return MagickFalse;

       
      a0 = ReadBlobLSBLong(image);
      a1 = ReadBlobLSBLong(image);

       
      c0 = ReadBlobLSBShort(image);
      c1 = ReadBlobLSBShort(image);
      bits = ReadBlobLSBLong(image);

      CalculateColors(c0, c1, &colors, MagickTrue);

       
      for (j = 0; j < 4; j++)
      {
        for (i = 0; i < 4; i++)
        {
          if ((x + i) < (ssize_t) dds_info->width && (y + j) < (ssize_t) dds_info->height)
            {
              code = (bits >> ((4*j+i)*2)) & 0x3;
              SetPixelRed(q,ScaleCharToQuantum(colors.r[code]));
              SetPixelGreen(q,ScaleCharToQuantum(colors.g[code]));
              SetPixelBlue(q,ScaleCharToQuantum(colors.b[code]));
               
              if (j < 2)
                alpha = 17U * (unsigned char) ((a0 >> (4*(4*j+i))) & 0xf);
              else
                alpha = 17U * (unsigned char) ((a1 >> (4*(4*(j-2)+i))) & 0xf);
              SetPixelAlpha(q,ScaleCharToQuantum((unsigned char)
                alpha));
              q++;
            }
        }
      }

      if (SyncAuthenticPixels(image,exception) == MagickFalse)
        return MagickFalse;
    }
  }

  return(SkipDXTMipmaps(image,dds_info,16,exception));
}
