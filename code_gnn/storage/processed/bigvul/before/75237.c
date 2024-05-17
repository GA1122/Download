static int codebook_decode_scalar(vorb *f, Codebook *c)
{
   int i;
   if (f->valid_bits < STB_VORBIS_FAST_HUFFMAN_LENGTH)
      prep_huffman(f);
   i = f->acc & FAST_HUFFMAN_TABLE_MASK;
   i = c->fast_huffman[i];
   if (i >= 0) {
      f->acc >>= c->codeword_lengths[i];
      f->valid_bits -= c->codeword_lengths[i];
      if (f->valid_bits < 0) { f->valid_bits = 0; return -1; }
      return i;
   }
   return codebook_decode_scalar_raw(f,c);
}