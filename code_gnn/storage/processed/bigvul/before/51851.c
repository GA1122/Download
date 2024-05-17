dissect_crci_bits(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                  fp_info *p_fp_info, int offset)
{
    int         n, num_tbs;
    proto_item *ti         = NULL;
    proto_tree *crcis_tree = NULL;
    guint       errors     = 0;

    num_tbs = get_tb_count(p_fp_info);


     
    if (tree) {
        ti =  proto_tree_add_item(tree, hf_fp_crcis, tvb, offset, (num_tbs+7)/8, ENC_NA);
        proto_item_set_text(ti, "CRCI bits for %u tbs", num_tbs);
        crcis_tree = proto_item_add_subtree(ti, ett_fp_crcis);
    }

     
    for (n=0; n < num_tbs; n++) {
        int bit = (tvb_get_guint8(tvb, offset+(n/8)) >> (7-(n%8))) & 0x01;
        proto_tree_add_item(crcis_tree, hf_fp_crci[n%8], tvb, offset+(n/8),
                            1, ENC_BIG_ENDIAN);

        if (bit == 1) {
            errors++;
            expert_add_info(pinfo, ti, &ei_fp_crci_error_bit_set_for_tb);
        }
    }

    if (tree) {
         
        proto_item_set_len(ti, (num_tbs+7) / 8);

         
        proto_item_append_text(ti, " (%u errors)", errors);
    }

    offset += ((num_tbs+7) / 8);
    return offset;
}
