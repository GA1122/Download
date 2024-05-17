unsigned lodepng_zlib_decompress(unsigned char** out, size_t* outsize, const unsigned char* in,
                                 size_t insize, const LodePNGDecompressSettings* settings)
{
  unsigned error = 0;
  unsigned CM, CINFO, FDICT;

  if(insize < 2) return 53;  
   
  if((in[0] * 256 + in[1]) % 31 != 0)
  {
     
    return 24;
  }

  CM = in[0] & 15;
  CINFO = (in[0] >> 4) & 15;
     
  FDICT = (in[1] >> 5) & 1;
     

  if(CM != 8 || CINFO > 7)
  {
     
    return 25;
  }
  if(FDICT != 0)
  {
     
    return 26;
  }

  error = inflate(out, outsize, in + 2, insize - 2, settings);
  if(error) return error;

  if(!settings->ignore_adler32)
  {
    unsigned ADLER32 = lodepng_read32bitInt(&in[insize - 4]);
    unsigned checksum = adler32(*out, (unsigned)(*outsize));
    if(checksum != ADLER32) return 58;  
  }

  return 0;  
}
