static unsigned inflateNoCompression(ucvector* out, const unsigned char* in, size_t* bp, size_t* pos, size_t inlength)
{
   
  size_t p;
  unsigned LEN, NLEN, n, error = 0;
  while(((*bp) & 0x7) != 0) (*bp)++;
  p = (*bp) / 8;  

   
  if(p >= inlength - 4) return 52;  
  LEN = in[p] + 256u * in[p + 1]; p += 2;
  NLEN = in[p] + 256u * in[p + 1]; p += 2;

   
  if(LEN + NLEN != 65535) return 21;  

  if(!ucvector_resize(out, (*pos) + LEN)) return 83;  

   
  if(p + LEN > inlength) return 23;  
  for(n = 0; n < LEN; n++) out->data[(*pos)++] = in[p++];

  (*bp) = p * 8;

  return error;
}
