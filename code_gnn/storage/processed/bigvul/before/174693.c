WORD32 ih264d_parse_bmb_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_num,
                              UWORD8 u1_num_mbsNby2)
{
    UWORD32 u4_cbp;
 deblk_mb_t * ps_cur_deblk_mb = ps_dec->ps_deblk_mbn + u1_mb_num;
 dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 * pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
 const UWORD8 *puc_mb_mc_mode = (const UWORD8 *)gau1_ih264d_mb_mc_mode;
    UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;

    WORD32 ret;
    UWORD8 u1_Bdirect_tranform_read = 1;
    ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 1;
    ps_cur_mb_info->u1_tran_form8x8 = 0;
    ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

    ps_cur_mb_info->u1_mb_mc_mode = puc_mb_mc_mode[5 + u1_mb_type];

    ps_cur_deblk_mb->u1_mb_type |= D_B_SLICE;
 if(u1_mb_type != B_DIRECT)
 {
        ret = ih264d_parse_bmb_non_direct_cavlc(ps_dec, ps_cur_mb_info, u1_mb_num,
                                          u1_num_mbsNby2);
 if(ret != OK)
 return ret;
 }
 else
 {
  
 parse_part_params_t * ps_part_info;
        ps_part_info = ps_dec->ps_part;
        ps_part_info->u1_is_direct = PART_DIRECT_16x16;
        ps_part_info->u1_sub_mb_num = 0;
        ps_dec->ps_part++;
  
        u1_Bdirect_tranform_read =
                        ps_dec->s_high_profile.u1_direct_8x8_inference_flag;
 }

  
 {
 const UWORD8 * puc_CbpInter = gau1_ih264d_cbp_inter;
        UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
        UWORD32 u4_word, u4_ldz;

  
  
  
        NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
        u4_ldz = CLZ(u4_word);
  
        u4_bitstream_offset += (u4_ldz + 1);
  
        u4_word = 0;
 if(u4_ldz)
            GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf, u4_ldz);
 *pu4_bitstrm_ofst = u4_bitstream_offset;
        u4_cbp = ((1 << u4_ldz) + u4_word - 1);
 if(u4_cbp > 47)
 return ERROR_CBP;
        u4_cbp = puc_CbpInter[u4_cbp];

 if((ps_dec->s_high_profile.u1_transform8x8_present) && (u4_cbp & (0xf))
 && (ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag)
 && (u1_Bdirect_tranform_read))
 {
            ps_cur_mb_info->u1_tran_form8x8 = ih264d_get_bit_h264(ps_bitstrm);
            COPYTHECONTEXT("transform_size_8x8_flag", ps_cur_mb_info->u1_tran_form8x8);
            ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = ps_cur_mb_info->u1_tran_form8x8;
 }

 }

    COPYTHECONTEXT("coded_block_pattern", u4_cbp);
    ps_cur_mb_info->u1_cbp = u4_cbp;

  
 if(u4_cbp)
 {
        WORD32 i_temp;

        UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
        UWORD32 u4_word, u4_ldz, u4_abs_val;

  
  
  
        NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
        u4_ldz = CLZ(u4_word);

  
        u4_bitstream_offset += (u4_ldz + 1);

  
        u4_word = 0;
 if(u4_ldz)
            GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf, u4_ldz);

 *pu4_bitstrm_ofst = u4_bitstream_offset;
        u4_abs_val = ((1 << u4_ldz) + u4_word) >> 1;

 if(u4_word & 0x1)
            i_temp = (-(WORD32)u4_abs_val);
 else
            i_temp = (u4_abs_val);

 if(i_temp < -26 || i_temp > 25)
 return ERROR_INV_RANGE_QP_T;
        COPYTHECONTEXT("mb_qp_delta", i_temp);
 if(i_temp)
 {
            ret = ih264d_update_qp(ps_dec, (WORD8)i_temp);
 if(ret != OK)
 return ret;
 }

        ret = ih264d_parse_residual4x4_cavlc(ps_dec, ps_cur_mb_info, 0);
 if(ret != OK)
 return ret;
 if(EXCEED_OFFSET(ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;
 }
 else
 {
        ps_dec->i1_prev_mb_qp_delta = 0;
        ih264d_update_nnz_for_skipmb(ps_dec, ps_cur_mb_info, CAVLC);
 }

 return OK;
}
