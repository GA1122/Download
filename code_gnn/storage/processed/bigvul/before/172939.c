png_read_filter_row_up_neon(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_bytep rp = row;
   png_bytep rp_stop = row + row_info->rowbytes;
   png_const_bytep pp = prev_row;

 for (; rp < rp_stop; rp += 16, pp += 16)
 {
 uint8x16_t qrp, qpp;

      qrp = vld1q_u8(rp);
      qpp = vld1q_u8(pp);
      qrp = vaddq_u8(qrp, qpp);
      vst1q_u8(rp, qrp);
 }
}
