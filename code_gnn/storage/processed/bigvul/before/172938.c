png_read_filter_row_sub4_neon(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_bytep rp = row;
   png_bytep rp_stop = row + row_info->rowbytes;

 uint8x8x4_t vdest;
   vdest.val[3] = vdup_n_u8(0);

 for (; rp < rp_stop; rp += 16)
 {
 uint32x2x4_t vtmp = vld4_u32(png_ptr(uint32_t,rp));
 uint8x8x4_t *vrpt = png_ptr(uint8x8x4_t,&vtmp);
 uint8x8x4_t vrp = *vrpt;
 uint32x2x4_t *temp_pointer;

      vdest.val[0] = vadd_u8(vdest.val[3], vrp.val[0]);
      vdest.val[1] = vadd_u8(vdest.val[0], vrp.val[1]);
      vdest.val[2] = vadd_u8(vdest.val[1], vrp.val[2]);
      vdest.val[3] = vadd_u8(vdest.val[2], vrp.val[3]);
      vst4_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2x4_t,&vdest), 0);
 }

   PNG_UNUSED(prev_row)
}
