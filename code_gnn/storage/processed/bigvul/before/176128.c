VOID ixheaacd_esbr_qmfsyn64_winadd(WORD32 *tmp1, WORD32 *tmp2, WORD32 *inp1,
                                   WORD32 *sample_buffer, WORD32 ch_fac) {
  WORD32 k;

 for (k = 0; k < 64; k++) {
    WORD64 syn_out = 0;

    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp1[0 + k], inp1[k + 0]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp1[256 + k], inp1[k + 128]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp1[512 + k], inp1[k + 256]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp1[768 + k], inp1[k + 384]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp1[1024 + k], inp1[k + 512]));

    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp2[128 + k], inp1[k + 64]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp2[384 + k], inp1[k + 192]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp2[640 + k], inp1[k + 320]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp2[896 + k], inp1[k + 448]));
    syn_out =
        ixheaacd_add64(syn_out, ixheaacd_mult64(tmp2[1152 + k], inp1[k + 576]));

    sample_buffer[ch_fac * k] = (WORD32)(syn_out >> 31);
 }
}
