png_read_filter_row_sub3_neon(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_bytep rp = row;
   png_bytep rp_stop = row + row_info->rowbytes;

 uint8x16_t vtmp = vld1q_u8(rp);
 uint8x8x2_t *vrpt = png_ptr(uint8x8x2_t, &vtmp);
 uint8x8x2_t vrp = *vrpt;

 uint8x8x4_t vdest;
   vdest.val[3] = vdup_n_u8(0);

 for (; rp < rp_stop;)
 {
 uint8x8_t vtmp1, vtmp2;
 uint32x2_t *temp_pointer;

      vtmp1 = vext_u8(vrp.val[0], vrp.val[1], 3);
      vdest.val[0] = vadd_u8(vdest.val[3], vrp.val[0]);
      vtmp2 = vext_u8(vrp.val[0], vrp.val[1], 6);
      vdest.val[1] = vadd_u8(vdest.val[0], vtmp1);

      vtmp1 = vext_u8(vrp.val[1], vrp.val[1], 1);
      vdest.val[2] = vadd_u8(vdest.val[1], vtmp2);
      vdest.val[3] = vadd_u8(vdest.val[2], vtmp1);

      vtmp = vld1q_u8(rp + 12);
      vrpt = png_ptr(uint8x8x2_t, &vtmp);
      vrp = *vrpt;

      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[0]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[1]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[2]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[3]), 0);
      rp += 3;
 }

   PNG_UNUSED(prev_row)
}
