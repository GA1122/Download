WORD32 ihevcd_set_processor(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 ihevcd_cxa_ctl_set_processor_ip_t *ps_ip;
 ihevcd_cxa_ctl_set_processor_op_t *ps_op;
 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;

    ps_ip = (ihevcd_cxa_ctl_set_processor_ip_t *)pv_api_ip;
    ps_op = (ihevcd_cxa_ctl_set_processor_op_t *)pv_api_op;

    ps_codec->e_processor_arch = (IVD_ARCH_T)ps_ip->u4_arch;
    ps_codec->e_processor_soc = (IVD_SOC_T)ps_ip->u4_soc;

    ihevcd_init_function_ptr(ps_codec);

    ihevcd_update_function_ptr(ps_codec);

 if(ps_codec->e_processor_soc && (ps_codec->e_processor_soc <= SOC_HISI_37X))
 {
  
 if(ps_codec->e_processor_soc & 0x80)
            ps_codec->u4_enable_fmt_conv_ahead = 1;

  
        ps_codec->e_processor_soc &= 0x7F;

 if(ps_codec->e_processor_soc)
            ps_codec->u4_nctb = ps_codec->e_processor_soc;


 }

 if((ps_codec->e_processor_soc == SOC_HISI_37X) && (ps_codec->i4_num_cores == 2))
 {
        ps_codec->u4_nctb = 2;
 }


    ps_op->u4_error_code = 0;
 return IV_SUCCESS;
}
