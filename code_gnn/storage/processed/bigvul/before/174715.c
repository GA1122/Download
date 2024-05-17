WORD32 ih264d_cavlc_parse4x4coeff_n8(WORD16 *pi2_coeff_block,
                                     UWORD32 u4_isdc,  
                                     WORD32 u4_n,
 dec_struct_t *ps_dec,
                                     UWORD32 *pu4_total_coeff)
{

 dec_bit_stream_t *ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 u4_bitstream_offset = ps_bitstrm->u4_ofst;
    UWORD32 u4_code;
    UNUSED(u4_n);
    UNUSED(pi2_coeff_block);
    GETBITS(u4_code, u4_bitstream_offset, pu4_bitstrm_buf, 6);
    ps_bitstrm->u4_ofst = u4_bitstream_offset;
 *pu4_total_coeff = 0;

 if(u4_code != 3)
 {
        UWORD8 *pu1_offset = (UWORD8 *)gau1_ih264d_total_coeff_fn_ptr_offset;
        UWORD32 u4_trailing_ones, u4_offset, u4_total_coeff_tone;

 *pu4_total_coeff = (u4_code >> 2) + 1;
        u4_trailing_ones = u4_code & 0x03;
        u4_offset = pu1_offset[*pu4_total_coeff - 1];
        u4_total_coeff_tone = (*pu4_total_coeff << 16) | u4_trailing_ones;

        ps_dec->pf_cavlc_4x4res_block[u4_offset](u4_isdc,
                                                 u4_total_coeff_tone,
                                                 ps_bitstrm);
 }

 return OK;
}
