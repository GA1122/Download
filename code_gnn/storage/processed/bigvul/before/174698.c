WORD32 ih264d_parse_ipcm_mb(dec_struct_t * ps_dec,
 dec_mb_info_t *ps_cur_mb_info,
                          UWORD8 u1_mbNum)
{
 dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
    UWORD8 *pu1_y, *pu1_u, *pu1_v;
    WORD32 ret;

    UWORD32 u4_rec_width_y, u4_rec_width_uv;
    UWORD32 u1_num_mb_pair;
    UWORD8 u1_x, u1_y;
  
 tfr_ctxt_t *ps_frame_buf;
    UWORD8 u1_mb_field_decoding_flag;
    UWORD32 *pu4_buf;
    UWORD8 *pu1_buf;
  

 if(ps_dec->u1_separate_parse)
 {
        ps_frame_buf = &ps_dec->s_tran_addrecon_parse;
 }
 else
 {
        ps_frame_buf = &ps_dec->s_tran_addrecon;
 }
  
  
  
 if(ps_bitstrm->u4_ofst & 0x07)
 {
        ps_bitstrm->u4_ofst += 8;
        ps_bitstrm->u4_ofst &= 0xFFFFFFF8;
 }

  

    pu1_buf = ps_dec->pu1_left_nnz_y;
    pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x10101010;
    pu1_buf = ps_cur_mb_info->ps_curmb->pu1_nnz_y;
    pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x10101010;
    pu1_buf = ps_cur_mb_info->ps_curmb->pu1_nnz_uv;
    pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x10101010;
    pu1_buf = ps_dec->pu1_left_nnz_uv;
    pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x10101010;
    ps_cur_mb_info->u1_cbp = 0xff;

    ps_dec->i1_prev_mb_qp_delta = 0;
  
    u1_num_mb_pair = (u1_mbNum >> u1_mbaff);

  
  
  
  
    u1_mb_field_decoding_flag = ps_cur_mb_info->u1_mb_field_decodingflag;
    pu1_y = ps_frame_buf->pu1_dest_y + (u1_num_mb_pair << 4);
    pu1_u = ps_frame_buf->pu1_dest_u + (u1_num_mb_pair << 4);
    pu1_v = pu1_u + 1;

    u4_rec_width_y = ps_dec->u2_frm_wd_y << u1_mb_field_decoding_flag;
    u4_rec_width_uv = ps_dec->u2_frm_wd_uv << u1_mb_field_decoding_flag;
  

 if(u1_mbaff)
 {
        UWORD8 u1_top_mb;

        u1_top_mb = ps_cur_mb_info->u1_topmb;

 if(u1_top_mb == 0)
 {
            pu1_y += (u1_mb_field_decoding_flag ?
 (u4_rec_width_y >> 1) : (u4_rec_width_y << 4));
            pu1_u += (u1_mb_field_decoding_flag ?
 (u4_rec_width_uv) : (u4_rec_width_uv << 4));
            pu1_v = pu1_u + 1;
 }
 }

  
 for(u1_y = 0; u1_y < 16; u1_y++)
 {
 for(u1_x = 0; u1_x < 16; u1_x++)
            pu1_y[u1_x] = ih264d_get_bits_h264(ps_bitstrm, 8);

        pu1_y += u4_rec_width_y;
 }

  
 for(u1_y = 0; u1_y < 8; u1_y++)
 {
 for(u1_x = 0; u1_x < 8; u1_x++)
            pu1_u[u1_x * YUV420SP_FACTOR] = ih264d_get_bits_h264(ps_bitstrm, 8);

        pu1_u += u4_rec_width_uv;
 }

 for(u1_y = 0; u1_y < 8; u1_y++)
 {
 for(u1_x = 0; u1_x < 8; u1_x++)
            pu1_v[u1_x * YUV420SP_FACTOR] = ih264d_get_bits_h264(ps_bitstrm, 8);

        pu1_v += u4_rec_width_uv;
 }

 if(CABAC == ps_dec->ps_cur_pps->u1_entropy_coding_mode)
 {
        UWORD32 *pu4_buf;
        UWORD8 *pu1_buf;
 ctxt_inc_mb_info_t *p_curr_ctxt = ps_dec->ps_curr_ctxt_mb_info;
  
        ret = ih264d_init_cabac_dec_envirnoment(&(ps_dec->s_cab_dec_env), ps_bitstrm);
 if(ret != OK)
 return ret;
  
        p_curr_ctxt->u1_mb_type = CAB_I_PCM;
        p_curr_ctxt->u1_cbp = 47;
        p_curr_ctxt->u1_intra_chroma_pred_mode = 0;
        p_curr_ctxt->u1_transform8x8_ctxt = 0;
        ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

        pu1_buf = ps_dec->pu1_left_nnz_y;
        pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x01010101;

        pu1_buf = ps_cur_mb_info->ps_curmb->pu1_nnz_y;
        pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x01010101;

        pu1_buf = ps_cur_mb_info->ps_curmb->pu1_nnz_uv;
        pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x01010101;

        pu1_buf = ps_dec->pu1_left_nnz_uv;
        pu4_buf = (UWORD32 *)pu1_buf;
 *pu4_buf = 0x01010101;

        p_curr_ctxt->u1_yuv_dc_csbp = 0x7;
        ps_dec->pu1_left_yuv_dc_csbp[0] = 0x7;
 if(ps_dec->ps_cur_slice->u1_slice_type != I_SLICE)
 {

            MEMSET_16BYTES(&ps_dec->pu1_left_mv_ctxt_inc[0][0], 0);
            memset(ps_dec->pi1_left_ref_idx_ctxt_inc, 0, 4);
            MEMSET_16BYTES(p_curr_ctxt->u1_mv, 0);
            memset(p_curr_ctxt->i1_ref_idx, 0, 4);

 }
 }
 return OK;
}
