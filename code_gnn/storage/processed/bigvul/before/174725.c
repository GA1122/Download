WORD32 ih264d_parse_residual4x4_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t *ps_cur_mb_info,
                                      UWORD8 u1_offset)
{
    UWORD8 u1_cbp = ps_cur_mb_info->u1_cbp;
    UWORD16 ui16_csbp = 0;
    UWORD32 u4_nbr_avl;
    WORD16 *pi2_residual_buf;

    UWORD8 u1_is_top_mb_avail;
    UWORD8 u1_is_left_mb_avail;

    UWORD8 *pu1_top_nnz = ps_cur_mb_info->ps_curmb->pu1_nnz_y;
    UWORD8 *pu1_left_nnz = ps_dec->pu1_left_nnz_y;
    WORD16 *pi2_coeff_block = NULL;
    UWORD32 *pu4_dummy;
    WORD32 ret;

    WORD32 (**pf_cavlc_parse_8x8block)(WORD16 *pi2_coeff_block,
                                       UWORD32 u4_sub_block_strd,
                                       UWORD32 u4_isdc,
 struct _DecStruct *ps_dec,
                                       UWORD8 *pu1_top_nnz,
                                       UWORD8 *pu1_left_nnz,
                                       UWORD8 u1_tran_form8x8,
                                       UWORD8 u1_mb_field_decodingflag,
                                       UWORD32 *pu4_dummy) = ps_dec->pf_cavlc_parse_8x8block;


 {
        UWORD8 uc_temp = ps_dec->u1_mb_ngbr_availablity;
        u1_is_top_mb_avail = BOOLEAN(uc_temp & TOP_MB_AVAILABLE_MASK);
        u1_is_left_mb_avail = BOOLEAN(uc_temp & LEFT_MB_AVAILABLE_MASK);
        u4_nbr_avl = (u1_is_top_mb_avail << 1) | u1_is_left_mb_avail;
 }

    ps_cur_mb_info->u1_qp_div6 = ps_dec->u1_qp_y_div6;
    ps_cur_mb_info->u1_qp_rem6 = ps_dec->u1_qp_y_rem6;
    ps_cur_mb_info->u1_qpc_div6 = ps_dec->u1_qp_u_div6;
    ps_cur_mb_info->u1_qpc_rem6 = ps_dec->u1_qp_u_rem6;
    ps_cur_mb_info->u1_qpcr_div6 = ps_dec->u1_qp_v_div6;
    ps_cur_mb_info->u1_qpcr_rem6 = ps_dec->u1_qp_v_rem6;

 if(u1_cbp & 0xf)
 {
        pu1_top_nnz[0] = ps_cur_mb_info->ps_top_mb->pu1_nnz_y[0];
        pu1_top_nnz[1] = ps_cur_mb_info->ps_top_mb->pu1_nnz_y[1];
        pu1_top_nnz[2] = ps_cur_mb_info->ps_top_mb->pu1_nnz_y[2];
        pu1_top_nnz[3] = ps_cur_mb_info->ps_top_mb->pu1_nnz_y[3];

  
  
  
 if(!(u1_cbp & 0x1))
 {
 *(UWORD16 *)(pu1_top_nnz) = 0;
 *(UWORD16 *)(pu1_left_nnz) = 0;

 }
 else
 {
            UWORD32 u4_temp;
            ret = pf_cavlc_parse_8x8block[u4_nbr_avl](
                        pi2_coeff_block, 4, u1_offset, ps_dec, pu1_top_nnz,
                        pu1_left_nnz, ps_cur_mb_info->u1_tran_form8x8,
                        ps_cur_mb_info->u1_mb_field_decodingflag, &u4_temp);
 if(ret != OK)
 return ret;
            ui16_csbp = u4_temp;
 }

  
  
  
 if(ps_cur_mb_info->u1_tran_form8x8)
 {
            pi2_coeff_block += 64;
 }
 else
 {
            pi2_coeff_block += (2 * NUM_COEFFS_IN_4x4BLK);
 }

 if(!(u1_cbp & 0x2))
 {
 *(UWORD16 *)(pu1_top_nnz + 2) = 0;
 *(UWORD16 *)(pu1_left_nnz) = 0;
 }
 else
 {
            UWORD32 u4_temp = (u4_nbr_avl | 0x1);
            ret = pf_cavlc_parse_8x8block[u4_temp](
                        pi2_coeff_block, 4, u1_offset, ps_dec,
 (pu1_top_nnz + 2), pu1_left_nnz,
                        ps_cur_mb_info->u1_tran_form8x8,
                        ps_cur_mb_info->u1_mb_field_decodingflag, &u4_temp);
 if(ret != OK)
 return ret;
            ui16_csbp |= (u4_temp << 2);
 }

  
  
  
 if(ps_cur_mb_info->u1_tran_form8x8)
 {
            pi2_coeff_block += 64;
 }
 else
 {
            pi2_coeff_block += (6 * NUM_COEFFS_IN_4x4BLK);
 }

 if(!(u1_cbp & 0x4))
 {
 *(UWORD16 *)(pu1_top_nnz) = 0;
 *(UWORD16 *)(pu1_left_nnz + 2) = 0;
 }
 else
 {
            UWORD32 u4_temp = (u4_nbr_avl | 0x2);
            ret = pf_cavlc_parse_8x8block[u4_temp](
                        pi2_coeff_block, 4, u1_offset, ps_dec, pu1_top_nnz,
 (pu1_left_nnz + 2), ps_cur_mb_info->u1_tran_form8x8,
                        ps_cur_mb_info->u1_mb_field_decodingflag, &u4_temp);
 if(ret != OK)
 return ret;
            ui16_csbp |= (u4_temp << 8);
 }

  
  
  
 if(ps_cur_mb_info->u1_tran_form8x8)
 {
            pi2_coeff_block += 64;
 }
 else
 {
            pi2_coeff_block += (2 * NUM_COEFFS_IN_4x4BLK);
 }

 if(!(u1_cbp & 0x8))
 {
 *(UWORD16 *)(pu1_top_nnz + 2) = 0;
 *(UWORD16 *)(pu1_left_nnz + 2) = 0;
 }
 else
 {
            UWORD32 u4_temp;
            ret = pf_cavlc_parse_8x8block[0x3](
                        pi2_coeff_block, 4, u1_offset, ps_dec,
 (pu1_top_nnz + 2), (pu1_left_nnz + 2),
                        ps_cur_mb_info->u1_tran_form8x8,
                        ps_cur_mb_info->u1_mb_field_decodingflag, &u4_temp);
 if(ret != OK)
 return ret;
            ui16_csbp |= (u4_temp << 10);
 }
 }
 else
 {
 *(UWORD32 *)(pu1_top_nnz) = 0;
 *(UWORD32 *)(pu1_left_nnz) = 0;
 }

    ps_cur_mb_info->u2_luma_csbp = ui16_csbp;
    ps_cur_mb_info->ps_curmb->u2_luma_csbp = ui16_csbp;

 {
        UWORD16 u2_chroma_csbp = 0;
        ps_cur_mb_info->u2_chroma_csbp = 0;
        pu1_top_nnz = ps_cur_mb_info->ps_curmb->pu1_nnz_uv;
        pu1_left_nnz = ps_dec->pu1_left_nnz_uv;

        u1_cbp >>= 4;
  
  
  
  
 if(u1_cbp == CBPC_ALLZERO || u1_cbp == CBPC_ACZERO)
 {
 *(UWORD32 *)(pu1_top_nnz) = 0;
 *(UWORD32 *)(pu1_left_nnz) = 0;
 }

 if(u1_cbp == CBPC_ALLZERO)
 {
 return (0);
 }
  
  
  
 {
            WORD32 u4_scale_u;
            WORD32 u4_scale_v;
            WORD32 i4_mb_inter_inc;
            u4_scale_u = ps_dec->pu2_quant_scale_u[0] << ps_dec->u1_qp_u_div6;
            u4_scale_v = ps_dec->pu2_quant_scale_v[0] << ps_dec->u1_qp_v_div6;
            i4_mb_inter_inc = (!((ps_cur_mb_info->ps_curmb->u1_mb_type == I_4x4_MB)
 || (ps_cur_mb_info->ps_curmb->u1_mb_type == I_16x16_MB)))
 * 3;

 if(ps_dec->s_high_profile.u1_scaling_present)
 {
                u4_scale_u *=
                                ps_dec->s_high_profile.i2_scalinglist4x4[i4_mb_inter_inc
 + 1][0];
                u4_scale_v *=
                                ps_dec->s_high_profile.i2_scalinglist4x4[i4_mb_inter_inc
 + 2][0];

 }
 else
 {
                u4_scale_u <<= 4;
                u4_scale_v <<= 4;
 }

            ih264d_cavlc_parse_chroma_dc(ps_cur_mb_info,pi2_coeff_block, ps_dec->ps_bitstrm,
                                         u4_scale_u, u4_scale_v,
                                         i4_mb_inter_inc);
 }

 if(u1_cbp == CBPC_ACZERO)
 return (0);

        pu1_top_nnz[0] = ps_cur_mb_info->ps_top_mb->pu1_nnz_uv[0];
        pu1_top_nnz[1] = ps_cur_mb_info->ps_top_mb->pu1_nnz_uv[1];
        pu1_top_nnz[2] = ps_cur_mb_info->ps_top_mb->pu1_nnz_uv[2];
        pu1_top_nnz[3] = ps_cur_mb_info->ps_top_mb->pu1_nnz_uv[3];
  
  
  
 {
            UWORD32 u4_temp;
  
  
  
            ret = pf_cavlc_parse_8x8block[u4_nbr_avl](
                        pi2_coeff_block, 2, 1, ps_dec, pu1_top_nnz,
                        pu1_left_nnz, 0, 0, &u4_temp);
 if(ret != OK)
 return ret;
            u2_chroma_csbp = u4_temp;

            pi2_coeff_block += MB_CHROM_SIZE;
  
  
  
            ret = pf_cavlc_parse_8x8block[u4_nbr_avl](pi2_coeff_block, 2, 1,
                                                      ps_dec,
 (pu1_top_nnz + 2),
 (pu1_left_nnz + 2), 0,
 0, &u4_temp);
 if(ret != OK)
 return ret;
            u2_chroma_csbp |= (u4_temp << 4);
 }

        ps_cur_mb_info->u2_chroma_csbp = u2_chroma_csbp;
 }
 return OK;
}
