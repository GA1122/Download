void inverse_mdct_slow(float *buffer, int n, vorb *f, int blocktype)
{
   int i, n4 = n >> 2, n2 = n >> 1, n3_4 = n - n4;
   float temp[4096];

   memcpy(temp, buffer, n2 * sizeof(float));
   dct_iv_slow(temp, n2);   

   for (i=0; i < n4  ; ++i) buffer[i] = temp[i+n4];             
   for (   ; i < n3_4; ++i) buffer[i] = -temp[n3_4 - i - 1];    
   for (   ; i < n   ; ++i) buffer[i] = -temp[i - n3_4];        
}
