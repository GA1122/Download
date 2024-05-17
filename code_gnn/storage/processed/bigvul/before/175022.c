void impeg2d_post_pic_dec_proc(dec_state_t *ps_dec)
{

   WORD32 u4_update_pic_buf = 0;
  
  
  
 if(ps_dec->u2_picture_structure != FRAME_PICTURE)
 {
        ps_dec->u2_num_vert_mb       = (ps_dec->u2_vertical_size + 31) >> 5;

 if(ps_dec->u2_num_flds_decoded == 1)
 {
            ps_dec->u2_num_flds_decoded = 0;
            u4_update_pic_buf = 1;
 }
 else
 {
            ps_dec->u2_num_flds_decoded = 1;
 }
 }
 else
 {
        u4_update_pic_buf = 1;
 }

 if(u4_update_pic_buf)
 {
        ps_dec->i4_frame_decoded = 1;
 if(ps_dec->e_pic_type != B_PIC)
 {
  
  
 if(ps_dec->aps_ref_pics[0] == NULL)
 {
                ps_dec->aps_ref_pics[0] = ps_dec->ps_cur_pic;
 }

  
 else if(ps_dec->aps_ref_pics[1] == NULL)
 {
                ps_dec->aps_ref_pics[1] = ps_dec->ps_cur_pic;
                impeg2_disp_mgr_add(&ps_dec->s_disp_mgr, ps_dec->aps_ref_pics[0], ps_dec->aps_ref_pics[0]->i4_buf_id);
 }
 else
 {

                impeg2_disp_mgr_add(&ps_dec->s_disp_mgr, ps_dec->aps_ref_pics[1], ps_dec->aps_ref_pics[1]->i4_buf_id);
                impeg2_buf_mgr_release(ps_dec->pv_pic_buf_mg, ps_dec->aps_ref_pics[0]->i4_buf_id, BUF_MGR_REF);
                ps_dec->aps_ref_pics[0] = ps_dec->aps_ref_pics[1];
                ps_dec->aps_ref_pics[1] = ps_dec->ps_cur_pic;

 }
 }
 else
 {
            impeg2_disp_mgr_add(&ps_dec->s_disp_mgr, ps_dec->ps_cur_pic, ps_dec->ps_cur_pic->i4_buf_id);

            impeg2_buf_mgr_release(ps_dec->pv_pic_buf_mg, ps_dec->ps_cur_pic->i4_buf_id, BUF_MGR_REF);
 }

 }
  
  
  
 if(ps_dec->e_pic_type != B_PIC)
 {
 switch(ps_dec->u2_picture_structure)
 {
 case FRAME_PICTURE:
 {
                ps_dec->as_recent_fld[0][0] = ps_dec->as_recent_fld[1][0];
                ps_dec->as_recent_fld[0][1] = ps_dec->as_recent_fld[1][1];

                ps_dec->as_recent_fld[1][0] = ps_dec->s_cur_frm_buf;
                impeg2d_get_bottom_field_buf(&ps_dec->s_cur_frm_buf, &ps_dec->as_recent_fld[1][1],
                ps_dec->u2_frame_width);
 break;
 }
 case TOP_FIELD:
 {
                ps_dec->as_recent_fld[0][0] = ps_dec->as_recent_fld[1][0];
                ps_dec->as_recent_fld[1][0] = ps_dec->s_cur_frm_buf;
 break;
 }
 case BOTTOM_FIELD:
 {
                ps_dec->as_recent_fld[0][1] = ps_dec->as_recent_fld[1][1];
                impeg2d_get_bottom_field_buf(&ps_dec->s_cur_frm_buf, &ps_dec->as_recent_fld[1][1],
                ps_dec->u2_frame_width);
 break;
 }
 }
 }
}
