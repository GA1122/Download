static void convert_channels_short_interleaved(int buf_c, short *buffer, int data_c, float **data, int d_offset, int len)
{
   int i;
   check_endianness();
   if (buf_c != data_c && buf_c <= 2 && data_c <= 6) {
      assert(buf_c == 2);
      for (i=0; i < buf_c; ++i)
         compute_stereo_samples(buffer, data_c, data, d_offset, len);
   } else {
      int limit = buf_c < data_c ? buf_c : data_c;
      int j;
      for (j=0; j < len; ++j) {
         for (i=0; i < limit; ++i) {
            FASTDEF(temp);
            float f = data[i][d_offset+j];
            int v = FAST_SCALED_FLOAT_TO_INT(temp, f,15); 
            if ((unsigned int) (v + 32768) > 65535)
               v = v < 0 ? -32768 : 32767;
            *buffer++ = v;
         }
         for (   ; i < buf_c; ++i)
            *buffer++ = 0;
      }
   }
}