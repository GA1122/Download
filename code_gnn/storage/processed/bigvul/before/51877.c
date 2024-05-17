dissect_macd_pdu_data(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                      int offset, guint16 length, guint16 number_of_pdus,
                      struct fp_info *p_fp_info, void *data)
{
    int         pdu;
    int         bit_offset = 0;
    proto_item *pdus_ti    = NULL;
    proto_tree *data_tree  = NULL;
    gboolean    dissected  = FALSE;

     
    if (tree) {
        pdus_ti =  proto_tree_add_item(tree, hf_fp_data, tvb, offset, -1, ENC_NA);
        proto_item_set_text(pdus_ti, "%u MAC-d PDUs of %u bits", number_of_pdus, length);
        data_tree = proto_item_add_subtree(pdus_ti, ett_fp_data);
    }

     
    for (pdu=0; pdu < number_of_pdus; pdu++) {
        proto_item *pdu_ti;

        if (data_tree) {
             
            proto_tree_add_item(data_tree, hf_fp_hsdsch_data_padding, tvb, offset+(bit_offset/8), 1, ENC_BIG_ENDIAN);

        }
        bit_offset += 4;

         
        if (data_tree) {
            pinfo->fd->subnum = pdu;  
            p_fp_info->cur_tb = pdu;     
            pdu_ti = proto_tree_add_item(data_tree, hf_fp_mac_d_pdu, tvb,
                                         offset + (bit_offset/8),
                                         ((bit_offset % 8) + length + 7) / 8,
                                         ENC_NA);
            proto_item_set_text(pdu_ti, "MAC-d PDU (PDU %u)", pdu+1);
        }
        if (preferences_call_mac_dissectors  ) {
            tvbuff_t *next_tvb;
            next_tvb = tvb_new_subset(tvb, offset + bit_offset/8,
                                      ((bit_offset % 8) + length + 7)/8, -1);
            call_dissector_with_data(mac_fdd_hsdsch_handle, next_tvb, pinfo, top_level_tree, data);
            dissected = TRUE;
        }

         
        bit_offset += length;

         
        if (bit_offset % 8) {
            bit_offset += (8 - (bit_offset % 8));
        }
    }

     
    proto_item_set_len(pdus_ti, bit_offset/8);

     
    offset += (bit_offset / 8);

     
    if (dissected == FALSE) {
        col_append_fstr(pinfo->cinfo, COL_INFO, "   %u PDUs of %u bits",
                        number_of_pdus, length);
    }

    return offset;
}
