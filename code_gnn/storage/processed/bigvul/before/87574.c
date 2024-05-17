unsigned lodepng_zlib_compress(unsigned char** out, size_t* outsize, const unsigned char* in,
                               size_t insize, const LodePNGCompressSettings* settings)
{
   
  ucvector outv;
  size_t i;
  unsigned error;
  unsigned char* deflatedata = 0;
  size_t deflatesize = 0;

  unsigned ADLER32;
   
  unsigned CMF = 120;  
  unsigned FLEVEL = 0;
  unsigned FDICT = 0;
  unsigned CMFFLG = 256 * CMF + FDICT * 32 + FLEVEL * 64;
  unsigned FCHECK = 31 - CMFFLG % 31;
  CMFFLG += FCHECK;

   
  ucvector_init_buffer(&outv, *out, *outsize);

  if (!ucvector_push_back(&outv, (unsigned char)(CMFFLG / 256))) return 83;
  if (!ucvector_push_back(&outv, (unsigned char)(CMFFLG % 256))) return 83;

  error = deflate(&deflatedata, &deflatesize, in, insize, settings);

  if(!error)
  {
    ADLER32 = adler32(in, (unsigned)insize);
    for(i = 0; i < deflatesize; i++)
    {
      if (!ucvector_push_back(&outv, deflatedata[i])) return 83;
    }
    free(deflatedata);
    error = !lodepng_add32bitInt(&outv, ADLER32);
  }

  if (!error)
  {
    *out = outv.data;
    *outsize = outv.size;
  }
  else
  {
    *out = NULL;
    *outsize = 0;
    ucvector_cleanup(&outv);
  }

  return error;
}
