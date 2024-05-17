dissect_e_dch_channel_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                           int offset, struct fp_info *p_fp_info,
                           gboolean is_common, rlc_info  *rlcinf,
                           void *data)
{
    gboolean is_control_frame;
    guint8   number_of_subframes;
    guint8   cfn;
    int      n;
    struct   edch_t1_subframe_info subframes[16];
    guint16 header_crc = 0;
    proto_item * header_crc_pi = NULL;
    guint header_length = 0;

    if (p_fp_info->edch_type == 1) {
        col_append_str(pinfo->cinfo, COL_INFO, " (T2)");
    }

     
      
    header_crc = (tvb_get_bits8(tvb, offset*8, 7) << 4) + tvb_get_bits8(tvb, offset*8+8, 4);

     
    is_control_frame = tvb_get_guint8(tvb, offset) & 0x01;

    col_append_str(pinfo->cinfo, COL_INFO, is_control_frame ? " [Control] " : " [Data] ");

    if (is_control_frame) {
         

         
        header_crc = tvb_get_bits8(tvb, 0, 7);
        header_crc_pi = proto_tree_add_item(tree, hf_fp_header_crc, tvb, 0, 1, ENC_BIG_ENDIAN);
        proto_tree_add_item(tree, hf_fp_ft, tvb, 0, 1, ENC_BIG_ENDIAN);
        offset++;
        if (preferences_header_checksum) {
            verify_control_frame_crc(tvb, pinfo, header_crc_pi, header_crc);
        }
        dissect_dch_control_frame(tree, pinfo, tvb, offset, p_fp_info);
    }
    else {
         
         
        guint  bit_offset = 0;
        guint  total_pdus = 0;
        guint  total_bits = 0;
        gboolean dissected = FALSE;

        header_crc_pi = proto_tree_add_uint_format(tree, hf_fp_edch_header_crc, tvb,
                offset, 2, header_crc,
                "%u%u%u%u%u%u%u.%u%u%u%u.... = E-DCH Header CRC: 0x%x",
                (header_crc >> 10) & 1,
                (header_crc >> 9) & 1,
                (header_crc >> 8) & 1,
                (header_crc >> 7) & 1,
                (header_crc >> 6) & 1,
                (header_crc >> 5) & 1,
                (header_crc >> 4) & 1,
                (header_crc >> 3) & 1,
                (header_crc >> 2) & 1,
                (header_crc >> 1) & 1,
                (header_crc >> 0) & 1, header_crc);
        proto_tree_add_item(tree, hf_fp_ft, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;
         
        proto_tree_add_item(tree, hf_fp_edch_fsn, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;

         
        if ((p_fp_info->release >= 6) &&
            ((p_fp_info->release_year > 2005) ||
             ((p_fp_info->release_year == 2005) && (p_fp_info->release_month >= 9)))) {

             
            number_of_subframes = (tvb_get_guint8(tvb, offset) & 0x0f) + 1;
        }
        else {
             
            number_of_subframes = (tvb_get_guint8(tvb, offset) & 0x07);
        }
        proto_tree_add_uint(tree, hf_fp_edch_number_of_subframes, tvb, offset, 1,
                            number_of_subframes);

        offset++;

         
        cfn = tvb_get_guint8(tvb, offset);
        proto_tree_add_item(tree, hf_fp_cfn, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;

         
        if (p_fp_info->edch_type == 1) {
            dissect_e_dch_t2_or_common_channel_info(tvb, pinfo, tree, offset, p_fp_info,
                                                    number_of_subframes,
                                                    is_common, header_crc,
                                                    header_crc_pi, data);
            return;
        }

         
        for (n=0; n < number_of_subframes; n++) {
            int i;
            int start_offset = offset;
            proto_item *subframe_header_ti;
            proto_tree *subframe_header_tree;

             
            subframe_header_ti = proto_tree_add_string_format(tree, hf_fp_edch_subframe_header, tvb, offset, 0,
                                                              "", "Subframe");
            subframe_header_tree = proto_item_add_subtree(subframe_header_ti, ett_fp_edch_subframe_header);

             
            proto_tree_add_item(subframe_header_tree, hf_fp_edch_harq_retransmissions, tvb,
                                offset, 1, ENC_BIG_ENDIAN);

             
            subframes[n].subframe_number = (tvb_get_guint8(tvb, offset) & 0x07);
            proto_tree_add_bits_item(subframe_header_tree, hf_fp_edch_subframe_number, tvb,
                                     offset*8+5, 1, ENC_BIG_ENDIAN);
            offset++;

             
            subframes[n].number_of_mac_es_pdus = (tvb_get_guint8(tvb, offset) & 0xf0) >> 4;
            proto_tree_add_item(subframe_header_tree, hf_fp_edch_number_of_mac_es_pdus,
                                tvb, offset, 1, ENC_BIG_ENDIAN);
            bit_offset = 4;

            proto_item_append_text(subframe_header_ti, " %u header (%u MAC-es PDUs)",
                                   subframes[n].subframe_number,
                                   subframes[n].number_of_mac_es_pdus);

             
            for (i=0; i < subframes[n].number_of_mac_es_pdus; i++) {
                guint64 ddi;
                guint64 n_pdus;     

                proto_item *ddi_ti;
                gint ddi_size = -1;
                int     p;

                 
                ddi_ti = proto_tree_add_bits_ret_val(subframe_header_tree, hf_fp_edch_ddi, tvb,
                                                     offset*8 + bit_offset, 6, &ddi, ENC_BIG_ENDIAN);

                if (rlcinf) {
                    rlcinf->rbid[i] = (guint8)ddi;
                }
                 
                 
                 
                for (p=0; p < p_fp_info->no_ddi_entries; p++) {
                    if (ddi == p_fp_info->edch_ddi[p]) {
                        ddi_size = p_fp_info->edch_macd_pdu_size[p];

                        break;
                    }
                }

                if (ddi_size == -1) {
                    expert_add_info_format(pinfo, ddi_ti, &ei_fp_ddi_not_defined, "DDI %u not defined for this UE!", (guint)ddi);
                    return;
                }
                else {
                    proto_item_append_text(ddi_ti, " (%d bits)", ddi_size);
                }

                subframes[n].ddi[i] = (guint8)ddi;
                bit_offset += 6;

                 
                proto_tree_add_bits_ret_val(subframe_header_tree, hf_fp_edch_number_of_mac_d_pdus, tvb,
                                            offset*8 + bit_offset, 6, &n_pdus, ENC_BIG_ENDIAN);

                subframes[n].number_of_mac_d_pdus[i] = (guint8)n_pdus;
                bit_offset += 6;
            }

            offset += ((bit_offset+7)/8);

             
            proto_item_set_len(subframe_header_ti, offset - start_offset);
        }
        header_length = offset;
         
        for (n=0; n < number_of_subframes; n++) {
            int i;
            proto_item *subframe_ti;
            proto_tree *subframe_tree;
            guint bits_in_subframe = 0;
            guint mac_d_pdus_in_subframe = 0;
            guint    lchid=0;     
            umts_mac_info *macinf;
            bit_offset = 0;

            macinf = (umts_mac_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_umts_mac, 0);
             
            subframe_ti = proto_tree_add_string_format(tree, hf_fp_edch_subframe, tvb, offset, 0,
                                                       "", "Subframe %u data", subframes[n].subframe_number);
            subframe_tree = proto_item_add_subtree(subframe_ti, ett_fp_edch_subframe);

            for (i=0; i < subframes[n].number_of_mac_es_pdus; i++) {
                int         m;
                guint16     size = 0;
                 
                guint       send_size;
                proto_item  *ti;
                int         macd_idx;
                proto_tree  *maces_tree = NULL;

                 
                 
                for (m=0; m < p_fp_info->no_ddi_entries; m++) {
                    if (subframes[n].ddi[i] == p_fp_info->edch_ddi[m]) {
                        size = p_fp_info->edch_macd_pdu_size[m];
                        break;
                    }
                }
                 
                for (m=0; m < p_fp_info->no_ddi_entries; m++) {
                    if (subframes[n].ddi[i] == p_fp_info->edch_ddi[m]) {
                        lchid = p_fp_info->edch_lchId[m];
                        break;
                    }
                }

                if (m == p_fp_info->no_ddi_entries) {
                     
                    expert_add_info(pinfo, NULL, &ei_fp_unable_to_locate_ddi_entry);
                    return;
                }

                 
                send_size = size * subframes[n].number_of_mac_d_pdus[i];

                 
                proto_tree_add_item(subframe_tree, hf_fp_edch_pdu_padding, tvb,
                                    offset + (bit_offset/8),
                                    1, ENC_BIG_ENDIAN);
                bit_offset += 2;

                 
                 
                proto_tree_add_item(subframe_tree, hf_fp_edch_tsn, tvb,
                                    offset + (bit_offset/8),
                                    1, ENC_BIG_ENDIAN);
                bit_offset += 6;

                 
                if (subframe_tree) {
                    ti = proto_tree_add_item(subframe_tree, hf_fp_edch_mac_es_pdu, tvb,
                                             offset + (bit_offset/8),
                                             ((bit_offset % 8) + send_size + 7) / 8,
                                             ENC_NA);
                    proto_item_append_text(ti, " (%u * %u = %u bits, PDU %d)",
                                           size, subframes[n].number_of_mac_d_pdus[i],
                                           send_size, n);
                    maces_tree = proto_item_add_subtree(ti, ett_fp_edch_maces);
                }
                for (macd_idx = 0; macd_idx < subframes[n].number_of_mac_d_pdus[i]; macd_idx++) {

                    if (preferences_call_mac_dissectors  ) {
                        tvbuff_t *next_tvb;
                        pinfo->fd->subnum = macd_idx;  
                         
                        next_tvb = tvb_new_subset(tvb, offset + bit_offset/8,
                                ((bit_offset % 8) + size + 7) / 8, -1);


                         
                         
                        macinf->content[macd_idx] = lchId_type_table[lchid];      
                        macinf->lchid[macd_idx] = lchid;
                        rlcinf->mode[macd_idx] = lchId_rlc_map[lchid];  

                         
                        rlcinf->urnti[macd_idx] = p_fp_info->com_context_id;
                        rlcinf->rbid[macd_idx] = lchid;       
                         
                         
                        rlcinf->li_size[macd_idx] = RLC_LI_7BITS;

#if 0
                         
                        if ( rrc_ciph_inf && g_tree_lookup(rrc_ciph_inf, GINT_TO_POINTER((gint)p_fp_info->com_context_id)) ) {
                            rlcinf->ciphered[macd_idx] = TRUE;
                        } else {
                            rlcinf->ciphered[macd_idx] = FALSE;
                        }
#endif
                        rlcinf->ciphered[macd_idx] = FALSE;
                        rlcinf->deciphered[macd_idx] = FALSE;
                        p_fp_info->cur_tb = macd_idx;     

                         
                        call_dissector_with_data(mac_fdd_edch_handle, next_tvb, pinfo, top_level_tree, data);
                        dissected = TRUE;
                    }
                    else {
                         
                        proto_tree_add_item(maces_tree, hf_fp_mac_d_pdu, tvb,
                                            offset + (bit_offset/8),
                                            ((bit_offset % 8) + size + 7) / 8,
                                            ENC_NA);
                    }
                    bit_offset += size;
                }

                bits_in_subframe += send_size;
                mac_d_pdus_in_subframe += subframes[n].number_of_mac_d_pdus[i];

                 
                if (bit_offset % 8) {
                    bit_offset += (8 - (bit_offset % 8));
                }
            }

            if (tree) {
                 
                proto_item_set_len(subframe_ti, bit_offset/8);
                 
                proto_item_append_text(subframe_ti, " (%u bits in %u MAC-d PDUs)",
                                       bits_in_subframe, mac_d_pdus_in_subframe);
            }
            total_pdus += mac_d_pdus_in_subframe;
            total_bits += bits_in_subframe;

            offset += (bit_offset/8);
        }

         
        if (dissected == FALSE) {
            col_append_fstr(pinfo->cinfo, COL_INFO,
                            " CFN = %03u   (%u bits in %u pdus in %u subframes)",
                            cfn, total_bits, total_pdus, number_of_subframes);
        }
         
         
        if (preferences_header_checksum) {
            verify_header_crc_edch(tvb, pinfo, header_crc_pi, header_crc, header_length);
        }
         
        dissect_spare_extension_and_crc(tvb, pinfo, tree,
                                        p_fp_info->dch_crc_present, offset, header_length);
    }
}