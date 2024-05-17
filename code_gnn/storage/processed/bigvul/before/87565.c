unsigned lodepng_load_file(unsigned char** out, size_t* outsize, const char* filename)
{
  FILE* file;
  INT64 size;

   
  *out = 0;
  *outsize = 0;

  file = fopen(filename, "rb");
  if(!file) return 78;

   
  _fseeki64(file , 0 , SEEK_END);
  size = _ftelli64(file);
  rewind(file);

   
  *outsize = 0;
  *out = (unsigned char*)malloc((size_t)size);
  if(size && (*out)) (*outsize) = fread(*out, 1, (size_t)size, file);

  fclose(file);
  if (size < 0) return 91;
  if (*outsize != (size_t)size) return 91;
  if(!(*out) && size) return 83;  
  return 0;
}