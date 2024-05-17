void impeg2d_dec_pic_data(dec_state_t *ps_dec)
{

    WORD32 i;
 dec_state_multi_core_t *ps_dec_state_multi_core;

    UWORD32  u4_error_code;

 dec_state_t *ps_dec_thd;
    WORD32 i4_status;
    WORD32 i4_min_mb_y;


  
    ps_dec->u2_mb_x = ps_dec->u2_mb_y = 0;
    u4_error_code = 0;

    ps_dec_state_multi_core = ps_dec->ps_dec_state_multi_core;
    impeg2d_get_slice_pos(ps_dec_state_multi_core);

    i4_min_mb_y = 1;
 for(i=0; i < ps_dec->i4_num_cores - 1; i++)
 {

        ps_dec_thd = ps_dec_state_multi_core->ps_dec_state[i+1];

        ps_dec_thd->ps_disp_pic = ps_dec->ps_disp_pic;
        ps_dec_thd->ps_disp_frm_buf = ps_dec->ps_disp_frm_buf;

        i4_status = impeg2d_init_thread_dec_ctxt(ps_dec, ps_dec_thd, i4_min_mb_y);

 if(i4_status == 0)
 {
            ithread_create(ps_dec_thd->pv_codec_thread_handle, NULL, (void *)impeg2d_dec_pic_data_thread, ps_dec_thd);
            ps_dec_state_multi_core->au4_thread_launched[i + 1] = 1;
            i4_min_mb_y = ps_dec_thd->u2_mb_y + 1;
 }
 else
 {
            ps_dec_state_multi_core->au4_thread_launched[i + 1] = 0;
 break;
 }
 }

    impeg2d_dec_pic_data_thread(ps_dec);

 for(i=0; i < (ps_dec->i4_num_cores - 1); i++)
 {
 if(ps_dec_state_multi_core->au4_thread_launched[i + 1] == 1)
 {
            ps_dec_thd = ps_dec_state_multi_core->ps_dec_state[i+1];
            ithread_join(ps_dec_thd->pv_codec_thread_handle, NULL);
 }
 }

    ps_dec->u4_error_code = u4_error_code;

}
