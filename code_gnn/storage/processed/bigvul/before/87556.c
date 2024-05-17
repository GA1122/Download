static unsigned lodepng_inflatev(ucvector* out,
                                 const unsigned char* in, size_t insize,
                                 const LodePNGDecompressSettings* settings)
{
   
  size_t bp = 0;
  unsigned BFINAL = 0;
  size_t pos = 0;  
  unsigned error = 0;

  (void)settings;

  while(!BFINAL)
  {
    unsigned BTYPE;
    if(bp + 2 >= insize * 8) return 52;  
    BFINAL = readBitFromStream(&bp, in);
    BTYPE = 1u * readBitFromStream(&bp, in);
    BTYPE += 2u * readBitFromStream(&bp, in);

    if(BTYPE == 3) return 20;  
    else if(BTYPE == 0) error = inflateNoCompression(out, in, &bp, &pos, insize);  
    else error = inflateHuffmanBlock(out, in, &bp, &pos, insize, BTYPE);  

    if(error) return error;
  }

  return error;
}
