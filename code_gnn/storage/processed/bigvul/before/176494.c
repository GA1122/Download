void ihevcd_update_function_ptr(codec_t *ps_codec)
{

  
    ps_codec->apf_inter_pred[0] = NULL;
    ps_codec->apf_inter_pred[1] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_copy_fptr;
    ps_codec->apf_inter_pred[2] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_vert_fptr;
    ps_codec->apf_inter_pred[3] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_horz_fptr;
    ps_codec->apf_inter_pred[4] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[5] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_copy_w16out_fptr;
    ps_codec->apf_inter_pred[6] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_vert_w16out_fptr;
    ps_codec->apf_inter_pred[7] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[8] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[9] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_vert_w16inp_fptr;
    ps_codec->apf_inter_pred[10] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_luma_vert_w16inp_w16out_fptr;
    ps_codec->apf_inter_pred[11] = NULL;
    ps_codec->apf_inter_pred[12] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_copy_fptr;
    ps_codec->apf_inter_pred[13] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_vert_fptr;
    ps_codec->apf_inter_pred[14] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_horz_fptr;
    ps_codec->apf_inter_pred[15] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[16] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_copy_w16out_fptr;
    ps_codec->apf_inter_pred[17] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_vert_w16out_fptr;
    ps_codec->apf_inter_pred[18] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[19] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_horz_w16out_fptr;
    ps_codec->apf_inter_pred[20] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_vert_w16inp_fptr;
    ps_codec->apf_inter_pred[21] = (pf_inter_pred)ps_codec->s_func_selector.ihevc_inter_pred_chroma_vert_w16inp_w16out_fptr;

  
    ps_codec->apf_intra_pred_luma[0] = (pf_intra_pred)NULL;
    ps_codec->apf_intra_pred_luma[1] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_planar_fptr;
    ps_codec->apf_intra_pred_luma[2] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_dc_fptr;
    ps_codec->apf_intra_pred_luma[3] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode2_fptr;
    ps_codec->apf_intra_pred_luma[4] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode_3_to_9_fptr;
    ps_codec->apf_intra_pred_luma[5] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_horz_fptr;
    ps_codec->apf_intra_pred_luma[6] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode_11_to_17_fptr;
    ps_codec->apf_intra_pred_luma[7] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode_18_34_fptr;
    ps_codec->apf_intra_pred_luma[8] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode_19_to_25_fptr;
    ps_codec->apf_intra_pred_luma[9] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_ver_fptr;
    ps_codec->apf_intra_pred_luma[10] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_luma_mode_27_to_33_fptr;

    ps_codec->apf_intra_pred_chroma[0] = (pf_intra_pred)NULL;
    ps_codec->apf_intra_pred_chroma[1] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_planar_fptr;
    ps_codec->apf_intra_pred_chroma[2] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_dc_fptr;
    ps_codec->apf_intra_pred_chroma[3] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode2_fptr;
    ps_codec->apf_intra_pred_chroma[4] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode_3_to_9_fptr;
    ps_codec->apf_intra_pred_chroma[5] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_horz_fptr;
    ps_codec->apf_intra_pred_chroma[6] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode_11_to_17_fptr;
    ps_codec->apf_intra_pred_chroma[7] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode_18_34_fptr;
    ps_codec->apf_intra_pred_chroma[8] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode_19_to_25_fptr;
    ps_codec->apf_intra_pred_chroma[9] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_ver_fptr;
    ps_codec->apf_intra_pred_chroma[10] = (pf_intra_pred)ps_codec->s_func_selector.ihevc_intra_pred_chroma_mode_27_to_33_fptr;

  
    ps_codec->apf_itrans_recon[0] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_itrans_recon_4x4_ttype1_fptr;
    ps_codec->apf_itrans_recon[1] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_itrans_recon_4x4_fptr;
    ps_codec->apf_itrans_recon[2] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_itrans_recon_8x8_fptr;
    ps_codec->apf_itrans_recon[3] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_itrans_recon_16x16_fptr;
    ps_codec->apf_itrans_recon[4] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_itrans_recon_32x32_fptr;
    ps_codec->apf_itrans_recon[5] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_chroma_itrans_recon_4x4_fptr;
    ps_codec->apf_itrans_recon[6] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_chroma_itrans_recon_8x8_fptr;
    ps_codec->apf_itrans_recon[7] = (pf_itrans_recon)ps_codec->s_func_selector.ihevc_chroma_itrans_recon_16x16_fptr;

  
    ps_codec->apf_recon[0] = (pf_recon)ps_codec->s_func_selector.ihevc_recon_4x4_ttype1_fptr;
    ps_codec->apf_recon[1] = (pf_recon)ps_codec->s_func_selector.ihevc_recon_4x4_fptr;
    ps_codec->apf_recon[2] = (pf_recon)ps_codec->s_func_selector.ihevc_recon_8x8_fptr;
    ps_codec->apf_recon[3] = (pf_recon)ps_codec->s_func_selector.ihevc_recon_16x16_fptr;
    ps_codec->apf_recon[4] = (pf_recon)ps_codec->s_func_selector.ihevc_recon_32x32_fptr;
    ps_codec->apf_recon[5] = (pf_recon)ps_codec->s_func_selector.ihevc_chroma_recon_4x4_fptr;
    ps_codec->apf_recon[6] = (pf_recon)ps_codec->s_func_selector.ihevc_chroma_recon_8x8_fptr;
    ps_codec->apf_recon[7] = (pf_recon)ps_codec->s_func_selector.ihevc_chroma_recon_16x16_fptr;

  
    ps_codec->apf_itrans_recon_dc[0] = (pf_itrans_recon_dc)ps_codec->s_func_selector.ihevcd_itrans_recon_dc_luma_fptr;
    ps_codec->apf_itrans_recon_dc[1] = (pf_itrans_recon_dc)ps_codec->s_func_selector.ihevcd_itrans_recon_dc_chroma_fptr;

  
    ps_codec->apf_sao_luma[0] = (pf_sao_luma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class0_fptr;
    ps_codec->apf_sao_luma[1] = (pf_sao_luma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class1_fptr;
    ps_codec->apf_sao_luma[2] = (pf_sao_luma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class2_fptr;
    ps_codec->apf_sao_luma[3] = (pf_sao_luma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class3_fptr;

    ps_codec->apf_sao_chroma[0] = (pf_sao_chroma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class0_chroma_fptr;
    ps_codec->apf_sao_chroma[1] = (pf_sao_chroma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class1_chroma_fptr;
    ps_codec->apf_sao_chroma[2] = (pf_sao_chroma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class2_chroma_fptr;
    ps_codec->apf_sao_chroma[3] = (pf_sao_chroma)ps_codec->s_func_selector.ihevc_sao_edge_offset_class3_chroma_fptr;
}
