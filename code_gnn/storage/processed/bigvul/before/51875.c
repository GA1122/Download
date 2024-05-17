dissect_hsdsch_type_2_channel_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                                   int offset, struct fp_info *p_fp_info,
                                   void *data)
{
    gboolean is_control_frame;
    guint16 header_crc = 0;
    proto_item * header_crc_pi = NULL;
    guint16 header_length = 0;

     
    header_crc = tvb_get_bits8(tvb, 0, 7);
    header_crc_pi = proto_tree_add_item(tree, hf_fp_header_crc, tvb, offset, 1, ENC_BIG_ENDIAN);

     
    is_control_frame = tvb_get_guint8(tvb, offset) & 0x01;
    proto_tree_add_item(tree, hf_fp_ft, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset++;

    col_append_str(pinfo->cinfo, COL_INFO, is_control_frame ? " [Control] " : " [Data] ");

    if (is_control_frame) {
        dissect_common_control(tvb, pinfo, tree, offset, p_fp_info);
         
        if (preferences_header_checksum) {
            verify_control_frame_crc(tvb, pinfo, header_crc_pi, header_crc);
        }
    }
    else {
        guint8 number_of_pdu_blocks;
        gboolean drt_present = FALSE;
        gboolean fach_present = FALSE;
        guint16 user_buffer_size;
        int n;
        guint j;

        #define MAX_PDU_BLOCKS 31
        guint64 lchid[MAX_PDU_BLOCKS];
        guint64 pdu_length[MAX_PDU_BLOCKS];
        guint64 no_of_pdus[MAX_PDU_BLOCKS];

        umts_mac_info *macinf;
        rlc_info *rlcinf;

        rlcinf = (rlc_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0);
        macinf = (umts_mac_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0);
         
         

        col_append_str(pinfo->cinfo, COL_INFO, "(ehs)");

         
        if ((p_fp_info->release == 6) ||
            (p_fp_info->release == 7)) {

            guint8 frame_seq_no = (tvb_get_guint8(tvb, offset) & 0xf0) >> 4;
            proto_tree_add_item(tree, hf_fp_frame_seq_nr, tvb, offset, 1, ENC_BIG_ENDIAN);

            col_append_fstr(pinfo->cinfo, COL_INFO, "  seqno=%u", frame_seq_no);
        }

         
        proto_tree_add_item(tree, hf_fp_cmch_pi, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;

         
        number_of_pdu_blocks = (tvb_get_guint8(tvb, offset) >> 3);
        proto_tree_add_item(tree, hf_fp_total_pdu_blocks, tvb, offset, 1, ENC_BIG_ENDIAN);

        if (p_fp_info->release == 7) {
             
            proto_tree_add_item(tree, hf_fp_flush, tvb, offset, 1, ENC_BIG_ENDIAN);

             
            proto_tree_add_item(tree, hf_fp_fsn_drt_reset, tvb, offset, 1, ENC_BIG_ENDIAN);

             
            drt_present = tvb_get_guint8(tvb, offset) & 0x01;
            proto_tree_add_item(tree, hf_fp_drt_indicator, tvb, offset, 1, ENC_BIG_ENDIAN);
        }
        offset++;

         
        fach_present = (tvb_get_guint8(tvb, offset) & 0x80) >> 7;
        proto_tree_add_item(tree, hf_fp_fach_indicator, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;

         
        user_buffer_size = tvb_get_ntohs(tvb, offset);
        proto_tree_add_item(tree, hf_fp_user_buffer_size, tvb, offset, 2, ENC_BIG_ENDIAN);
        offset += 2;

        col_append_fstr(pinfo->cinfo, COL_INFO, "  User-Buffer-Size=%u", user_buffer_size);



         
         
        for (n=0; n < number_of_pdu_blocks; n++) {
            proto_item *pdu_block_header_ti;
            proto_tree *pdu_block_header_tree;
            int        block_header_start_offset = offset;

             
            pdu_block_header_ti = proto_tree_add_string_format(tree, hf_fp_hsdsch_pdu_block_header,
                                                               tvb, offset, 0,
                                                               "",
                                                               "PDU Block Header");
            pdu_block_header_tree = proto_item_add_subtree(pdu_block_header_ti,
                                                           ett_fp_hsdsch_pdu_block_header);

             
            proto_tree_add_bits_ret_val(pdu_block_header_tree, hf_fp_pdu_length_in_block, tvb,
                                        (offset*8) + ((n % 2) ? 4 : 0), 11,
                                        &pdu_length[n], ENC_BIG_ENDIAN);
            if ((n % 2) == 0)
                offset++;
            else
                offset += 2;


             
            proto_tree_add_bits_ret_val(pdu_block_header_tree, hf_fp_pdus_in_block, tvb,
                                        (offset*8) + ((n % 2) ? 0 : 4), 4,
                                        &no_of_pdus[n], ENC_BIG_ENDIAN);
            if ((n % 2) == 0) {
                offset++;
            }

             
            proto_tree_add_bits_ret_val(pdu_block_header_tree, hf_fp_lchid, tvb,
                                        (offset*8) + ((n % 2) ? 4 : 0), 4,
                                        &lchid[n], ENC_BIG_ENDIAN);
            if ((n % 2) == 1) {
                offset++;
            }
            else {
                if (n == (number_of_pdu_blocks-1)) {
                     
                    offset++;
                }
            }

             
            proto_item_append_text(pdu_block_header_ti,
                                   " (lch:%u, %u pdus of %u bytes)",
                                   (guint16)lchid[n],
                                   (guint16)no_of_pdus[n],
                                   (guint16)pdu_length[n]);

             
            if (((n % 2) == 0) && (n < (number_of_pdu_blocks-1))) {
                proto_item_set_len(pdu_block_header_ti,
                                   offset - block_header_start_offset+1);
            }
            else {
                proto_item_set_len(pdu_block_header_ti,
                                   offset - block_header_start_offset);
            }
        }

        if (header_length == 0) {
            header_length = offset;
        }
         
         
        if (drt_present) {
             
            proto_tree_add_item(tree, hf_fp_drt, tvb, offset, 2, ENC_BIG_ENDIAN);
            offset += 2;
        }

        if (fach_present) {
             
            proto_tree_add_item(tree, hf_fp_hrnti, tvb, offset, 2, ENC_BIG_ENDIAN);
            offset += 2;

             
            proto_tree_add_item(tree, hf_fp_rach_measurement_result, tvb, offset, 2, ENC_BIG_ENDIAN);
            offset++;
        }


         
         
        for (n=0; n < number_of_pdu_blocks; n++) {
            for (j=0;j<no_of_pdus[n];j++) {

                 
                macinf->content[j] = lchId_type_table[lchid[n]+1];   
                macinf->lchid[j] = (guint8)lchid[n]+1;     
                macinf->macdflow_id[j] = p_fp_info->hsdsch_macflowd_id;
                 
                rlcinf->mode[j] = lchId_rlc_map[lchid[n]+1]; 

                macinf->ctmux[n] = FALSE;

                rlcinf->li_size[j] = RLC_LI_7BITS;

                 
#if 0
                 
                if ( rrc_ciph_inf && g_tree_lookup(rrc_ciph_inf, GINT_TO_POINTER((gint)p_fp_info->com_context_id)) ) {
                    rlcinf->ciphered[j] = TRUE;
                } else {
                    rlcinf->ciphered[j] = FALSE;
                }
#endif
                rlcinf->ciphered[j] = FALSE;
                rlcinf->deciphered[j] = FALSE;
                rlcinf->rbid[j] = (guint8)lchid[n]+1;

                rlcinf->urnti[j] = p_fp_info->com_context_id;     
            }

             
            offset = dissect_macd_pdu_data_type_2(tvb, pinfo, tree, offset,
                                                  (guint16)pdu_length[n],
                                                  (guint16)no_of_pdus[n],
                                                  p_fp_info, data);
        }
        if (preferences_header_checksum) {
            verify_header_crc(tvb, pinfo, header_crc_pi, header_crc, header_length);
        }
         
        dissect_spare_extension_and_crc(tvb, pinfo, tree, 1, offset, header_length);
    }
}
