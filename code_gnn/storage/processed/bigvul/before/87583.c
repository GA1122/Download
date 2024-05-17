static unsigned readChunk_PLTE(LodePNGColorMode* color, const unsigned char* data, size_t chunkLength)
{
  unsigned pos = 0, i;
  free(color->palette);
  color->palettesize = chunkLength / 3;
  color->palette = (unsigned char*)malloc(4 * color->palettesize);
  if(!color->palette && color->palettesize)
  {
    color->palettesize = 0;
    return 83;  
  }
  if(color->palettesize > 256) return 38;  

  for(i = 0; i < color->palettesize; i++)
  {
    color->palette[4 * i + 0] = data[pos++];  
    color->palette[4 * i + 1] = data[pos++];  
    color->palette[4 * i + 2] = data[pos++];  
    color->palette[4 * i + 3] = 255;  
  }

  return 0;  
}