read_logical_screen_descriptor(Gif_Stream *gfs, Gif_Reader *grr)
      
{
  uint8_t packed;

   
  gfs->screen_width = gifgetunsigned(grr);
  gfs->screen_height = gifgetunsigned(grr);

  packed = gifgetbyte(grr);
  gfs->background = gifgetbyte(grr);

   
  gifgetbyte(grr);

  if (packed & 0x80) {  
      int ncol = 1 << ((packed & 0x07) + 1);
      gfs->global = read_color_table(ncol, grr);
      if (!gfs->global) return 0;
      gfs->global->refcount = 1;
  } else
      gfs->background = 256;

  return 1;
}
