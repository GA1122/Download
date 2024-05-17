flx_colorspace_converter_new (gint width, gint height)
{
  FlxColorSpaceConverter *new = g_malloc (sizeof (FlxColorSpaceConverter));

  new->width = width;
  new->height = height;

  memset (new->palvec, 0, sizeof (new->palvec));
  return new;
}
