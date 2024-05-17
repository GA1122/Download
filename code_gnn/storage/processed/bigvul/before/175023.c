IMPEG2D_ERROR_CODES_T impeg2d_pre_pic_dec_proc(dec_state_t *ps_dec)
{
    WORD32 u4_get_disp;
 pic_buf_t *ps_disp_pic;
    IMPEG2D_ERROR_CODES_T e_error = (IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE;

    u4_get_disp = 0;
    ps_disp_pic = NULL;

  
 if(ps_dec->u2_picture_structure != FRAME_PICTURE)
 {
        ps_dec->u2_num_vert_mb       = (ps_dec->u2_vertical_size + 31) >> 5;

 if(ps_dec->u2_num_flds_decoded == 0)
 {
 pic_buf_t *ps_pic_buf;
            u4_get_disp = 1;

            ps_pic_buf = impeg2_buf_mgr_get_next_free(ps_dec->pv_pic_buf_mg, &ps_dec->i4_cur_buf_id);

 if (NULL == ps_pic_buf)
 {
 return IMPEG2D_NO_FREE_BUF_ERR;
 }

            impeg2_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mg, ps_dec->i4_cur_buf_id, BUF_MGR_DISP);
            impeg2_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mg, ps_dec->i4_cur_buf_id, BUF_MGR_REF);

            ps_pic_buf->u4_ts = ps_dec->u4_inp_ts;
            ps_pic_buf->e_pic_type = ps_dec->e_pic_type;
            ps_dec->ps_cur_pic = ps_pic_buf;
            ps_dec->s_cur_frm_buf.pu1_y = ps_pic_buf->pu1_y;
            ps_dec->s_cur_frm_buf.pu1_u = ps_pic_buf->pu1_u;
            ps_dec->s_cur_frm_buf.pu1_v = ps_pic_buf->pu1_v;
 }

 if(ps_dec->u2_picture_structure == TOP_FIELD)
 {
            ps_dec->u2_fld_parity = TOP;
 }
 else
 {
            ps_dec->u2_fld_parity = BOTTOM;
 }
        ps_dec->u2_field_dct           = 0;
        ps_dec->u2_read_dct_type        = 0;
        ps_dec->u2_read_motion_type     = 1;
        ps_dec->u2_fld_pic             = 1;
        ps_dec->u2_frm_pic             = 0;
        ps_dec->ps_func_forw_or_back     = gas_impeg2d_func_fld_fw_or_bk;
        ps_dec->ps_func_bi_direct       = gas_impeg2d_func_fld_bi_direct;
 }
  
 else
 {
 pic_buf_t *ps_pic_buf;


        ps_dec->u2_num_vert_mb       = (ps_dec->u2_vertical_size + 15) >> 4;
        u4_get_disp = 1;
        ps_pic_buf = impeg2_buf_mgr_get_next_free(ps_dec->pv_pic_buf_mg, &ps_dec->i4_cur_buf_id);

 if (NULL == ps_pic_buf)
 {
 return IMPEG2D_NO_FREE_BUF_ERR;
 }
        impeg2_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mg, ps_dec->i4_cur_buf_id, BUF_MGR_DISP);
        impeg2_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mg, ps_dec->i4_cur_buf_id, BUF_MGR_REF);

        ps_pic_buf->u4_ts = ps_dec->u4_inp_ts;
        ps_pic_buf->e_pic_type = ps_dec->e_pic_type;
        ps_dec->ps_cur_pic = ps_pic_buf;
        ps_dec->s_cur_frm_buf.pu1_y = ps_pic_buf->pu1_y;
        ps_dec->s_cur_frm_buf.pu1_u = ps_pic_buf->pu1_u;
        ps_dec->s_cur_frm_buf.pu1_v = ps_pic_buf->pu1_v;


 if(ps_dec->u2_frame_pred_frame_dct == 0)
 {
            ps_dec->u2_read_dct_type    = 1;
            ps_dec->u2_read_motion_type = 1;
 }
 else
 {
            ps_dec->u2_read_dct_type    = 0;
            ps_dec->u2_read_motion_type = 0;
            ps_dec->u2_motion_type     = 2;
            ps_dec->u2_field_dct       = 0;
 }

        ps_dec->u2_fld_parity          = TOP;
        ps_dec->u2_fld_pic             = 0;
        ps_dec->u2_frm_pic             = 1;
        ps_dec->ps_func_forw_or_back     = gas_impeg2d_func_frm_fw_or_bk;
        ps_dec->ps_func_bi_direct       = gas_impeg2d_func_frm_bi_direct;
 }
    ps_dec->u2_def_dc_pred[Y_LUMA] = 128 << ps_dec->u2_intra_dc_precision;
    ps_dec->u2_def_dc_pred[U_CHROMA] = 128 << ps_dec->u2_intra_dc_precision;
    ps_dec->u2_def_dc_pred[V_CHROMA] = 128 << ps_dec->u2_intra_dc_precision;
    ps_dec->u2_num_mbs_left  = ps_dec->u2_num_horiz_mb * ps_dec->u2_num_vert_mb;
 if(u4_get_disp)
 {
 if(ps_dec->u4_num_frames_decoded > 1)
 {
            ps_disp_pic = impeg2_disp_mgr_get(&ps_dec->s_disp_mgr, &ps_dec->i4_disp_buf_id);
 }
        ps_dec->ps_disp_pic = ps_disp_pic;
 if(ps_disp_pic)
 {
 if(1 == ps_dec->u4_share_disp_buf)
 {
                ps_dec->ps_disp_frm_buf->pv_y_buf  = ps_disp_pic->pu1_y;
 if(IV_YUV_420P == ps_dec->i4_chromaFormat)
 {
                    ps_dec->ps_disp_frm_buf->pv_u_buf  = ps_disp_pic->pu1_u;
                    ps_dec->ps_disp_frm_buf->pv_v_buf  = ps_disp_pic->pu1_v;
 }
 else
 {
                    UWORD8 *pu1_buf;

                    pu1_buf = ps_dec->as_disp_buffers[ps_disp_pic->i4_buf_id].pu1_bufs[1];
                    ps_dec->ps_disp_frm_buf->pv_u_buf  = pu1_buf;

                    pu1_buf = ps_dec->as_disp_buffers[ps_disp_pic->i4_buf_id].pu1_bufs[2];
                    ps_dec->ps_disp_frm_buf->pv_v_buf  = pu1_buf;
 }
 }
 }
 }


 switch(ps_dec->e_pic_type)
 {
 case I_PIC:
 {
            ps_dec->pf_decode_slice = impeg2d_dec_i_slice;
 break;
 }
 case D_PIC:
 {
            ps_dec->pf_decode_slice = impeg2d_dec_d_slice;
 break;
 }
 case P_PIC:
 {
            ps_dec->pf_decode_slice = impeg2d_dec_p_b_slice;
            ps_dec->pu2_mb_type       = gau2_impeg2d_p_mb_type;
 break;
 }
 case B_PIC:
 {
            ps_dec->pf_decode_slice = impeg2d_dec_p_b_slice;
            ps_dec->pu2_mb_type       = gau2_impeg2d_b_mb_type;
 break;
 }
 default:
 return IMPEG2D_INVALID_PIC_TYPE;
 }

  
  
  

  
  
  

 if(ps_dec->e_pic_type == P_PIC)
 {
 if (NULL == ps_dec->as_recent_fld[1][0].pu1_y)
 {
            ps_dec->as_recent_fld[1][0] = ps_dec->s_cur_frm_buf;
 }
 if (NULL == ps_dec->as_recent_fld[1][1].pu1_y)
 {
            impeg2d_get_bottom_field_buf(&ps_dec->s_cur_frm_buf, &ps_dec->as_recent_fld[1][1],
                ps_dec->u2_frame_width);
 }

        ps_dec->as_ref_buf[FORW][TOP] = ps_dec->as_recent_fld[1][0];
        ps_dec->as_ref_buf[FORW][BOTTOM] = ps_dec->as_recent_fld[1][1];


 }
 else if(ps_dec->e_pic_type == B_PIC)
 {
 if((NULL == ps_dec->as_recent_fld[1][0].pu1_y) && (NULL == ps_dec->as_recent_fld[0][0].pu1_y))
 {
            ps_dec->as_recent_fld[1][0] = ps_dec->s_cur_frm_buf;
            impeg2d_get_bottom_field_buf(&ps_dec->s_cur_frm_buf, &ps_dec->as_recent_fld[1][1],
                ps_dec->u2_frame_width);
            ps_dec->as_recent_fld[0][0] = ps_dec->s_cur_frm_buf;
            ps_dec->as_recent_fld[0][1] = ps_dec->as_recent_fld[1][1];
 }
 else if ((NULL != ps_dec->as_recent_fld[1][0].pu1_y) && (NULL == ps_dec->as_recent_fld[0][0].pu1_y))
 {
            ps_dec->as_recent_fld[0][0] = ps_dec->as_recent_fld[1][0];
            ps_dec->as_recent_fld[0][1] = ps_dec->as_recent_fld[1][1];
 }
 else if ((NULL == ps_dec->as_recent_fld[1][0].pu1_y) && (NULL != ps_dec->as_recent_fld[0][0].pu1_y))
 {
            ps_dec->as_recent_fld[1][0] = ps_dec->as_recent_fld[0][0];
            ps_dec->as_recent_fld[1][1] = ps_dec->as_recent_fld[0][1];
 }

        ps_dec->as_ref_buf[FORW][TOP] = ps_dec->as_recent_fld[0][0];
        ps_dec->as_ref_buf[FORW][BOTTOM] = ps_dec->as_recent_fld[0][1];
        ps_dec->as_ref_buf[BACK][TOP] = ps_dec->as_recent_fld[1][0];
        ps_dec->as_ref_buf[BACK][BOTTOM] = ps_dec->as_recent_fld[1][1];


 }

 return e_error;
}
