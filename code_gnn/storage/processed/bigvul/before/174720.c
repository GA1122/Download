void ih264d_cavlc_parse_chroma_dc(dec_mb_info_t *ps_cur_mb_info,
                                  WORD16 *pi2_coeff_block,
 dec_bit_stream_t *ps_bitstrm,
                                  UWORD32 u4_scale_u,
                                  UWORD32 u4_scale_v,
                                  WORD32 i4_mb_inter_inc)
{
    UWORD32 u4_total_coeff, u4_trailing_ones, u4_total_coeff_tone, u4_code;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 u4_bitstream_offset = ps_bitstrm->u4_ofst;
 const UWORD8 *pu1_cav_chromdc = (const UWORD8*)gau1_ih264d_cav_chromdc_vld;
    UNUSED(i4_mb_inter_inc);
  
  
  
    NEXTBITS(u4_code, u4_bitstream_offset, pu4_bitstrm_buf, 8);

    u4_code = pu1_cav_chromdc[u4_code];

    FLUSHBITS(u4_bitstream_offset, ((u4_code & 0x7) + 1));
    ps_bitstrm->u4_ofst = u4_bitstream_offset;

    u4_total_coeff = (u4_code >> 5);

 if(u4_total_coeff)
 {
        WORD32 i_z0, i_z1, i_z2, i_z3;
 tu_sblk4x4_coeff_data_t *ps_tu_4x4;
 dec_struct_t *ps_dec = (dec_struct_t *)ps_bitstrm->pv_codec_handle;
        WORD16 ai2_dc_coef[4];
        UWORD8 pu1_inv_scan[4] =
 { 0, 1, 2, 3 };
        WORD16 *pi2_coeff_data =
 (WORD16 *)ps_dec->pv_parse_tu_coeff_data;

        ps_tu_4x4 = (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;

        u4_trailing_ones = ((u4_code >> 3) & 0x3);
        u4_total_coeff_tone = (u4_total_coeff << 16) | u4_trailing_ones;
        ih264d_rest_of_residual_cav_chroma_dc_block(u4_total_coeff_tone,
                                                    ps_bitstrm);

        ai2_dc_coef[0] = 0;
        ai2_dc_coef[1] = 0;
        ai2_dc_coef[2] = 0;
        ai2_dc_coef[3] = 0;

        ih264d_unpack_coeff4x4_dc_4x4blk(ps_tu_4x4,
                                         ai2_dc_coef,
                                         pu1_inv_scan);
  
  
  
        i_z0 = (ai2_dc_coef[0] + ai2_dc_coef[2]);
        i_z1 = (ai2_dc_coef[0] - ai2_dc_coef[2]);
        i_z2 = (ai2_dc_coef[1] - ai2_dc_coef[3]);
        i_z3 = (ai2_dc_coef[1] + ai2_dc_coef[3]);

  
  
  
 *pi2_coeff_data++ = ((i_z0 + i_z3) * u4_scale_u) >> 5;
 *pi2_coeff_data++ = ((i_z0 - i_z3) * u4_scale_u) >> 5;
 *pi2_coeff_data++ = ((i_z1 + i_z2) * u4_scale_u) >> 5;
 *pi2_coeff_data++ = ((i_z1 - i_z2) * u4_scale_u) >> 5;

        ps_dec->pv_parse_tu_coeff_data = (void *)pi2_coeff_data;

        SET_BIT(ps_cur_mb_info->u1_yuv_dc_block_flag,1);
 }

  
  
  
    pi2_coeff_block += 64;
    u4_bitstream_offset = ps_bitstrm->u4_ofst;

    NEXTBITS(u4_code, u4_bitstream_offset, pu4_bitstrm_buf, 8);

    u4_code = pu1_cav_chromdc[u4_code];

    FLUSHBITS(u4_bitstream_offset, ((u4_code & 0x7) + 1));
    ps_bitstrm->u4_ofst = u4_bitstream_offset;

    u4_total_coeff = (u4_code >> 5);

 if(u4_total_coeff)
 {
        WORD32 i_z0, i_z1, i_z2, i_z3;
 tu_sblk4x4_coeff_data_t *ps_tu_4x4;
 dec_struct_t *ps_dec = (dec_struct_t *)ps_bitstrm->pv_codec_handle;
        WORD16 ai2_dc_coef[4];
        UWORD8 pu1_inv_scan[4] =
 { 0, 1, 2, 3 };
        WORD16 *pi2_coeff_data =
 (WORD16 *)ps_dec->pv_parse_tu_coeff_data;

        ps_tu_4x4 = (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;

        u4_trailing_ones = ((u4_code >> 3) & 0x3);
        u4_total_coeff_tone = (u4_total_coeff << 16) | u4_trailing_ones;
        ih264d_rest_of_residual_cav_chroma_dc_block(u4_total_coeff_tone,
                                                    ps_bitstrm);

        ai2_dc_coef[0] = 0;
        ai2_dc_coef[1] = 0;
        ai2_dc_coef[2] = 0;
        ai2_dc_coef[3] = 0;

        ih264d_unpack_coeff4x4_dc_4x4blk(ps_tu_4x4,
                                         ai2_dc_coef,
                                         pu1_inv_scan);

  
  
  
        i_z0 = (ai2_dc_coef[0] + ai2_dc_coef[2]);
        i_z1 = (ai2_dc_coef[0] - ai2_dc_coef[2]);
        i_z2 = (ai2_dc_coef[1] - ai2_dc_coef[3]);
        i_z3 = (ai2_dc_coef[1] + ai2_dc_coef[3]);

  
  
  
 *pi2_coeff_data++ = ((i_z0 + i_z3) * u4_scale_v) >> 5;
 *pi2_coeff_data++ = ((i_z0 - i_z3) * u4_scale_v) >> 5;
 *pi2_coeff_data++ = ((i_z1 + i_z2) * u4_scale_v) >> 5;
 *pi2_coeff_data++ = ((i_z1 - i_z2) * u4_scale_v) >> 5;

        ps_dec->pv_parse_tu_coeff_data = (void *)pi2_coeff_data;

        SET_BIT(ps_cur_mb_info->u1_yuv_dc_block_flag,2);
 }
}
