flx_colorspace_convert (FlxColorSpaceConverter * flxpal, guchar * src,
    guchar * dest)
{
  guint size, col;

  g_return_if_fail (flxpal != NULL);
  g_return_if_fail (src != dest);


  size = flxpal->width * flxpal->height;

  while (size--) {
    col = (*src++ * 3);

#if G_BYTE_ORDER == G_BIG_ENDIAN
    *dest++ = 0;
    *dest++ = flxpal->palvec[col];
    *dest++ = flxpal->palvec[col + 1];
    *dest++ = flxpal->palvec[col + 2];
#else
    *dest++ = flxpal->palvec[col + 2];
    *dest++ = flxpal->palvec[col + 1];
    *dest++ = flxpal->palvec[col];
    *dest++ = 0;
#endif
  }

}
