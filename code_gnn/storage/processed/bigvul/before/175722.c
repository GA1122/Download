WORD32 ih264d_init_dec_mb_grp(dec_struct_t *ps_dec)
{
 dec_seq_params_t *ps_seq = ps_dec->ps_cur_sps;
    UWORD8 u1_frm = ps_seq->u1_frame_mbs_only_flag;

    ps_dec->u1_recon_mb_grp = ps_dec->u2_frm_wd_in_mbs << ps_seq->u1_mb_aff_flag;

    ps_dec->u1_recon_mb_grp_pair = ps_dec->u1_recon_mb_grp >> 1;

 if(!ps_dec->u1_recon_mb_grp)
 {
 return ERROR_MB_GROUP_ASSGN_T;
 }

    ps_dec->u4_num_mbs_prev_nmb = ps_dec->u1_recon_mb_grp;

 return OK;
}
