 WORD32 ih264d_parse_inter_slice_data_cabac(dec_struct_t * ps_dec,
  dec_slice_params_t * ps_slice,
                                            UWORD16 u2_first_mb_in_slice)
 {
     UWORD32 uc_more_data_flag;
     WORD32 i2_cur_mb_addr;
     UWORD32 u1_num_mbs, u1_num_mbsNby2, u1_mb_idx;
     UWORD32 u1_mbaff;
     UWORD32 u1_num_mbs_next, u1_end_of_row;
  const UWORD16 i2_pic_wdin_mbs = ps_dec->u2_frm_wd_in_mbs;
     UWORD32 u1_slice_end = 0;
     UWORD32 u1_tfr_n_mb = 0;
     UWORD32 u1_decode_nmb = 0;
 
 
  deblk_mb_t *ps_cur_deblk_mb;
  dec_mb_info_t *ps_cur_mb_info;
 
  parse_pmbarams_t *ps_parse_mb_data = ps_dec->ps_parse_mb_data;
     UWORD32 u1_inter_mb_skip_type;
     UWORD32 u1_inter_mb_type;
     UWORD32 u1_deblk_mb_type;
     UWORD32 u1_mb_threshold;
  dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
     WORD32 ret = OK;
 
   
   
   
  if(ps_slice->u1_slice_type == P_SLICE)
  {
         u1_inter_mb_skip_type = CAB_P_SKIP;
         u1_inter_mb_type = P_MB;
         u1_deblk_mb_type = D_INTER_MB;
         u1_mb_threshold = 5;
  }
  else  
  {
         u1_inter_mb_skip_type = CAB_B_SKIP;
         u1_inter_mb_type = B_MB;
         u1_deblk_mb_type = D_B_SLICE;
         u1_mb_threshold = 23;
  }
 
   
   
   
     i2_cur_mb_addr = u2_first_mb_in_slice;
     ps_dec->u1_qp = ps_slice->u1_slice_qp;
     ih264d_update_qp(ps_dec, 0);
     u1_mb_idx = ps_dec->u1_mb_idx;
     u1_num_mbs = u1_mb_idx;
     u1_num_mbsNby2 = 0;
     u1_mbaff = ps_slice->u1_mbaff_frame_flag;
     i2_cur_mb_addr = u2_first_mb_in_slice << u1_mbaff;
     uc_more_data_flag = 1;
 
   
  if(ps_bitstrm->u4_ofst & 0x07)
  {
         ps_bitstrm->u4_ofst += 8;
         ps_bitstrm->u4_ofst &= 0xFFFFFFF8;
  }
 
     ret = ih264d_init_cabac_dec_envirnoment(&(ps_dec->s_cab_dec_env), ps_bitstrm);
  if(ret != OK)
  return ret;
 
     ps_dec->i1_prev_mb_qp_delta = 0;
 
  while(!u1_slice_end)
  {
         UWORD8 u1_mb_type;
         UWORD32 u4_mb_skip;
 
         ps_dec->pv_prev_mb_parse_tu_coeff_data = ps_dec->pv_parse_tu_coeff_data;
 
  if(i2_cur_mb_addr > ps_dec->ps_cur_sps->u2_max_mb_addr)
  {
             ret = ERROR_MB_ADDRESS_T;
  break;
  }
 
         ps_cur_mb_info = ps_dec->ps_nmb_info + u1_num_mbs;
         ps_dec->u4_num_mbs_cur_nmb = u1_num_mbs;
 
         ps_cur_mb_info->u1_Mux = 0;
         ps_dec->u4_num_pmbair = (u1_num_mbs >> u1_mbaff);
         ps_cur_deblk_mb = ps_dec->ps_deblk_mbn + u1_num_mbs;
 
         ps_cur_mb_info->u1_end_of_slice = 0;
 
   
         ps_parse_mb_data->u1_num_part = 1;
         ps_parse_mb_data->u1_isI_mb = 0;
 
   
   
   
   
         u4_mb_skip = ps_dec->pf_get_mb_info(ps_dec, i2_cur_mb_addr, ps_cur_mb_info, 1);
 
   
   
   
         ps_cur_mb_info->u1_tran_form8x8 = 0;
         ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;
 
   
   
   
  if(ps_dec->u4_app_disable_deblk_frm == 0)
             ih264d_set_deblocking_parameters(ps_cur_deblk_mb, ps_slice,
                                              ps_dec->u1_mb_ngbr_availablity,
                                              ps_dec->u1_cur_mb_fld_dec_flag);
 
  if(u4_mb_skip)
  {
 
   
             memset(ps_dec->ps_curr_ctxt_mb_info, 0, sizeof(ctxt_inc_mb_info_t));
             ps_dec->ps_curr_ctxt_mb_info->u1_mb_type = u1_inter_mb_skip_type;
 
             MEMSET_16BYTES(&ps_dec->pu1_left_mv_ctxt_inc[0][0], 0);
 
  *((UWORD32 *)ps_dec->pi1_left_ref_idx_ctxt_inc) = 0;
  *(ps_dec->pu1_left_yuv_dc_csbp) = 0;
 
             ps_dec->i1_prev_mb_qp_delta = 0;
             ps_cur_mb_info->u1_mb_type = MB_SKIP;
             ps_cur_mb_info->u1_cbp = 0;
 
  {
   
  parse_part_params_t *ps_part_info = ps_dec->ps_part;
                 ps_part_info->u1_is_direct = PART_DIRECT_16x16;
                 ps_part_info->u1_sub_mb_num = 0;
                 ps_dec->ps_part++;
  }
 
   
             ih264d_update_nnz_for_skipmb(ps_dec, ps_cur_mb_info, CABAC);
 
             ps_cur_mb_info->ps_curmb->u1_mb_type = u1_inter_mb_type;
             ps_cur_deblk_mb->u1_mb_type |= u1_deblk_mb_type;
             ps_cur_deblk_mb->u1_mb_qp = ps_dec->u1_qp;
 
  }
  else
  {
 
   
   
             u1_mb_type = ih264d_parse_mb_type_cabac(ps_dec);
             ps_cur_mb_info->u1_mb_type = u1_mb_type;
  if(u1_mb_type > (25 + u1_mb_threshold))
  return ERROR_MB_TYPE;
 
   
  if(u1_mb_type < u1_mb_threshold)
  {
                 ps_cur_mb_info->ps_curmb->u1_mb_type = u1_inter_mb_type;
  *(ps_dec->pu1_left_yuv_dc_csbp) &= 0x6;
 
                 ret = ps_dec->pf_parse_inter_mb(ps_dec, ps_cur_mb_info, u1_num_mbs,
                                           u1_num_mbsNby2);
  if(ret != OK)
  return ret;
                 ps_cur_deblk_mb->u1_mb_qp = ps_dec->u1_qp;
                 ps_cur_deblk_mb->u1_mb_type |= u1_deblk_mb_type;
  }
  else
  {
   
                 ps_parse_mb_data->u1_num_part = 0;
                 ps_parse_mb_data->u1_isI_mb = 1;
 
  if((25 + u1_mb_threshold) == u1_mb_type)
  {
   
                     ps_cur_mb_info->ps_curmb->u1_mb_type = I_PCM_MB;
                     ret = ih264d_parse_ipcm_mb(ps_dec, ps_cur_mb_info, u1_num_mbs);
  if(ret != OK)
  return ret;
                     ps_cur_deblk_mb->u1_mb_qp = 0;
  }
  else
  {
  if(u1_mb_type == u1_mb_threshold)
                         ps_cur_mb_info->ps_curmb->u1_mb_type = I_4x4_MB;
  else
                         ps_cur_mb_info->ps_curmb->u1_mb_type = I_16x16_MB;
 
                     ret = ih264d_parse_imb_cabac(
                                     ps_dec, ps_cur_mb_info,
  (UWORD8)(u1_mb_type - u1_mb_threshold));
  if(ret != OK)
  return ret;
                     ps_cur_deblk_mb->u1_mb_qp = ps_dec->u1_qp;
  }
                 ps_cur_deblk_mb->u1_mb_type |= D_INTRA_MB;
 
  }
 
  }
 
  if(u1_mbaff)
  {
             ih264d_update_mbaff_left_nnz(ps_dec, ps_cur_mb_info);
  }
   
         i2_cur_mb_addr++;
 
  if(ps_cur_mb_info->u1_topmb && u1_mbaff)
             uc_more_data_flag = 1;
  else
  {
             uc_more_data_flag = ih264d_decode_terminate(&ps_dec->s_cab_dec_env,
                                                       ps_bitstrm);
             uc_more_data_flag = !uc_more_data_flag;
             COPYTHECONTEXT("Decode Sliceterm",!uc_more_data_flag);
 
          }
  
          u1_num_mbs++;
        ps_dec->u2_total_mbs_coded++;
          u1_num_mbsNby2++;
          ps_parse_mb_data++;
  
   
   
   
   
   
         u1_num_mbs_next = i2_pic_wdin_mbs - ps_dec->u2_mbx - 1;
         u1_end_of_row = (!u1_num_mbs_next) && (!(u1_mbaff && (u1_num_mbs & 0x01)));
         u1_slice_end = !uc_more_data_flag;
         u1_tfr_n_mb = (u1_num_mbs == ps_dec->u1_recon_mb_grp) || u1_end_of_row
  || u1_slice_end;
         u1_decode_nmb = u1_tfr_n_mb || u1_slice_end;
         ps_cur_mb_info->u1_end_of_slice = u1_slice_end;
   
 
  if(u1_decode_nmb)
  {
 
             ps_dec->pf_mvpred_ref_tfr_nby2mb(ps_dec, u1_mb_idx, u1_num_mbs);
             u1_num_mbsNby2 = 0;
 
  {
                 ps_parse_mb_data = ps_dec->ps_parse_mb_data;
                 ps_dec->ps_part = ps_dec->ps_parse_part_params;
  }
  }
 
   
  if(u1_decode_nmb)
  {
 
  if(ps_dec->u1_separate_parse)
  {
                 ih264d_parse_tfr_nmb(ps_dec, u1_mb_idx, u1_num_mbs,
                                      u1_num_mbs_next, u1_tfr_n_mb, u1_end_of_row);
                 ps_dec->ps_nmb_info +=  u1_num_mbs;
  }
  else
  {
                 ih264d_decode_recon_tfr_nmb(ps_dec, u1_mb_idx, u1_num_mbs,
 
                                              u1_num_mbs_next, u1_tfr_n_mb,
                                              u1_end_of_row);
              }
//             ps_dec->u2_total_mbs_coded += u1_num_mbs;
              if(u1_tfr_n_mb)
                  u1_num_mbs = 0;
              u1_mb_idx = u1_num_mbs;
             ps_dec->u1_mb_idx = u1_num_mbs;
 
  }
  }
 
 
     ps_dec->u4_num_mbs_cur_nmb = 0;
     ps_dec->ps_cur_slice->u4_mbs_in_slice = i2_cur_mb_addr
 
  - (u2_first_mb_in_slice << u1_mbaff);
 
  return ret;
 }