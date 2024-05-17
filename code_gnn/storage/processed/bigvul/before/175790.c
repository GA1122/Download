WORD32 ih264d_init_video_decoder(iv_obj_t *dec_hdl,
 ih264d_init_ip_t *ps_init_ip,
 ih264d_init_op_t *ps_init_op)
{
 dec_struct_t * ps_dec;
 iv_mem_rec_t *memtab;
    UWORD8 *pu1_extra_mem_base,*pu1_mem_base;

    memtab = ps_init_ip->s_ivd_init_ip_t.pv_mem_rec_location;

    dec_hdl->pv_codec_handle = memtab[MEM_REC_CODEC].pv_base;
    ps_dec = dec_hdl->pv_codec_handle;

    memset(ps_dec, 0, sizeof(dec_struct_t));

 if(ps_init_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, i4_level))
 {
        ps_dec->u4_level_at_init = ps_init_ip->i4_level;
 }
 else
 {
        ps_dec->u4_level_at_init = H264_LEVEL_3_1;
 }

 if(ps_init_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_num_ref_frames))
 {
        ps_dec->u4_num_ref_frames_at_init = ps_init_ip->u4_num_ref_frames;
 }
 else
 {
        ps_dec->u4_num_ref_frames_at_init = H264_MAX_REF_PICS;
 }

 if(ps_init_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_num_reorder_frames))
 {
        ps_dec->u4_num_reorder_frames_at_init =
                        ps_init_ip->u4_num_reorder_frames;
 }
 else
 {
        ps_dec->u4_num_reorder_frames_at_init = H264_MAX_REF_PICS;
 }

 if(ps_init_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_num_extra_disp_buf))
 {
        ps_dec->u4_num_extra_disp_bufs_at_init =
                        ps_init_ip->u4_num_extra_disp_buf;
 }
 else
 {
        ps_dec->u4_num_extra_disp_bufs_at_init = 0;
 }

 if(ps_init_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_share_disp_buf))
 {
#ifndef LOGO_EN
        ps_dec->u4_share_disp_buf = ps_init_ip->u4_share_disp_buf;
#else
        ps_dec->u4_share_disp_buf = 0;
#endif
 }
 else
 {
        ps_dec->u4_share_disp_buf = 0;
 }

 if((ps_init_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_420P)
 && (ps_init_ip->s_ivd_init_ip_t.e_output_format
 != IV_YUV_420SP_UV)
 && (ps_init_ip->s_ivd_init_ip_t.e_output_format
 != IV_YUV_420SP_VU))
 {
        ps_dec->u4_share_disp_buf = 0;
 }

 if((ps_dec->u4_level_at_init < MIN_LEVEL_SUPPORTED)
 || (ps_dec->u4_level_at_init > MAX_LEVEL_SUPPORTED))
 {
        ps_init_op->s_ivd_init_op_t.u4_error_code |= ERROR_LEVEL_UNSUPPORTED;
 return (IV_FAIL);
 }

 if(ps_dec->u4_num_ref_frames_at_init > H264_MAX_REF_PICS)
 {
        ps_init_op->s_ivd_init_op_t.u4_error_code |= ERROR_NUM_REF;
        ps_dec->u4_num_ref_frames_at_init = H264_MAX_REF_PICS;
 }

 if(ps_dec->u4_num_reorder_frames_at_init > H264_MAX_REF_PICS)
 {
        ps_init_op->s_ivd_init_op_t.u4_error_code |= ERROR_NUM_REF;
        ps_dec->u4_num_reorder_frames_at_init = H264_MAX_REF_PICS;
 }

 if(ps_dec->u4_num_extra_disp_bufs_at_init > H264_MAX_REF_PICS)
 {
        ps_init_op->s_ivd_init_op_t.u4_error_code |= ERROR_NUM_REF;
        ps_dec->u4_num_extra_disp_bufs_at_init = 0;
 }

 if(0 == ps_dec->u4_share_disp_buf)
        ps_dec->u4_num_extra_disp_bufs_at_init = 0;

    ps_dec->u4_num_disp_bufs_requested = 1;

    ps_dec->u4_width_at_init = ps_init_ip->s_ivd_init_ip_t.u4_frm_max_wd;
    ps_dec->u4_height_at_init = ps_init_ip->s_ivd_init_ip_t.u4_frm_max_ht;

    ps_dec->u4_width_at_init = ALIGN16(ps_dec->u4_width_at_init);
    ps_dec->u4_height_at_init = ALIGN32(ps_dec->u4_height_at_init);

    ps_dec->pv_dec_thread_handle = memtab[MEM_REC_THREAD_HANDLE].pv_base;

    pu1_mem_base = memtab[MEM_REC_THREAD_HANDLE].pv_base;
    ps_dec->pv_bs_deblk_thread_handle = pu1_mem_base
 + ithread_get_handle_size();

    ps_dec->u4_extra_mem_used = 0;

    pu1_extra_mem_base = memtab[MEM_REC_EXTRA_MEM].pv_base;

    ps_dec->ps_dec_err_status = (dec_err_status_t *)(pu1_extra_mem_base + ps_dec->u4_extra_mem_used);
    ps_dec->u4_extra_mem_used += (((sizeof(dec_err_status_t) + 127) >> 7) << 7);

    ps_dec->ps_mem_tab = memtab[MEM_REC_BACKUP].pv_base;

    memcpy(ps_dec->ps_mem_tab, memtab, sizeof(iv_mem_rec_t) * MEM_REC_CNT);

    ps_dec->ps_pps = memtab[MEM_REC_PPS].pv_base;

    ps_dec->ps_sps = memtab[MEM_REC_SPS].pv_base;

    ps_dec->ps_sei = (sei *)(pu1_extra_mem_base + ps_dec->u4_extra_mem_used);
    ps_dec->u4_extra_mem_used += sizeof(sei);

    ps_dec->ps_dpb_mgr = memtab[MEM_REC_DPB_MGR].pv_base;

    ps_dec->ps_dpb_cmds = (dpb_commands_t *)(pu1_extra_mem_base + ps_dec->u4_extra_mem_used);
    ps_dec->u4_extra_mem_used += sizeof(dpb_commands_t);

    ps_dec->ps_bitstrm = (dec_bit_stream_t *)(pu1_extra_mem_base + ps_dec->u4_extra_mem_used);
    ps_dec->u4_extra_mem_used += sizeof(dec_bit_stream_t);

    ps_dec->ps_cur_slice =(dec_slice_params_t *) (pu1_extra_mem_base + ps_dec->u4_extra_mem_used);
    ps_dec->u4_extra_mem_used += sizeof(dec_slice_params_t);

    ps_dec->pv_scratch_sps_pps = (void *)(pu1_extra_mem_base + ps_dec->u4_extra_mem_used);


    ps_dec->u4_extra_mem_used += MAX(sizeof(dec_seq_params_t),
 sizeof(dec_pic_params_t));
    ps_dec->ps_pred_pkd = memtab[MEM_REC_PRED_INFO_PKD].pv_base;


    ps_dec->ps_dpb_mgr->pv_codec_handle = ps_dec;

    ps_dec->pv_dec_out = (void *)ps_init_op;
    ps_dec->pv_dec_in = (void *)ps_init_ip;

    ps_dec->u1_chroma_format =
 (UWORD8)(ps_init_ip->s_ivd_init_ip_t.e_output_format);



    ih264d_init_decoder(ps_dec);

 return (IV_SUCCESS);

}
