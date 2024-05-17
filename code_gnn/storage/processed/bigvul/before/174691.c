WORD32 ih264d_mv_pred_ref_tfr_nby2_bmb(dec_struct_t * ps_dec,
                                     UWORD8 u1_mb_idx,
                                     UWORD8 u1_num_mbs)
{
 parse_pmbarams_t * ps_mb_part_info;
 parse_part_params_t * ps_part;
 mv_pred_t *ps_mv_nmb, *ps_mv_nmb_start, *ps_mv_ntop, *ps_mv_ntop_start;
 pic_buffer_t * ps_ref_frame;
    UWORD8 u1_direct_mode_width;
    UWORD8 i, j;
 dec_mb_info_t * ps_cur_mb_info;
 const UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
    UWORD8 u1_field;
    WORD32 ret = 0;

    ps_dec->i4_submb_ofst -= (u1_num_mbs - u1_mb_idx) << 4;
    ps_mb_part_info = ps_dec->ps_parse_mb_data;
    ps_part = ps_dec->ps_parse_part_params;

  
 for(i = u1_mb_idx; i < u1_num_mbs; i++, ps_mb_part_info++)
 {
        UWORD8 u1_colz = 0;
        ps_dec->i4_submb_ofst += SUB_BLK_SIZE;
  
        ps_cur_mb_info = ps_dec->ps_nmb_info + i;
        ps_dec->u2_wait_id = i;

        u1_field = ps_cur_mb_info->u1_mb_field_decodingflag;

        ps_mv_nmb_start = ps_dec->ps_mv_cur + (i << 4);
        ps_dec->u2_mbx = ps_cur_mb_info->u2_mbx;
        ps_dec->u2_mby = ps_cur_mb_info->u2_mby;
        ps_dec->u1_currB_type = 0;
        ps_dec->u2_mv_2mb[i & 0x1] = 0;

  
 if(!ps_mb_part_info->u1_isI_mb)
 {
            UWORD8 u1_blk_no;
            WORD16 i1_ref_idx, i1_ref_idx1;
            UWORD8 u1_pred_mode;
            UWORD8 u1_sub_mb_x, u1_sub_mb_y, u1_sub_mb_num;
            UWORD8 u1_lx, u1_lx_start, u1_lxend, u1_tmp_lx;
            UWORD8 u1_num_part, u1_num_ref, u1_wd, u1_ht;
            UWORD32 *pu4_wt_offst;
            UWORD8 u1_scale_ref, u4_bot_mb;
 deblk_mb_t * ps_cur_deblk_mb = ps_dec->ps_deblk_mbn + i;
            WORD8 (*pi1_ref_idx)[MAX_REFIDX_INFO_PER_MB] =
                            ps_mb_part_info->i1_ref_idx;
            WORD8 *pi1_ref_idx0 = pi1_ref_idx[0],
 *pi1_ref_idx1 = pi1_ref_idx[1];
            UWORD32 **ppu4_wt_ofst = ps_mb_part_info->pu4_wt_offst;

  
            ps_dec->u4_num_pmbair = i >> u1_mbaff;
            ps_dec->u1_mb_idx_mv = i;

  
            ps_mv_ntop_start = ps_mv_nmb_start
 - (ps_dec->u2_frm_wd_in_mbs << (4 + u1_mbaff)) + 12;

            u1_num_part = ps_mb_part_info->u1_num_part;
            ps_cur_deblk_mb->u1_mb_type |= (u1_num_part > 1) << 1;
            u1_direct_mode_width = (1 == ps_mb_part_info->u1_num_part) ? 16 : 8;


            ps_cur_mb_info->u4_pred_info_pkd_idx = ps_dec->u4_pred_info_pkd_idx;
            ps_cur_mb_info->u1_num_pred_parts = 0;

  
  
  
  
            u1_scale_ref = u1_mbaff & ps_cur_mb_info->u1_mb_field_decodingflag;
            u4_bot_mb = 1 - ps_cur_mb_info->u1_topmb;
 if(ps_dec->ps_cur_pps->u1_wted_bipred_idc)
 {
                u1_num_ref = MIN(u1_num_part, 4);
 if(PART_DIRECT_16x16 != ps_part->u1_is_direct)
 {
 for(u1_blk_no = 0; u1_blk_no < u1_num_ref; u1_blk_no++)
 {
                        i1_ref_idx = MAX(pi1_ref_idx0[u1_blk_no], 0);
 if(u1_scale_ref)
                            i1_ref_idx >>= 1;
                        i1_ref_idx *=
                                        ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[1];
 if(u1_scale_ref)
                            i1_ref_idx +=
 (MAX(pi1_ref_idx1[u1_blk_no], 0)
 >> 1);
 else
                            i1_ref_idx += MAX(pi1_ref_idx1[u1_blk_no], 0);
                        pu4_wt_offst = (UWORD32*)&ps_dec->pu4_wt_ofsts[2
 * X3(i1_ref_idx)];

 if(pi1_ref_idx0[u1_blk_no] < 0)
                            pu4_wt_offst += 1;

                        ppu4_wt_ofst[u1_blk_no] = pu4_wt_offst;
 if(u1_scale_ref
 && (ps_dec->ps_cur_pps->u1_wted_bipred_idc
 == 2))
 {
                            i1_ref_idx = MAX(pi1_ref_idx0[u1_blk_no], 0);
                            i1_ref_idx *=
 (ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[1]
 << 1);
                            i1_ref_idx += MAX(pi1_ref_idx1[u1_blk_no], 0);
 if(u4_bot_mb)
 {
                                i1_ref_idx +=
 (ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[0]
 << 1)
 * (ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[1]
 << 1);
 }
                            pu4_wt_offst = (UWORD32*)&ps_dec->pu4_mbaff_wt_mat[2
 * X3(i1_ref_idx)];
                            ppu4_wt_ofst[u1_blk_no] = pu4_wt_offst;
 }
 }
 }
 }

  
  
  
  
 for(j = 0; j < u1_num_part; j++, ps_part++)
 {
                u1_sub_mb_num = ps_part->u1_sub_mb_num;
                ps_dec->u1_sub_mb_num = u1_sub_mb_num;

 if(PART_NOT_DIRECT != ps_part->u1_is_direct)
 {
  
  
  
  
                    ret = ps_dec->ps_cur_slice->pf_decodeDirect(ps_dec,
                                                                u1_direct_mode_width,
                                                                ps_cur_mb_info, i);
 if(ret != OK)
 return ret;
                    ps_cur_deblk_mb->u1_mb_type |= (ps_dec->u1_currB_type << 1);

 }
 else
 {
 mv_pred_t s_mvPred;
  
  
  
  
                    u1_sub_mb_x = u1_sub_mb_num & 0x03;
                    u1_sub_mb_y = u1_sub_mb_num >> 2;
                    u1_blk_no =
 (u1_num_part < 4) ?
                                                    j :
 (((u1_sub_mb_y >> 1) << 1)
 + (u1_sub_mb_x
 >> 1));

                    ps_mv_ntop = ps_mv_ntop_start + u1_sub_mb_x;
                    ps_mv_nmb = ps_mv_nmb_start + u1_sub_mb_num;

                    u1_pred_mode = ps_part->u1_pred_mode;
                    u1_wd = ps_part->u1_partwidth;
                    u1_ht = ps_part->u1_partheight;

                    u1_lx_start = 0;
                    u1_lxend = 2;
 if( PRED_L0 == u1_pred_mode)
 {
                        s_mvPred.i2_mv[2] = 0;
                        s_mvPred.i2_mv[3] = 0;
                        u1_lxend = 1;
 }
 if( PRED_L1 == u1_pred_mode)
 {
                        s_mvPred.i2_mv[0] = 0;
                        s_mvPred.i2_mv[1] = 0;
                        u1_lx_start = 1;
 }

  
                    s_mvPred.i1_ref_frame[0] = pi1_ref_idx0[u1_blk_no];
                    s_mvPred.i1_ref_frame[1] = pi1_ref_idx1[u1_blk_no];

                    ps_dec->pf_mvpred(ps_dec, ps_cur_mb_info, ps_mv_nmb, ps_mv_ntop,
 &s_mvPred, u1_sub_mb_num, u1_wd,
                                      u1_lx_start, u1_lxend,
                                      ps_cur_mb_info->u1_mb_mc_mode);

  
  
  
  
 for(u1_lx = u1_lx_start; u1_lx < u1_lxend; u1_lx++)
 {
                        WORD16 i2_mv_x, i2_mv_y;

  
  
  
  
                        i1_ref_idx = s_mvPred.i1_ref_frame[u1_lx];
                        u1_tmp_lx = (u1_lx << 1);

                        i2_mv_x = ps_mv_nmb->i2_mv[u1_tmp_lx];
                        i2_mv_y = ps_mv_nmb->i2_mv[u1_tmp_lx + 1];

                        i2_mv_x += s_mvPred.i2_mv[u1_tmp_lx];
                        i2_mv_y += s_mvPred.i2_mv[u1_tmp_lx + 1];
                        s_mvPred.i2_mv[u1_tmp_lx] = i2_mv_x;
                        s_mvPred.i2_mv[u1_tmp_lx + 1] = i2_mv_y;

  
  
  
  
  
                        i1_ref_idx1 = i1_ref_idx >> u1_scale_ref;
 if(u1_scale_ref && ((i1_ref_idx & 0x01) != u4_bot_mb))
                            i1_ref_idx1 += MAX_REF_BUFS;
                        ps_ref_frame =
                                        ps_dec->ps_ref_pic_buf_lx[u1_lx][i1_ref_idx1];

  
 if(u1_lx == u1_lx_start)
 {
  
                            s_mvPred.u1_col_ref_pic_idx =
                                            ps_ref_frame->u1_mv_buf_id;
                            s_mvPred.u1_pic_type = ps_ref_frame->u1_pic_type;

  
                            u1_colz =
 (u1_field << 1)
 | ((i1_ref_idx == 0)
 && (ABS(i2_mv_x)
 <= 1)
 && (ABS(i2_mv_y)
 <= 1));
                            u1_colz |= ps_mb_part_info->u1_col_info[u1_blk_no];
 }

                        pu4_wt_offst = ppu4_wt_ofst[u1_blk_no];
 {
 pred_info_pkd_t *ps_pred_pkd;
                           WORD16 i2_mv[2];

                           i2_mv[0] = i2_mv_x;
                           i2_mv[1] = i2_mv_y;

                           ps_pred_pkd = ps_dec->ps_pred_pkd + ps_dec->u4_pred_info_pkd_idx;
                        ih264d_fill_pred_info(i2_mv,u1_wd,u1_ht,u1_sub_mb_num,u1_pred_mode,
                                        ps_pred_pkd,ps_ref_frame->u1_pic_buf_id,i1_ref_idx,pu4_wt_offst,
                                        ps_ref_frame->u1_pic_type);
                        ps_dec->u4_pred_info_pkd_idx++;
                        ps_cur_mb_info->u1_num_pred_parts++;


 }

 }
                    ih264d_rep_mv_colz(ps_dec, &s_mvPred, ps_mv_nmb,
                                       u1_sub_mb_num, u1_colz, u1_ht,
                                       u1_wd);
 }
 }

 }
 else
 {
  
 mv_pred_t s_mvPred =
 {
 { 0, 0, 0, 0 },
 { -1, -1 }, 0, 0};
  
            ih264d_rep_mv_colz(ps_dec, &s_mvPred, ps_mv_nmb_start, 0,
 (UWORD8)(u1_field << 1), 4, 4);
 }

  
 if(ps_dec->u4_num_cores < 3)
 {
 if(ps_dec->u4_app_disable_deblk_frm == 0)
                ps_dec->pf_compute_bs(ps_dec, ps_cur_mb_info,
 (UWORD16)(i >> u1_mbaff));
 }
 }
 return OK;
}