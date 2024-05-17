UWORD8 ih264d_is_sps_changed(prev_seq_params_t * ps_prv,
 dec_seq_params_t * ps_cur)
{

 if((ps_prv->u2_frm_wd_in_mbs != ps_cur->u2_frm_wd_in_mbs)
 || (ps_prv->u1_level_idc != ps_cur->u1_level_idc)
 || (ps_prv->u1_profile_idc != ps_cur->u1_profile_idc)
 || (ps_cur->u2_frm_ht_in_mbs != ps_prv->u2_frm_ht_in_mbs)
 || (ps_cur->u1_frame_mbs_only_flag
 != ps_prv->u1_frame_mbs_only_flag)
 || (ps_cur->u1_direct_8x8_inference_flag
 != ps_prv->u1_direct_8x8_inference_flag))
 return 1;

 return 0;
}
