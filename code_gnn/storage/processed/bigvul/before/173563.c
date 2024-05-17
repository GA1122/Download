WORD32 ih264d_unpack_coeff8x8_8x8blk_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                            UWORD16 ui2_luma_csbp,
                                            WORD16 *pi2_out_coeff_data)
{
    UWORD8 *pu1_inv_scan;
    UWORD8 u1_mb_field_decoding_flag = ps_cur_mb_info->u1_mb_field_decodingflag;
    UWORD8 u1_field_coding_flag = ps_cur_mb_info->ps_curmb->u1_mb_fld;
    WORD32 dc_only_flag = 0;

    PROFILE_DISABLE_UNPACK_LUMA()
 if(ui2_luma_csbp & 0x33)
 {
        memset(pi2_out_coeff_data,0,64*sizeof(WORD16));
 }

 if(!u1_mb_field_decoding_flag)
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[0];
 }
 else
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[0];
 }
 if(ui2_luma_csbp & 0x1)
 {
        dc_only_flag = ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }

 if(!u1_mb_field_decoding_flag)
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[1];
 }
 else
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[1];
 }
 if(ui2_luma_csbp & 0x2)
 {
        dc_only_flag = 0;
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }

 if(!u1_mb_field_decoding_flag)
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[2];
 }
 else
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[2];
 }
 if(ui2_luma_csbp & 0x10)
 {
        dc_only_flag = 0;
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }

 if(!u1_mb_field_decoding_flag)
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_prog8x8_cavlc[3];
 }
 else
 {
        pu1_inv_scan =
 (UWORD8*)gau1_ih264d_inv_scan_int8x8_cavlc[3];
 }
 if(ui2_luma_csbp & 0x20)
 {
        dc_only_flag = 0;
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }
 return dc_only_flag;
}
