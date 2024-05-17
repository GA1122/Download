WORD32 ihevcd_ref_pic_list_modification(bitstrm_t *ps_bitstrm,
 slice_header_t *ps_slice_hdr,
                                        WORD32 num_poc_total_curr)
{
    WORD32 ret = IHEVCD_SUCCESS;
    WORD32 value;
    WORD32 i;
 rplm_t *ps_rplm;
    WORD32 num_bits_list_entry;

    ps_rplm = &(ps_slice_hdr->s_rplm);

  
 {
        num_bits_list_entry = 32 - CLZ(num_poc_total_curr);
  
 if(0 == (num_poc_total_curr & (num_poc_total_curr - 1)))
 {
            num_bits_list_entry--;
 }
 }

 if(ps_slice_hdr->i1_slice_type  == PSLICE || ps_slice_hdr->i1_slice_type  == BSLICE)
 {
        BITS_PARSE("ref_pic_list_modification_flag_l0", value, ps_bitstrm, 1);
        ps_rplm->i1_ref_pic_list_modification_flag_l0 = value;

 if(ps_rplm->i1_ref_pic_list_modification_flag_l0)
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l0_active; i++)
 {
                BITS_PARSE("list_entry_l0", value, ps_bitstrm, num_bits_list_entry);
                ps_rplm->i1_list_entry_l0[i] = value;

                ps_rplm->i1_list_entry_l0[i] = CLIP3(ps_rplm->i1_list_entry_l0[i], 0, num_poc_total_curr - 1);
 }
 }

 if(ps_slice_hdr->i1_slice_type  == BSLICE)
 {
        BITS_PARSE("ref_pic_list_modification_flag_l1", value, ps_bitstrm, 1);
        ps_rplm->i1_ref_pic_list_modification_flag_l1 = value;

 if(ps_rplm->i1_ref_pic_list_modification_flag_l1)
 for(i = 0; i < ps_slice_hdr->i1_num_ref_idx_l1_active; i++)
 {
                BITS_PARSE("list_entry_l1", value, ps_bitstrm, num_bits_list_entry);
                ps_rplm->i1_list_entry_l1[i] = value;

                ps_rplm->i1_list_entry_l1[i] = CLIP3(ps_rplm->i1_list_entry_l1[i], 0, num_poc_total_curr - 1);
 }

 }

 return ret;
}
