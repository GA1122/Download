IV_API_CALL_STATUS_T impeg2d_api_reset(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
    UNUSED(pv_api_ip);
 impeg2d_ctl_reset_op_t *s_ctl_reset_op = (impeg2d_ctl_reset_op_t *)pv_api_op;

    WORD32 i4_num_threads;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

 if(ps_dec_state_multi_core != NULL)
 {
 if(ps_dec_state->aps_ref_pics[1] != NULL)
            impeg2_buf_mgr_release(ps_dec_state->pv_pic_buf_mg, ps_dec_state->aps_ref_pics[1]->i4_buf_id, BUF_MGR_REF);
 if(ps_dec_state->aps_ref_pics[0] != NULL)
            impeg2_buf_mgr_release(ps_dec_state->pv_pic_buf_mg, ps_dec_state->aps_ref_pics[0]->i4_buf_id, BUF_MGR_REF);
 while(1)
 {
 pic_buf_t *ps_disp_pic = impeg2_disp_mgr_get(&ps_dec_state->s_disp_mgr, &ps_dec_state->i4_disp_buf_id);
 if(NULL == ps_disp_pic)
 break;
 if(0 == ps_dec_state->u4_share_disp_buf)
                impeg2_buf_mgr_release(ps_dec_state->pv_pic_buf_mg, ps_disp_pic->i4_buf_id, BUF_MGR_DISP);

 }

 if((ps_dec_state->u4_deinterlace) && (NULL != ps_dec_state->ps_deint_pic))
 {
            impeg2_buf_mgr_release(ps_dec_state->pv_pic_buf_mg,
                                   ps_dec_state->ps_deint_pic->i4_buf_id,
                                   MPEG2_BUF_MGR_DEINT);
 }

 for(i4_num_threads = 0; i4_num_threads < MAX_THREADS; i4_num_threads++)
 {
            ps_dec_state = ps_dec_state_multi_core->ps_dec_state[i4_num_threads];


  
  

            ps_dec_state->u2_header_done    = 0;  
            ps_dec_state->u4_frm_buf_stride = 0;
            ps_dec_state->u2_is_mpeg2       = 0;
            ps_dec_state->aps_ref_pics[0] = NULL;
            ps_dec_state->aps_ref_pics[1] = NULL;
            ps_dec_state->ps_deint_pic = NULL;
 }
 }
 else
 {
        s_ctl_reset_op->s_ivd_ctl_reset_op_t.u4_error_code =
                        IMPEG2D_INIT_NOT_DONE;
 }

 return(IV_SUCCESS);
}
