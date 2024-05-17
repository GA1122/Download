WORD32 ih264d_get_next_display_field(dec_struct_t * ps_dec,
 ivd_out_bufdesc_t *ps_out_buffer,
 ivd_get_display_frame_op_t *pv_disp_op)
{
 pic_buffer_t *pic_buf;

    UWORD8 i1_cur_fld;
    WORD32 u4_api_ret = -1;
    WORD32 i4_disp_buf_id;
 iv_yuv_buf_t *ps_op_frm;



    ps_op_frm = &(ps_dec->s_disp_frame_info);
    H264_MUTEX_LOCK(&ps_dec->process_disp_mutex);
    pic_buf = (pic_buffer_t *)ih264_disp_mgr_get(
 (disp_mgr_t *)ps_dec->pv_disp_buf_mgr, &i4_disp_buf_id);
    ps_dec->u4_num_fld_in_frm = 0;
    u4_api_ret = -1;
    pv_disp_op->u4_ts = -1;
    pv_disp_op->e_output_format = ps_dec->u1_chroma_format;

    pv_disp_op->s_disp_frm_buf.pv_y_buf = ps_out_buffer->pu1_bufs[0];
    pv_disp_op->s_disp_frm_buf.pv_u_buf = ps_out_buffer->pu1_bufs[1];
    pv_disp_op->s_disp_frm_buf.pv_v_buf = ps_out_buffer->pu1_bufs[2];
 if(pic_buf != NULL)
 {
        pv_disp_op->e4_fld_type = 0;
        pv_disp_op->u4_disp_buf_id = i4_disp_buf_id;

        ps_op_frm->u4_y_ht = pic_buf->u2_disp_height << 1;
        ps_op_frm->u4_u_ht = ps_op_frm->u4_v_ht = ps_op_frm->u4_y_ht >> 1;
        ps_op_frm->u4_y_wd = pic_buf->u2_disp_width;

        ps_op_frm->u4_u_wd = ps_op_frm->u4_v_wd = ps_op_frm->u4_y_wd >> 1;

        ps_op_frm->u4_y_strd = pic_buf->u2_frm_wd_y;
        ps_op_frm->u4_u_strd = ps_op_frm->u4_v_strd = pic_buf->u2_frm_wd_uv;

  
        pv_disp_op->u4_ts = pic_buf->u4_ts;

  
        ps_op_frm->pv_y_buf = pic_buf->pu1_buf1 + pic_buf->u2_crop_offset_y;
        ps_op_frm->pv_u_buf = pic_buf->pu1_buf2 + pic_buf->u2_crop_offset_uv;
        ps_op_frm->pv_v_buf = pic_buf->pu1_buf3 + pic_buf->u2_crop_offset_uv;
        ps_dec->u4_num_fld_in_frm++;
        ps_dec->u4_num_fld_in_frm++;
        u4_api_ret = 0;

 if(pic_buf->u1_picturetype == 0)
            pv_disp_op->u4_progressive_frame_flag = 1;
 else
            pv_disp_op->u4_progressive_frame_flag = 0;

 } H264_MUTEX_UNLOCK(&ps_dec->process_disp_mutex);
    pv_disp_op->u4_error_code = u4_api_ret;
    pv_disp_op->e_pic_type = 0xFFFFFFFF;  

 if(u4_api_ret)
 {
        pv_disp_op->u4_error_code = 1;  
 }
 else
 {

        UWORD32 temp;
        UWORD32 dest_inc_Y = 0, dest_inc_UV = 0;

        pv_disp_op->s_disp_frm_buf.u4_y_wd = temp = MIN(ps_op_frm->u4_y_wd,
                                                        ps_op_frm->u4_y_strd);
        pv_disp_op->s_disp_frm_buf.u4_u_wd = pv_disp_op->s_disp_frm_buf.u4_y_wd
 >> 1;
        pv_disp_op->s_disp_frm_buf.u4_v_wd = pv_disp_op->s_disp_frm_buf.u4_y_wd
 >> 1;

        pv_disp_op->s_disp_frm_buf.u4_y_ht = ps_op_frm->u4_y_ht;
        pv_disp_op->s_disp_frm_buf.u4_u_ht = pv_disp_op->s_disp_frm_buf.u4_y_ht
 >> 1;
        pv_disp_op->s_disp_frm_buf.u4_v_ht = pv_disp_op->s_disp_frm_buf.u4_y_ht
 >> 1;
 if(0 == ps_dec->u4_share_disp_buf)
 {
            pv_disp_op->s_disp_frm_buf.u4_y_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_wd;
            pv_disp_op->s_disp_frm_buf.u4_u_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_wd >> 1;
            pv_disp_op->s_disp_frm_buf.u4_v_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_wd >> 1;

 }
 else
 {
            pv_disp_op->s_disp_frm_buf.u4_y_strd = ps_op_frm->u4_y_strd;
 }

 if(ps_dec->u4_app_disp_width)
 {
            pv_disp_op->s_disp_frm_buf.u4_y_strd = MAX(
                            ps_dec->u4_app_disp_width,
                            pv_disp_op->s_disp_frm_buf.u4_y_strd);
 }

        pv_disp_op->u4_error_code = 0;
 if(pv_disp_op->e_output_format == IV_YUV_420P)
 {
            UWORD32 i;
            pv_disp_op->s_disp_frm_buf.u4_u_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_strd >> 1;
            pv_disp_op->s_disp_frm_buf.u4_v_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_strd >> 1;

            pv_disp_op->s_disp_frm_buf.u4_u_wd = ps_op_frm->u4_y_wd >> 1;
            pv_disp_op->s_disp_frm_buf.u4_v_wd = ps_op_frm->u4_y_wd >> 1;

 if(1 == ps_dec->u4_share_disp_buf)
 {
                pv_disp_op->s_disp_frm_buf.pv_y_buf = ps_op_frm->pv_y_buf;

 for(i = 0; i < MAX_DISP_BUFS_NEW; i++)
 {
                    UWORD8 *buf = ps_dec->disp_bufs[i].buf[0];
                    buf += ps_dec->disp_bufs[i].u4_ofst[0];
 if(((UWORD8 *)pv_disp_op->s_disp_frm_buf.pv_y_buf
 - pic_buf->u2_crop_offset_y) == buf)
 {
                        buf = ps_dec->disp_bufs[i].buf[1];
                        buf += ps_dec->disp_bufs[i].u4_ofst[1];
                        pv_disp_op->s_disp_frm_buf.pv_u_buf = buf
 + pic_buf->u2_crop_offset_uv;

                        buf = ps_dec->disp_bufs[i].buf[2];
                        buf += ps_dec->disp_bufs[i].u4_ofst[2];
                        pv_disp_op->s_disp_frm_buf.pv_v_buf = buf
 + pic_buf->u2_crop_offset_uv;
 }
 }
 }

 }
 else if((pv_disp_op->e_output_format == IV_YUV_420SP_UV)
 || (pv_disp_op->e_output_format == IV_YUV_420SP_VU))
 {
            pv_disp_op->s_disp_frm_buf.u4_u_strd =
                            pv_disp_op->s_disp_frm_buf.u4_y_strd;
            pv_disp_op->s_disp_frm_buf.u4_v_strd = 0;

 if(1 == ps_dec->u4_share_disp_buf)
 {
                UWORD32 i;

                pv_disp_op->s_disp_frm_buf.pv_y_buf = ps_op_frm->pv_y_buf;

 for(i = 0; i < MAX_DISP_BUFS_NEW; i++)
 {
                    UWORD8 *buf = ps_dec->disp_bufs[i].buf[0];
                    buf += ps_dec->disp_bufs[i].u4_ofst[0];
 if((UWORD8 *)pv_disp_op->s_disp_frm_buf.pv_y_buf
 - pic_buf->u2_crop_offset_y == buf)
 {
                        buf = ps_dec->disp_bufs[i].buf[1];
                        buf += ps_dec->disp_bufs[i].u4_ofst[1];
                        pv_disp_op->s_disp_frm_buf.pv_u_buf = buf
 + pic_buf->u2_crop_offset_uv;
 ;

                        buf = ps_dec->disp_bufs[i].buf[2];
                        buf += ps_dec->disp_bufs[i].u4_ofst[2];
                        pv_disp_op->s_disp_frm_buf.pv_v_buf = buf
 + pic_buf->u2_crop_offset_uv;
 ;
 }
 }
 }
            pv_disp_op->s_disp_frm_buf.u4_u_wd =
                            pv_disp_op->s_disp_frm_buf.u4_y_wd;
            pv_disp_op->s_disp_frm_buf.u4_v_wd = 0;

 }
 else if((pv_disp_op->e_output_format == IV_RGB_565)
 || (pv_disp_op->e_output_format == IV_YUV_422ILE))
 {

            pv_disp_op->s_disp_frm_buf.u4_u_strd = 0;
            pv_disp_op->s_disp_frm_buf.u4_v_strd = 0;
            pv_disp_op->s_disp_frm_buf.u4_u_wd = 0;
            pv_disp_op->s_disp_frm_buf.u4_v_wd = 0;
            pv_disp_op->s_disp_frm_buf.u4_u_ht = 0;
            pv_disp_op->s_disp_frm_buf.u4_v_ht = 0;

 }


 }

 return u4_api_ret;
}
