IHEVCD_ERROR_T ihevcd_short_term_ref_pic_set(bitstrm_t *ps_bitstrm,
 stref_picset_t *ps_stref_picset_base,
                                             WORD32 num_short_term_ref_pic_sets,
                                             WORD32 idx,
 stref_picset_t *ps_stref_picset)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 value;
 stref_picset_t *ps_stref_picset_ref;
    WORD32 delta_idx, delta_rps;
    WORD32 r_idx;
    WORD32 i;
    WORD32 j, k, temp;
 if(idx > 0)
 {
        BITS_PARSE("inter_ref_pic_set_prediction_flag", value, ps_bitstrm, 1);
        ps_stref_picset->i1_inter_ref_pic_set_prediction_flag = value;
 }
 else
        ps_stref_picset->i1_inter_ref_pic_set_prediction_flag = 0;

 if(ps_stref_picset->i1_inter_ref_pic_set_prediction_flag)
 {
        WORD32 delta_rps_sign;
        WORD32 abs_delta_rps;
        WORD32 num_neg_pics = 0;
        WORD32 num_pos_pics = 0;
        WORD32 num_pics = 0;

 if(idx == num_short_term_ref_pic_sets)
 {
            UEV_PARSE("delta_idx_minus1", value, ps_bitstrm);
            delta_idx = value + 1;
 }
 else
 {
            delta_idx = 1;
 }
        r_idx = idx - delta_idx;
        r_idx = CLIP3(r_idx, 0, idx - 1);

        ps_stref_picset_ref = ps_stref_picset_base + r_idx;

        BITS_PARSE("delta_rps_sign", value, ps_bitstrm, 1);
        delta_rps_sign = value;

        UEV_PARSE("abs_delta_rps_minus1", value, ps_bitstrm);
        abs_delta_rps = value + 1;

        delta_rps = (1 - 2 * delta_rps_sign) * (abs_delta_rps);



 for(i = 0; i <= ps_stref_picset_ref->i1_num_delta_pocs; i++)
 {
            WORD32 ref_idc;

  
  
  
  
  
  
            BITS_PARSE("used_by_curr_pic_flag", value, ps_bitstrm, 1);
            ref_idc = value;
            ps_stref_picset->ai1_used[num_pics] = value;
  
 if(0 == ref_idc)
 {
                BITS_PARSE("use_delta_flag", value, ps_bitstrm, 1);
                ref_idc = value << 1;
 }
 if((ref_idc == 1) || (ref_idc == 2))
 {
                WORD32 delta_poc;
                delta_poc = delta_rps;
                delta_poc +=
 ((i < ps_stref_picset_ref->i1_num_delta_pocs) ?
                                ps_stref_picset_ref->ai2_delta_poc[i] :
 0);

                ps_stref_picset->ai2_delta_poc[num_pics] = delta_poc;

 if(delta_poc < 0)
 {
                    num_neg_pics++;
 }
 else
 {
                    num_pos_pics++;
 }
                num_pics++;
 }
            ps_stref_picset->ai1_ref_idc[i] = ref_idc;
 }

        num_neg_pics = CLIP3(num_neg_pics, 0, MAX_DPB_SIZE - 1);
        num_pos_pics = CLIP3(num_pos_pics, 0, (MAX_DPB_SIZE - 1 - num_neg_pics));
        num_pics = num_neg_pics + num_pos_pics;

        ps_stref_picset->i1_num_ref_idc =
                        ps_stref_picset_ref->i1_num_delta_pocs + 1;
        ps_stref_picset->i1_num_delta_pocs = num_pics;
        ps_stref_picset->i1_num_pos_pics = num_pos_pics;
        ps_stref_picset->i1_num_neg_pics = num_neg_pics;


 for(j = 1; j < num_pics; j++)
 {
            WORD32 delta_poc = ps_stref_picset->ai2_delta_poc[j];
            WORD8 i1_used = ps_stref_picset->ai1_used[j];
 for(k = j - 1; k >= 0; k--)
 {
                temp = ps_stref_picset->ai2_delta_poc[k];
 if(delta_poc < temp)
 {
                    ps_stref_picset->ai2_delta_poc[k + 1] = temp;
                    ps_stref_picset->ai1_used[k + 1] = ps_stref_picset->ai1_used[k];
                    ps_stref_picset->ai2_delta_poc[k] = delta_poc;
                    ps_stref_picset->ai1_used[k] = i1_used;
 }
 }
 }
 for(j = 0, k = num_neg_pics - 1; j < num_neg_pics >> 1; j++, k--)
 {
            WORD32 delta_poc = ps_stref_picset->ai2_delta_poc[j];
            WORD8 i1_used = ps_stref_picset->ai1_used[j];
            ps_stref_picset->ai2_delta_poc[j] = ps_stref_picset->ai2_delta_poc[k];
            ps_stref_picset->ai1_used[j] = ps_stref_picset->ai1_used[k];
            ps_stref_picset->ai2_delta_poc[k] = delta_poc;
            ps_stref_picset->ai1_used[k] = i1_used;
 }

 }
 else
 {
        WORD32 prev_poc = 0;
        WORD32 poc;

        UEV_PARSE("num_negative_pics", value, ps_bitstrm);
        ps_stref_picset->i1_num_neg_pics = value;
        ps_stref_picset->i1_num_neg_pics = CLIP3(ps_stref_picset->i1_num_neg_pics,
 0,
                                                 MAX_DPB_SIZE - 1);

        UEV_PARSE("num_positive_pics", value, ps_bitstrm);
        ps_stref_picset->i1_num_pos_pics = value;
        ps_stref_picset->i1_num_pos_pics = CLIP3(ps_stref_picset->i1_num_pos_pics,
 0,
 (MAX_DPB_SIZE - 1 - ps_stref_picset->i1_num_neg_pics));

        ps_stref_picset->i1_num_delta_pocs =
                        ps_stref_picset->i1_num_neg_pics +
                        ps_stref_picset->i1_num_pos_pics;


 for(i = 0; i < ps_stref_picset->i1_num_neg_pics; i++)
 {
            UEV_PARSE("delta_poc_s0_minus1", value, ps_bitstrm);
            poc = prev_poc - (value + 1);
            prev_poc = poc;
            ps_stref_picset->ai2_delta_poc[i] = poc;

            BITS_PARSE("used_by_curr_pic_s0_flag", value, ps_bitstrm, 1);
            ps_stref_picset->ai1_used[i] = value;

 }
        prev_poc = 0;
 for(i = ps_stref_picset->i1_num_neg_pics;
                        i < ps_stref_picset->i1_num_delta_pocs;
                        i++)
 {
            UEV_PARSE("delta_poc_s1_minus1", value, ps_bitstrm);
            poc = prev_poc + (value + 1);
            prev_poc = poc;
            ps_stref_picset->ai2_delta_poc[i] = poc;

            BITS_PARSE("used_by_curr_pic_s1_flag", value, ps_bitstrm, 1);
            ps_stref_picset->ai1_used[i] = value;

 }

 }

 return ret;
}
