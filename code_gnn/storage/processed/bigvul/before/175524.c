static WORD32 ihevcd_parse_sub_layer_hrd_parameters(bitstrm_t *ps_bitstrm,
 sub_lyr_hrd_params_t *ps_sub_layer_hrd_params,
                                                    WORD32 cpb_cnt,
                                                    WORD32 sub_pic_cpb_params_present_flag)
{
    WORD32 ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 i;

 for(i = 0; i <= cpb_cnt; i++)
 {
        UEV_PARSE("bit_rate_value_minus1[ i ]", ps_sub_layer_hrd_params->au4_bit_rate_value_minus1[i], ps_bitstrm);
        UEV_PARSE("cpb_size_value_minus1[ i ]", ps_sub_layer_hrd_params->au4_cpb_size_value_minus1[i], ps_bitstrm);

 if(sub_pic_cpb_params_present_flag)
 {
            UEV_PARSE("cpb_size_du_value_minus1[ i ]", ps_sub_layer_hrd_params->au4_cpb_size_du_value_minus1[i], ps_bitstrm);
            UEV_PARSE("bit_rate_du_value_minus1[ i ]", ps_sub_layer_hrd_params->au4_bit_rate_du_value_minus1[i], ps_bitstrm);
 }
        BITS_PARSE("cbr_flag[ i ]", ps_sub_layer_hrd_params->au1_cbr_flag[i], ps_bitstrm, 1);
 }

 return ret;
}
