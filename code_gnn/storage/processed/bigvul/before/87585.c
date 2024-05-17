static unsigned readChunk_iTXt(LodePNGInfo* info, const LodePNGDecompressSettings* zlibsettings,
                               const unsigned char* data, size_t chunkLength)
{
  unsigned error = 0;
  unsigned i;

  unsigned length, begin, compressed;
  char *key = 0, *langtag = 0, *transkey = 0;
  ucvector decoded;
  ucvector_init(&decoded);

  while(!error)  
  {
     
    if(chunkLength < 5) CERROR_BREAK(error, 30);  

     
    for(length = 0; length < chunkLength && data[length] != 0; length++) ;
    if(length + 3 >= chunkLength) CERROR_BREAK(error, 75);  
    if(length < 1 || length > 79) CERROR_BREAK(error, 89);  

    key = (char*)malloc(length + 1);
    if(!key) CERROR_BREAK(error, 83);  

    key[length] = 0;
    for(i = 0; i < length; i++) key[i] = (char)data[i];

     
    compressed = data[length + 1];
    if(data[length + 2] != 0) CERROR_BREAK(error, 72);  

     

     
    begin = length + 3;
    length = 0;
    for(i = begin; i < chunkLength && data[i] != 0; i++) length++;

    langtag = (char*)malloc(length + 1);
    if(!langtag) CERROR_BREAK(error, 83);  

    langtag[length] = 0;
    for(i = 0; i < length; i++) langtag[i] = (char)data[begin + i];

     
    begin += length + 1;
    length = 0;
    for(i = begin; i < chunkLength && data[i] != 0; i++) length++;

    transkey = (char*)malloc(length + 1);
    if(!transkey) CERROR_BREAK(error, 83);  

    transkey[length] = 0;
    for(i = 0; i < length; i++) transkey[i] = (char)data[begin + i];

     
    begin += length + 1;

    length = chunkLength < begin ? 0 : chunkLength - begin;

    if(compressed)
    {
       
      error = zlib_decompress(&decoded.data, &decoded.size,
                              (unsigned char*)(&data[begin]),
                              length, zlibsettings);
      if(error) break;
      if(decoded.allocsize < decoded.size) decoded.allocsize = decoded.size;
      if (!ucvector_push_back(&decoded, 0)) CERROR_BREAK(error, 83  );
    }
    else
    {
      if(!ucvector_resize(&decoded, length + 1)) CERROR_BREAK(error, 83  );

      decoded.data[length] = 0;
      for(i = 0; i < length; i++) decoded.data[i] = data[begin + i];
    }

    error = lodepng_add_itext(info, key, langtag, transkey, (char*)decoded.data);

    break;
  }

  free(key);
  free(langtag);
  free(transkey);
  ucvector_cleanup(&decoded);

  return error;
}
