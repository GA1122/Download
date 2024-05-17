static int codebook_decode_scalar_raw(vorb *f, Codebook *c)
{
   int i;
   prep_huffman(f);

   if (c->codewords == NULL && c->sorted_codewords == NULL)
      return -1;

   if (c->entries > 8 ? c->sorted_codewords!=NULL : !c->codewords) {
      uint32 code = bit_reverse(f->acc);
      int x=0, n=c->sorted_entries, len;

      while (n > 1) {
         int m = x + (n >> 1);
         if (c->sorted_codewords[m] <= code) {
            x = m;
            n -= (n>>1);
         } else {
            n >>= 1;
         }
      }
      if (!c->sparse) x = c->sorted_values[x];
      len = c->codeword_lengths[x];
      if (f->valid_bits >= len) {
         f->acc >>= len;
         f->valid_bits -= len;
         return x;
      }

      f->valid_bits = 0;
      return -1;
   }

   assert(!c->sparse);
   for (i=0; i < c->entries; ++i) {
      if (c->codeword_lengths[i] == NO_CODE) continue;
      if (c->codewords[i] == (f->acc & ((1 << c->codeword_lengths[i])-1))) {
         if (f->valid_bits >= c->codeword_lengths[i]) {
            f->acc >>= c->codeword_lengths[i];
            f->valid_bits -= c->codeword_lengths[i];
            return i;
         }
         f->valid_bits = 0;
         return -1;
      }
   }

   error(f, VORBIS_invalid_stream);
   f->valid_bits = 0;
   return -1;
}