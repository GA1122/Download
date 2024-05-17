WORD32 ih264d_update_qp(dec_struct_t * ps_dec, const WORD8 i1_qp)
{
    WORD32 i_temp;
    i_temp = (ps_dec->u1_qp + i1_qp + 52) % 52;

 if((i_temp < 0) || (i_temp > 51) || (i1_qp < -26) || (i1_qp > 25))
 return ERROR_INV_RANGE_QP_T;

    ps_dec->u1_qp = i_temp;
    ps_dec->u1_qp_y_rem6 = ps_dec->u1_qp % 6;
    ps_dec->u1_qp_y_div6 = ps_dec->u1_qp / 6;
    i_temp = CLIP3(0, 51, ps_dec->u1_qp + ps_dec->ps_cur_pps->i1_chroma_qp_index_offset);
    ps_dec->u1_qp_u_rem6 = MOD(gau1_ih264d_qp_scale_cr[12 + i_temp], 6);
    ps_dec->u1_qp_u_div6 = DIV(gau1_ih264d_qp_scale_cr[12 + i_temp], 6);

    i_temp = CLIP3(0, 51, ps_dec->u1_qp + ps_dec->ps_cur_pps->i1_second_chroma_qp_index_offset);
    ps_dec->u1_qp_v_rem6 = MOD(gau1_ih264d_qp_scale_cr[12 + i_temp], 6);
    ps_dec->u1_qp_v_div6 = DIV(gau1_ih264d_qp_scale_cr[12 + i_temp], 6);

    ps_dec->pu2_quant_scale_y =
                    gau2_ih264_iquant_scale_4x4[ps_dec->u1_qp_y_rem6];
    ps_dec->pu2_quant_scale_u =
                    gau2_ih264_iquant_scale_4x4[ps_dec->u1_qp_u_rem6];
    ps_dec->pu2_quant_scale_v =
                    gau2_ih264_iquant_scale_4x4[ps_dec->u1_qp_v_rem6];
 return OK;
}
