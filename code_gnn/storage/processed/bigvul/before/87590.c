static unsigned readChunk_zTXt(LodePNGInfo* info, const LodePNGDecompressSettings* zlibsettings,
                               const unsigned char* data, size_t chunkLength)
{
  unsigned error = 0;
  unsigned i;

  unsigned length, string2_begin;
  char *key = 0;
  ucvector decoded;

  ucvector_init(&decoded);

  while(!error)  
  {
    for(length = 0; length < chunkLength && data[length] != 0; length++) ;
    if(length + 2 >= chunkLength) CERROR_BREAK(error, 75);  
    if(length < 1 || length > 79) CERROR_BREAK(error, 89);  

    key = (char*)malloc(length + 1);
    if(!key) CERROR_BREAK(error, 83);  

    key[length] = 0;
    for(i = 0; i < length; i++) key[i] = (char)data[i];

    if(data[length + 1] != 0) CERROR_BREAK(error, 72);  

    string2_begin = length + 2;
    if(string2_begin > chunkLength) CERROR_BREAK(error, 75);  

    length = chunkLength - string2_begin;
     
    error = zlib_decompress(&decoded.data, &decoded.size,
                            (unsigned char*)(&data[string2_begin]),
                            length, zlibsettings);
    if(error) break;
    if (!ucvector_push_back(&decoded, 0)) ERROR_BREAK(83);

    error = lodepng_add_text(info, key, (char*)decoded.data);

    break;
  }

  free(key);
  ucvector_cleanup(&decoded);

  return error;
}
