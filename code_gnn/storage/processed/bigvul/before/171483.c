void ih264d_fill_output_struct_from_context(dec_struct_t *ps_dec,
 ivd_video_decode_op_t *ps_dec_op)
{
 if((ps_dec_op->u4_error_code & 0xff)
 != ERROR_DYNAMIC_RESOLUTION_NOT_SUPPORTED)
 {
        ps_dec_op->u4_pic_wd = (UWORD32)ps_dec->u2_disp_width;
        ps_dec_op->u4_pic_ht = (UWORD32)ps_dec->u2_disp_height;
 }
    ps_dec_op->e_pic_type = ps_dec->i4_frametype;

    ps_dec_op->u4_new_seq = 0;
    ps_dec_op->u4_output_present = ps_dec->u4_output_present;
    ps_dec_op->u4_progressive_frame_flag =
                    ps_dec->s_disp_op.u4_progressive_frame_flag;

    ps_dec_op->u4_is_ref_flag = 1;
 if(ps_dec_op->u4_frame_decoded_flag)
 {
 if(ps_dec->ps_cur_slice->u1_nal_ref_idc == 0)
            ps_dec_op->u4_is_ref_flag = 0;
 }

    ps_dec_op->e_output_format = ps_dec->s_disp_op.e_output_format;
    ps_dec_op->s_disp_frm_buf = ps_dec->s_disp_op.s_disp_frm_buf;
    ps_dec_op->e4_fld_type = ps_dec->s_disp_op.e4_fld_type;
    ps_dec_op->u4_ts = ps_dec->s_disp_op.u4_ts;
    ps_dec_op->u4_disp_buf_id = ps_dec->s_disp_op.u4_disp_buf_id;
}
