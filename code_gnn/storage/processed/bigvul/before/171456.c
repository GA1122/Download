static int parse_bitdepth_colorspace_sampling(
    BITSTREAM_PROFILE profile, struct vpx_read_bit_buffer *rb) {
 vpx_color_space_t color_space;
 if (profile >= PROFILE_2)
    rb->bit_offset += 1;  
  color_space = (vpx_color_space_t)vpx_rb_read_literal(rb, 3);
 if (color_space != VPX_CS_SRGB) {
    rb->bit_offset += 1;  
 if (profile == PROFILE_1 || profile == PROFILE_3) {
      rb->bit_offset += 2;  
      rb->bit_offset += 1;  
 }
 } else {
 if (profile == PROFILE_1 || profile == PROFILE_3) {
      rb->bit_offset += 1;  
 } else {
 return 0;
 }
 }
 return 1;
}
