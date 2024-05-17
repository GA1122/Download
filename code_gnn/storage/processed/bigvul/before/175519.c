WORD32 ihevcd_parse_pred_wt_ofst(bitstrm_t *ps_bitstrm,
 sps_t *ps_sps,
 pps_t *ps_pps,
 slice_header_t *ps_slice_hdr)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 value;
    WORD32 i;

 pred_wt_ofst_t *ps_wt_ofst = &ps_slice_hdr->s_wt_ofst;
    UNUSED(ps_pps);

    UEV_PARSE("luma_log2_weight_denom", value, ps_bitstrm);
    ps_wt_ofst->i1_luma_log2_weight_denom = value;

 if(ps_sps->i1_chroma_format_idc != 0)
 {
        SEV_PARSE("delta_chroma_log2_weight_denom", value, ps_bitstrm);
        ps_wt_ofst->i1_chroma_log2_weight_denom = ps_wt_ofst->i1_luma_log2_weight_denom + value;
 }

 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l0_active; i++)
 {
        BITS_PARSE("luma_weight_l0_flag[ i ]", value, ps_bitstrm, 1);
        ps_wt_ofst->i1_luma_weight_l0_flag[i] = value;
 }



 if(ps_sps->i1_chroma_format_idc != 0)
 {
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l0_active; i++)
 {
            BITS_PARSE("chroma_weight_l0_flag[ i ]", value, ps_bitstrm, 1);
            ps_wt_ofst->i1_chroma_weight_l0_flag[i] = value;
 }
 }
 else
 {
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l0_active; i++)
 {
            ps_wt_ofst->i1_chroma_weight_l0_flag[i] = 0;
 }
 }


 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l0_active; i++)
 {
 if(ps_wt_ofst->i1_luma_weight_l0_flag[i])
 {
            SEV_PARSE("delta_luma_weight_l0[ i ]", value, ps_bitstrm);


            ps_wt_ofst->i2_luma_weight_l0[i] = (1 << ps_wt_ofst->i1_luma_log2_weight_denom) + value;

            SEV_PARSE("luma_offset_l0[ i ]", value, ps_bitstrm);
            ps_wt_ofst->i2_luma_offset_l0[i] = value;

 }
 else
 {
            ps_wt_ofst->i2_luma_weight_l0[i] = (1 << ps_wt_ofst->i1_luma_log2_weight_denom);
            ps_wt_ofst->i2_luma_offset_l0[i] = 0;
 }
 if(ps_wt_ofst->i1_chroma_weight_l0_flag[i])
 {
            WORD32 ofst;
            WORD32 shift = (1 << (BIT_DEPTH_CHROMA - 1));
            SEV_PARSE("delta_chroma_weight_l0[ i ][ j ]", value, ps_bitstrm);
            ps_wt_ofst->i2_chroma_weight_l0_cb[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom) + value;


            SEV_PARSE("delta_chroma_offset_l0[ i ][ j ]", value, ps_bitstrm);
            ofst = ((shift * ps_wt_ofst->i2_chroma_weight_l0_cb[i]) >> ps_wt_ofst->i1_chroma_log2_weight_denom);
            ofst = value - ofst + shift;

            ps_wt_ofst->i2_chroma_offset_l0_cb[i] = CLIP_S8(ofst);

            SEV_PARSE("delta_chroma_weight_l0[ i ][ j ]", value, ps_bitstrm);
            ps_wt_ofst->i2_chroma_weight_l0_cr[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom) + value;


            SEV_PARSE("delta_chroma_offset_l0[ i ][ j ]", value, ps_bitstrm);
            ofst = ((shift * ps_wt_ofst->i2_chroma_weight_l0_cr[i]) >> ps_wt_ofst->i1_chroma_log2_weight_denom);
            ofst = value - ofst + shift;

            ps_wt_ofst->i2_chroma_offset_l0_cr[i] = CLIP_S8(ofst);

 }
 else
 {
            ps_wt_ofst->i2_chroma_weight_l0_cb[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom);
            ps_wt_ofst->i2_chroma_weight_l0_cr[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom);

            ps_wt_ofst->i2_chroma_offset_l0_cb[i] = 0;
            ps_wt_ofst->i2_chroma_offset_l0_cr[i] = 0;
 }
 }
 if(BSLICE == ps_slice_hdr->i1_slice_type)
 {
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l1_active; i++)
 {
            BITS_PARSE("luma_weight_l1_flag[ i ]", value, ps_bitstrm, 1);
            ps_wt_ofst->i1_luma_weight_l1_flag[i] = value;
 }

 if(ps_sps->i1_chroma_format_idc != 0)
 {
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l1_active; i++)
 {
                BITS_PARSE("chroma_weight_l1_flag[ i ]", value, ps_bitstrm, 1);
                ps_wt_ofst->i1_chroma_weight_l1_flag[i] = value;
 }
 }
 else
 {
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l1_active; i++)
 {
                ps_wt_ofst->i1_chroma_weight_l1_flag[i] = 0;
 }
 }

 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l1_active; i++)
 {
 if(ps_wt_ofst->i1_luma_weight_l1_flag[i])
 {
                SEV_PARSE("delta_luma_weight_l1[ i ]", value, ps_bitstrm);


                ps_wt_ofst->i2_luma_weight_l1[i] = (1 << ps_wt_ofst->i1_luma_log2_weight_denom) + value;

                SEV_PARSE("luma_offset_l1[ i ]", value, ps_bitstrm);
                ps_wt_ofst->i2_luma_offset_l1[i] = value;

 }
 else
 {
                ps_wt_ofst->i2_luma_weight_l1[i] = (1 << ps_wt_ofst->i1_luma_log2_weight_denom);
                ps_wt_ofst->i2_luma_offset_l1[i] = 0;
 }

 if(ps_wt_ofst->i1_chroma_weight_l1_flag[i])
 {
                WORD32 ofst;
                WORD32 shift = (1 << (BIT_DEPTH_CHROMA - 1));
                SEV_PARSE("delta_chroma_weight_l1[ i ][ j ]", value, ps_bitstrm);
                ps_wt_ofst->i2_chroma_weight_l1_cb[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom) + value;;


                SEV_PARSE("delta_chroma_offset_l1[ i ][ j ]", value, ps_bitstrm);
                ofst = ((shift * ps_wt_ofst->i2_chroma_weight_l1_cb[i]) >> ps_wt_ofst->i1_chroma_log2_weight_denom);
                ofst = value - ofst + shift;

                ps_wt_ofst->i2_chroma_offset_l1_cb[i] = CLIP_S8(ofst);;

                SEV_PARSE("delta_chroma_weight_l1[ i ][ j ]", value, ps_bitstrm);
                ps_wt_ofst->i2_chroma_weight_l1_cr[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom) + value;


                SEV_PARSE("delta_chroma_offset_l1[ i ][ j ]", value, ps_bitstrm);
                ofst = ((shift * ps_wt_ofst->i2_chroma_weight_l1_cr[i]) >> ps_wt_ofst->i1_chroma_log2_weight_denom);
                ofst = value - ofst + shift;

                ps_wt_ofst->i2_chroma_offset_l1_cr[i] = CLIP_S8(ofst);;

 }
 else
 {
                ps_wt_ofst->i2_chroma_weight_l1_cb[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom);
                ps_wt_ofst->i2_chroma_weight_l1_cr[i] = (1 << ps_wt_ofst->i1_chroma_log2_weight_denom);

                ps_wt_ofst->i2_chroma_offset_l1_cb[i] = 0;
                ps_wt_ofst->i2_chroma_offset_l1_cr[i] = 0;

 }
 }
 }
 return ret;
}
