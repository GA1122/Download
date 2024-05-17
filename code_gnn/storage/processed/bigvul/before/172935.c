png_read_filter_row_paeth3_neon(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_bytep rp = row;
   png_const_bytep pp = prev_row;
   png_bytep rp_stop = row + row_info->rowbytes;

 uint8x16_t vtmp;
 uint8x8x2_t *vrpt;
 uint8x8x2_t vrp;
 uint8x8_t vlast = vdup_n_u8(0);
 uint8x8x4_t vdest;
   vdest.val[3] = vdup_n_u8(0);

   vtmp = vld1q_u8(rp);
   vrpt = png_ptr(uint8x8x2_t,&vtmp);
   vrp = *vrpt;

 for (; rp < rp_stop; pp += 12)
 {
 uint8x8x2_t *vppt;
 uint8x8x2_t vpp;
 uint8x8_t vtmp1, vtmp2, vtmp3;
 uint32x2_t *temp_pointer;

      vtmp = vld1q_u8(pp);
      vppt = png_ptr(uint8x8x2_t,&vtmp);
      vpp = *vppt;

      vdest.val[0] = paeth(vdest.val[3], vpp.val[0], vlast);
      vdest.val[0] = vadd_u8(vdest.val[0], vrp.val[0]);

      vtmp1 = vext_u8(vrp.val[0], vrp.val[1], 3);
      vtmp2 = vext_u8(vpp.val[0], vpp.val[1], 3);
      vdest.val[1] = paeth(vdest.val[0], vtmp2, vpp.val[0]);
      vdest.val[1] = vadd_u8(vdest.val[1], vtmp1);

      vtmp1 = vext_u8(vrp.val[0], vrp.val[1], 6);
      vtmp3 = vext_u8(vpp.val[0], vpp.val[1], 6);
      vdest.val[2] = paeth(vdest.val[1], vtmp3, vtmp2);
      vdest.val[2] = vadd_u8(vdest.val[2], vtmp1);

      vtmp1 = vext_u8(vrp.val[1], vrp.val[1], 1);
      vtmp2 = vext_u8(vpp.val[1], vpp.val[1], 1);

      vtmp = vld1q_u8(rp + 12);
      vrpt = png_ptr(uint8x8x2_t,&vtmp);
      vrp = *vrpt;

      vdest.val[3] = paeth(vdest.val[2], vtmp2, vtmp3);
      vdest.val[3] = vadd_u8(vdest.val[3], vtmp1);

      vlast = vtmp2;

      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[0]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[1]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[2]), 0);
      rp += 3;
      vst1_lane_u32(png_ptr(uint32_t,rp), png_ldr(uint32x2_t,&vdest.val[3]), 0);
      rp += 3;
 }
}
