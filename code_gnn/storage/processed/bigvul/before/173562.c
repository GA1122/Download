void ih264d_unpack_coeff4x4_8x8blk_chroma(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                          UWORD16 ui2_chroma_csbp,
                                          WORD16 *pi2_out_coeff_data)
{
    UWORD8 *pu1_inv_scan;
    UWORD8 u1_mb_field_decoding_flag = ps_cur_mb_info->u1_mb_field_decodingflag;
    UWORD8 u1_field_coding_flag = ps_cur_mb_info->ps_curmb->u1_mb_fld;

    PROFILE_DISABLE_UNPACK_CHROMA()
 if(u1_field_coding_flag || u1_mb_field_decoding_flag)
 {
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan_fld;
 }
 else
 {
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan;
 }

 if(ui2_chroma_csbp & 0x1)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }
    pi2_out_coeff_data += 16;
 if(ui2_chroma_csbp & 0x2)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }

    pi2_out_coeff_data += 16;
 if(ui2_chroma_csbp & 0x4)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }

    pi2_out_coeff_data += 16;
 if(ui2_chroma_csbp & 0x8)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
 }
}
