dissect_fp_common(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data)
{
    proto_tree       *fp_tree;
    proto_item       *ti;
    gint              offset = 0;
    struct fp_info   *p_fp_info;
    rlc_info         *rlcinf;
    conversation_t   *p_conv;
    umts_fp_conversation_info_t *p_conv_data = NULL;

     
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FP");

     
    ti = proto_tree_add_item(tree, proto_fp, tvb, offset, -1, ENC_NA);
    fp_tree = proto_item_add_subtree(ti, ett_fp);

    top_level_tree = tree;

     
    p_fp_info = (struct fp_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_fp, 0);

     
    p_conv = (conversation_t *)find_conversation(pinfo->num, &pinfo->net_dst, &pinfo->net_src,
                               pinfo->ptype,
                               pinfo->destport, pinfo->srcport, NO_ADDR_B);


    if (p_conv) {
          
#if 0
         while (p_conv->next != NULL && p_conv->next->setup_frame < pinfo->num) {
            p_conv = p_conv->next;
         }
#endif

        p_conv_data = (umts_fp_conversation_info_t *)conversation_get_proto_data(p_conv, proto_fp);

        if (p_conv_data) {
             
            if (addresses_equal(&(pinfo->net_dst), (&p_conv_data->crnc_address))) {

                proto_item *item= proto_tree_add_uint(fp_tree, hf_fp_ul_setup_frame,
                                                      tvb, 0, 0, p_conv_data->ul_frame_number);

                PROTO_ITEM_SET_GENERATED(item);
                 
                pinfo->link_dir=P2P_DIR_UL;
                if (p_fp_info == NULL) {
                    p_fp_info = fp_set_per_packet_inf_from_conv(p_conv_data, tvb, pinfo, fp_tree);
                }
            }
            else {
                 
                proto_item *item= proto_tree_add_uint(fp_tree, hf_fp_dl_setup_frame,
                                                       tvb, 0, 0, p_conv_data->ul_frame_number);

                PROTO_ITEM_SET_GENERATED(item);
                pinfo->link_dir=P2P_DIR_DL;
                if (p_fp_info == NULL) {
                    p_fp_info = fp_set_per_packet_inf_from_conv(p_conv_data, tvb, pinfo, fp_tree);
                }
            }
        }

    }

    if (pinfo->p2p_dir == P2P_DIR_UNKNOWN) {
        if (pinfo->link_dir == P2P_DIR_UL) {
            pinfo->p2p_dir = P2P_DIR_RECV;
        } else {
            pinfo->p2p_dir = P2P_DIR_SENT;
        }
    }

     
    if (p_fp_info == NULL) {
        proto_tree_add_expert(fp_tree, pinfo, &ei_fp_no_per_frame_info, tvb, offset, -1);
        return 1;
    }

    rlcinf = (rlc_info *)p_get_proto_data(wmem_file_scope(), pinfo, proto_rlc, 0);

     
    if (preferences_show_release_info) {
        proto_item *release_ti;
        proto_tree *release_tree;
        proto_item *temp_ti;

        release_ti = proto_tree_add_item(fp_tree, hf_fp_release, tvb, 0, 0, ENC_NA);
        PROTO_ITEM_SET_GENERATED(release_ti);
        proto_item_append_text(release_ti, " R%u (%d/%d)",
                               p_fp_info->release, p_fp_info->release_year, p_fp_info->release_month);
        release_tree = proto_item_add_subtree(release_ti, ett_fp_release);

        temp_ti = proto_tree_add_uint(release_tree, hf_fp_release_version, tvb, 0, 0, p_fp_info->release);
        PROTO_ITEM_SET_GENERATED(temp_ti);

        temp_ti = proto_tree_add_uint(release_tree, hf_fp_release_year, tvb, 0, 0, p_fp_info->release_year);
        PROTO_ITEM_SET_GENERATED(temp_ti);

        temp_ti = proto_tree_add_uint(release_tree, hf_fp_release_month, tvb, 0, 0, p_fp_info->release_month);
        PROTO_ITEM_SET_GENERATED(temp_ti);
    }

     
    col_set_str(pinfo->cinfo, COL_INFO,
                val_to_str_const(p_fp_info->channel,
                                 channel_type_vals,
                                 "Unknown channel type"));
    if (p_conv_data) {
        int i;
        col_append_fstr(pinfo->cinfo, COL_INFO, "(%u", p_conv_data->dchs_in_flow_list[0]);
        for (i=1; i < p_conv_data->num_dch_in_flow; i++) {
            col_append_fstr(pinfo->cinfo, COL_INFO, ",%u", p_conv_data->dchs_in_flow_list[i]);
        }
        col_append_fstr(pinfo->cinfo, COL_INFO, ") ");
    }
    proto_item_append_text(ti, " (%s)",
                           val_to_str_const(p_fp_info->channel,
                                            channel_type_vals,
                                            "Unknown channel type"));

     
    ti = proto_tree_add_uint(fp_tree, hf_fp_channel_type, tvb, 0, 0, p_fp_info->channel);
    PROTO_ITEM_SET_GENERATED(ti);

     
    if (p_fp_info->release == 7) {
        ti = proto_tree_add_uint(fp_tree, hf_fp_division, tvb, 0, 0, p_fp_info->division);
        PROTO_ITEM_SET_GENERATED(ti);
    }

     
    ti = proto_tree_add_uint(fp_tree, hf_fp_direction, tvb, 0, 0, p_fp_info->is_uplink);
    PROTO_ITEM_SET_GENERATED(ti);

     
    if (p_fp_info->iface_type == IuR_Interface) {
        return 1;
    }

     
    if (p_fp_info->no_ddi_entries > 0) {
        int n;
        proto_item *ddi_config_ti;
        proto_tree *ddi_config_tree;

        ddi_config_ti = proto_tree_add_string_format(fp_tree, hf_fp_ddi_config, tvb, offset, 0,
                                                     "", "DDI Config (");
        PROTO_ITEM_SET_GENERATED(ddi_config_ti);
        ddi_config_tree = proto_item_add_subtree(ddi_config_ti, ett_fp_ddi_config);

         
        for (n=0; n < p_fp_info->no_ddi_entries; n++) {
            proto_item_append_text(ddi_config_ti, "%s%u->%ubits",
                                   (n == 0) ? "" : "  ",
                                   p_fp_info->edch_ddi[n], p_fp_info->edch_macd_pdu_size[n]);
            ti = proto_tree_add_uint(ddi_config_tree, hf_fp_ddi_config_ddi, tvb, 0, 0,
                                p_fp_info->edch_ddi[n]);
            PROTO_ITEM_SET_GENERATED(ti);
            ti = proto_tree_add_uint(ddi_config_tree, hf_fp_ddi_config_macd_pdu_size, tvb, 0, 0,
                                p_fp_info->edch_macd_pdu_size[n]);
            PROTO_ITEM_SET_GENERATED(ti);

        }
        proto_item_append_text(ddi_config_ti, ")");
    }

     
     
    switch (p_fp_info->channel) {
        case CHANNEL_RACH_TDD:
        case CHANNEL_RACH_TDD_128:
        case CHANNEL_RACH_FDD:
            dissect_rach_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info,
                                      data);
            break;
        case CHANNEL_DCH:
            dissect_dch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info,
                                     data);
            break;
        case CHANNEL_FACH_FDD:
        case CHANNEL_FACH_TDD:
            dissect_fach_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info,
                                      data);
            break;
        case CHANNEL_DSCH_FDD:
        case CHANNEL_DSCH_TDD:
            dissect_dsch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info);
            break;
        case CHANNEL_USCH_TDD_128:
        case CHANNEL_USCH_TDD_384:
            dissect_usch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info);
            break;
        case CHANNEL_PCH:
            dissect_pch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info,
                                     data);
            break;
        case CHANNEL_CPCH:
            dissect_cpch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info);
            break;
        case CHANNEL_BCH:
            dissect_bch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info);
            break;
        case CHANNEL_HSDSCH:
             
            if (fp_tree)
            {
                proto_item *entity_ti;
                entity_ti = proto_tree_add_uint(fp_tree, hf_fp_hsdsch_entity,
                                                tvb, 0, 0,
                                                p_fp_info->hsdsch_entity);
                PROTO_ITEM_SET_GENERATED(entity_ti);
            }
            switch (p_fp_info->hsdsch_entity) {
                case entity_not_specified:
                case hs:
                     
                    dissect_hsdsch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info, data);
                    break;
                case ehs:
                    dissect_hsdsch_type_2_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info, data);
                    break;
                default:
                     
                    expert_add_info(pinfo, NULL, &ei_fp_hsdsch_entity_not_specified);
                    break;
            }
            break;
        case CHANNEL_HSDSCH_COMMON:
            expert_add_info(pinfo, NULL, &ei_fp_hsdsch_common_experimental_support);
             
            dissect_hsdsch_common_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info, data);

            break;
        case CHANNEL_HSDSCH_COMMON_T3:
            expert_add_info(pinfo, NULL, &ei_fp_hsdsch_common_t3_not_implemented);

             
            break;
        case CHANNEL_IUR_CPCHF:
             
            break;
        case CHANNEL_IUR_FACH:
             
            break;
        case CHANNEL_IUR_DSCH:
            dissect_iur_dsch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info);
            break;
        case CHANNEL_EDCH:
        case CHANNEL_EDCH_COMMON:
            dissect_e_dch_channel_info(tvb, pinfo, fp_tree, offset, p_fp_info,
                                       p_fp_info->channel == CHANNEL_EDCH_COMMON,
                                       rlcinf, data);
            break;

        default:
            expert_add_info(pinfo, NULL, &ei_fp_channel_type_unknown);
            break;
    }
    return tvb_captured_length(tvb);
}
