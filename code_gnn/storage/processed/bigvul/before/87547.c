unsigned lodepng_get_bpp(const LodePNGColorMode* info)
{
   
  return lodepng_get_bpp_lct(info->colortype, info->bitdepth);
}
