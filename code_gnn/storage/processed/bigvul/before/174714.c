WORD32 ih264d_cavlc_parse4x4coeff_n0to7(WORD16 *pi2_coeff_block,
                                        UWORD32 u4_isdc,  
                                        WORD32 u4_n,
 dec_struct_t *ps_dec,
                                        UWORD32 *pu4_total_coeff)
{
 dec_bit_stream_t *ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 u4_bitstream_offset = ps_bitstrm->u4_ofst;
    UWORD32 u4_code, u4_index, u4_ldz;
 const UWORD16 *pu2_code = (const UWORD16*)gau2_ih264d_code_gx;
 const UWORD16 *pu2_offset_num_vlc =
 (const UWORD16 *)gau2_ih264d_offset_num_vlc_tab;
    UWORD32 u4_offset_num_vlc = pu2_offset_num_vlc[u4_n];


    UNUSED(pi2_coeff_block);
 *pu4_total_coeff = 0;
    FIND_ONE_IN_STREAM_32(u4_ldz, u4_bitstream_offset, pu4_bitstrm_buf);
    NEXTBITS(u4_index, u4_bitstream_offset, pu4_bitstrm_buf, 3);
    u4_index += (u4_ldz << 3);
    u4_index += u4_offset_num_vlc;

    u4_index = MIN(u4_index, 303);
    u4_code = pu2_code[u4_index];

    FLUSHBITS(u4_bitstream_offset, (u4_code & 0x03));
    ps_bitstrm->u4_ofst = u4_bitstream_offset;
 *pu4_total_coeff = (u4_code >> 4);

 if(*pu4_total_coeff)
 {
        UWORD32 u4_trailing_ones, u4_offset, u4_total_coeff_tone;
 const UWORD8 *pu1_offset =
 (UWORD8 *)gau1_ih264d_total_coeff_fn_ptr_offset;
        WORD32 ret;
        u4_trailing_ones = ((u4_code >> 2) & 0x03);
        u4_offset = pu1_offset[*pu4_total_coeff - 1];
        u4_total_coeff_tone = (*pu4_total_coeff << 16) | u4_trailing_ones;

        ret = ps_dec->pf_cavlc_4x4res_block[u4_offset](u4_isdc,
                                                       u4_total_coeff_tone,
                                                       ps_bitstrm);
 if(ret != 0)
 return ERROR_CAVLC_NUM_COEFF_T;
 }

 return OK;
}
