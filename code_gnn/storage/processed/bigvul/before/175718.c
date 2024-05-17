WORD32 ih264d_end_of_pic_processing(dec_struct_t *ps_dec)
{
    UWORD8 u1_pic_type, u1_nal_ref_idc;
 dec_slice_params_t *ps_cur_slice = ps_dec->ps_cur_slice;

  

  

  
    u1_pic_type = 0;
    u1_nal_ref_idc = ps_cur_slice->u1_nal_ref_idc;

 if(u1_nal_ref_idc)
 {
 if(ps_cur_slice->u1_nal_unit_type == IDR_SLICE_NAL)
 {
 if(ps_dec->ps_dpb_cmds->u1_long_term_reference_flag == 0)
 {
                ih264d_reset_ref_bufs(ps_dec->ps_dpb_mgr);
  
                ih264d_insert_st_node(ps_dec->ps_dpb_mgr,
                                      ps_dec->ps_cur_pic,
                                      ps_dec->u1_pic_buf_id,
                                      ps_cur_slice->u2_frame_num);
 }
 else
 {
  

 {
  
                    ih264d_insert_st_node(ps_dec->ps_dpb_mgr,
                                          ps_dec->ps_cur_pic,
                                          ps_dec->u1_pic_buf_id,
                                          ps_cur_slice->u2_frame_num);

  
                    ih264d_delete_st_node_or_make_lt(
                                    ps_dec->ps_dpb_mgr,
                                    ps_cur_slice->u2_frame_num, 0,
                                    ps_cur_slice->u1_field_pic_flag);

                    ps_dec->ps_dpb_mgr->u1_max_lt_pic_idx_plus1 = 1;
 }
 }
 }
 else
 {

 {
                UWORD16 u2_pic_num = ps_cur_slice->u2_frame_num;

  
                ih264d_do_mmco_buffer(ps_dec->ps_dpb_cmds, ps_dec->ps_dpb_mgr,
                              ps_dec->ps_cur_sps->u1_num_ref_frames, u2_pic_num,
 (ps_dec->ps_cur_sps->u2_u4_max_pic_num_minus1),
                              ps_dec->u1_nal_unit_type, ps_dec->ps_cur_pic,
                              ps_dec->u1_pic_buf_id,
                              ps_cur_slice->u1_field_pic_flag,
                              ps_dec->e_dec_status);


 }
 }
        ih264d_update_default_index_list(ps_dec->ps_dpb_mgr);
 }

 if(ps_cur_slice->u1_field_pic_flag)
 {
 if(ps_cur_slice->u1_bottom_field_flag)
 {
 if(u1_nal_ref_idc)
                u1_pic_type = u1_pic_type | BOT_REF;
            u1_pic_type = u1_pic_type | BOT_FLD;
 }
 else
 {
 if(u1_nal_ref_idc)
                u1_pic_type = u1_pic_type | TOP_REF;
            u1_pic_type = u1_pic_type | TOP_FLD;
 }
 }
 else
        u1_pic_type = TOP_REF | BOT_REF;
    ps_dec->ps_cur_pic->u1_pic_type |= u1_pic_type;


 if(ps_cur_slice->u1_field_pic_flag)
 {
        H264_DEC_DEBUG_PRINT("Toggling secondField\n");
        ps_dec->u1_second_field = 1 - ps_dec->u1_second_field;
 }

 return OK;
}
