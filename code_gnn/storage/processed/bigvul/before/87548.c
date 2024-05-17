static unsigned lodepng_get_bpp_lct(LodePNGColorType colortype, unsigned bitdepth)
{
   
  return getNumColorChannels(colortype) * bitdepth;
}
