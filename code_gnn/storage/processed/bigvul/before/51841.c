dissect_common_control(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                       int offset, struct fp_info *p_fp_info)
{
     
    guint8 control_frame_type = tvb_get_guint8(tvb, offset);
    proto_tree_add_item(tree, hf_fp_common_control_frame_type, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset++;

    col_append_str(pinfo->cinfo, COL_INFO,
                   val_to_str_const(control_frame_type, common_control_frame_type_vals, "Unknown"));

     
    switch (control_frame_type) {
        case COMMON_OUTER_LOOP_POWER_CONTROL:
              dissect_common_outer_loop_power_control(pinfo, tree, tvb, offset, p_fp_info);
            break;
        case COMMON_TIMING_ADJUSTMENT:
              dissect_common_timing_adjustment(pinfo, tree, tvb, offset, p_fp_info);
            break;
        case COMMON_DL_SYNCHRONISATION:
              dissect_common_dl_synchronisation(pinfo, tree, tvb, offset, p_fp_info);
            break;
        case COMMON_UL_SYNCHRONISATION:
              dissect_common_ul_synchronisation(pinfo, tree, tvb, offset, p_fp_info);
            break;
        case COMMON_DL_NODE_SYNCHRONISATION:
              dissect_common_dl_node_synchronisation(pinfo, tree, tvb, offset);
            break;
        case COMMON_UL_NODE_SYNCHRONISATION:
              dissect_common_ul_node_synchronisation(pinfo, tree, tvb, offset);
            break;
        case COMMON_DYNAMIC_PUSCH_ASSIGNMENT:
              dissect_common_dynamic_pusch_assignment(pinfo, tree, tvb, offset);
            break;
        case COMMON_TIMING_ADVANCE:
              dissect_common_timing_advance(pinfo, tree, tvb, offset);
            break;
        case COMMON_HS_DSCH_Capacity_Request:
              dissect_hsdpa_capacity_request(pinfo, tree, tvb, offset);
            break;
        case COMMON_HS_DSCH_Capacity_Allocation:
              dissect_hsdpa_capacity_allocation(pinfo, tree, tvb, offset, p_fp_info);
            break;
        case COMMON_HS_DSCH_Capacity_Allocation_Type_2:
              dissect_hsdpa_capacity_allocation_type_2(pinfo, tree, tvb, offset);
            break;

        default:
            break;
    }

      
    
}
