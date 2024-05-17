WORD32 ih264d_cavlc_parse_8x8block_none_available(WORD16 *pi2_coeff_block,
                                                  UWORD32 u4_sub_block_strd,
                                                  UWORD32 u4_isdc,
 dec_struct_t * ps_dec,
                                                  UWORD8 *pu1_top_nnz,
                                                  UWORD8 *pu1_left_nnz,
                                                  UWORD8 u1_tran_form8x8,
                                                  UWORD8 u1_mb_field_decodingflag,
                                                  UWORD32 *pu4_csbp)
{
    UWORD32 u4_num_coeff, u4_n, u4_subblock_coded;
    UWORD32 u4_top0, u4_top1;
    UWORD32 *pu4_dummy;
    WORD32 (**pf_cavlc_parse4x4coeff)(WORD16 *pi2_coeff_block,
                                      UWORD32 u4_isdc,
                                      WORD32 u4_n,
 struct _DecStruct *ps_dec,
                                      UWORD32 *pu4_dummy) =
                                      ps_dec->pf_cavlc_parse4x4coeff;
    UWORD32 u4_idx = 0;
    UWORD8 *puc_temp;
    WORD32 ret;

 *pu4_csbp = 0;
    puc_temp = ps_dec->pu1_inv_scan;

  
  
  
 if(u1_tran_form8x8)
 {
 if(!u1_mb_field_decodingflag)
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[0];
 }
 else
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[0];
 }
 }
    ret = pf_cavlc_parse4x4coeff[0](pi2_coeff_block, u4_isdc, 0,
                                    ps_dec, &u4_num_coeff);
 if(ret != OK)
 return ret;

    u4_top0 = u4_num_coeff;
    u4_subblock_coded = (u4_num_coeff != 0);
    INSERT_BIT(*pu4_csbp, u4_idx, u4_subblock_coded);

  
  
  
    u4_idx++;
 if(u1_tran_form8x8)
 {
 if(!u1_mb_field_decodingflag)
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[1];
 }
 else
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[1];
 }
 }
 else
 {
        pi2_coeff_block += NUM_COEFFS_IN_4x4BLK;
 }
    u4_n = u4_num_coeff;
    ret = pf_cavlc_parse4x4coeff[(u4_n > 7)](pi2_coeff_block, u4_isdc,
                                             u4_n, ps_dec, &u4_num_coeff);
 if(ret != OK)
 return ret;

    u4_top1 = pu1_left_nnz[0] = u4_num_coeff;
    u4_subblock_coded = (u4_num_coeff != 0);
    INSERT_BIT(*pu4_csbp, u4_idx, u4_subblock_coded);

  
  
  
    u4_idx += (u4_sub_block_strd - 1);
 if(u1_tran_form8x8)
 {
 if(!u1_mb_field_decodingflag)
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[2];
 }
 else
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[2];
 }
 }
 else
 {
        pi2_coeff_block += ((u4_sub_block_strd - 1) * NUM_COEFFS_IN_4x4BLK);
 }
    u4_n = u4_top0;
    ret = pf_cavlc_parse4x4coeff[(u4_n > 7)](pi2_coeff_block, u4_isdc,
                                             u4_n, ps_dec, &u4_num_coeff);
 if(ret != OK)
 return ret;

    pu1_top_nnz[0] = u4_num_coeff;
    u4_subblock_coded = (u4_num_coeff != 0);
    INSERT_BIT(*pu4_csbp, u4_idx, u4_subblock_coded);

  
  
  
    u4_idx++;
 if(u1_tran_form8x8)
 {
 if(!u1_mb_field_decodingflag)
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[3];
 }
 else
 {
            ps_dec->pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[3];
 }
 }
 else
 {
        pi2_coeff_block += NUM_COEFFS_IN_4x4BLK;
 }
    u4_n = (u4_top1 + u4_num_coeff + 1) >> 1;
    ret = pf_cavlc_parse4x4coeff[(u4_n > 7)](pi2_coeff_block, u4_isdc,
                                             u4_n, ps_dec, &u4_num_coeff);
 if(ret != OK)
 return ret;

    pu1_top_nnz[1] = pu1_left_nnz[1] = u4_num_coeff;
    u4_subblock_coded = (u4_num_coeff != 0);
    INSERT_BIT(*pu4_csbp, u4_idx, u4_subblock_coded);

    ps_dec->pu1_inv_scan = puc_temp;

 return OK;
}
