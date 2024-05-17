static FLOAT32 *ixheaacd_map_prot_filter(WORD32 filt_length) {
 switch (filt_length) {
 case 4:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[0];
 break;
 case 8:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[40];
 break;
 case 12:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[120];
 break;
 case 16:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[240];
 break;
 case 20:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[400];
 break;
 case 24:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[600];
 break;
 case 32:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[840];
 break;
 case 40:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[1160];
 break;
 default:
 return (FLOAT32 *)&ixheaacd_sub_samp_qmf_window_coeff[0];
 }
}
