static inline void mpeg4_encode_dc(PutBitContext *s, int level, int n)
{
     
    level += 256;
    if (n < 4) {
         
        put_bits(s, uni_DCtab_lum_len[level], uni_DCtab_lum_bits[level]);
    } else {
         
        put_bits(s, uni_DCtab_chrom_len[level], uni_DCtab_chrom_bits[level]);
    }
}
