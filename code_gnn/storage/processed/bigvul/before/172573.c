void ih264d_deblock_picture(void *ptr)
{
 dec_struct_t *ps_dec = (dec_struct_t *)ptr;

 {
  
 if(ps_dec->u1_pic_decode_done == 1)
 {
 if(ps_dec->ps_cur_slice->u1_mbaff_frame_flag
 || ps_dec->ps_cur_slice->u1_field_pic_flag)
 {
                ps_dec->p_DeblockPicture[ps_dec->ps_cur_slice->u1_mbaff_frame_flag](
                                ps_dec);
 }
 else

 {

                ih264d_deblock_picture_progressive(ps_dec);
 }

 }
 }

}
