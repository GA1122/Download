UWORD8 ih264d_is_end_of_pic(UWORD16 u2_frame_num,
                            UWORD8 u1_nal_ref_idc,
 pocstruct_t *ps_cur_poc,
 pocstruct_t *ps_prev_poc,
 dec_slice_params_t * ps_prev_slice,  
                            UWORD8 u1_pic_order_cnt_type,
                            UWORD8 u1_nal_unit_type,
                            UWORD32 u4_idr_pic_id,
                            UWORD8 u1_field_pic_flag,
                            UWORD8 u1_bottom_field_flag)
{
    WORD8 i1_is_end_of_pic;
    WORD8 a, b, c, d, e, f, g, h;

    a = b = c = d = e = f = g = h = 0;
    a = (ps_prev_slice->u2_frame_num != u2_frame_num);
    b = (ps_prev_slice->u1_field_pic_flag != u1_field_pic_flag);
 if(u1_field_pic_flag && ps_prev_slice->u1_field_pic_flag)
        c = (u1_bottom_field_flag != ps_prev_slice->u1_bottom_field_flag);
    d =
 (u1_nal_ref_idc == 0 && ps_prev_slice->u1_nal_ref_idc != 0)
 || (u1_nal_ref_idc != 0
 && ps_prev_slice->u1_nal_ref_idc
 == 0);
 if(!a)
 {
 if((u1_pic_order_cnt_type == 0)
 && (ps_prev_slice->u1_pic_order_cnt_type == 0))
 {
            e =
 ((ps_cur_poc->i4_pic_order_cnt_lsb
 != ps_prev_poc->i4_pic_order_cnt_lsb)
 || (ps_cur_poc->i4_delta_pic_order_cnt_bottom
 != ps_prev_poc->i4_delta_pic_order_cnt_bottom));
 }

 if((u1_pic_order_cnt_type == 1)
 && (ps_prev_slice->u1_pic_order_cnt_type == 1))
 {
            f =
 ((ps_cur_poc->i4_delta_pic_order_cnt[0]
 != ps_prev_poc->i4_delta_pic_order_cnt[0])
 || (ps_cur_poc->i4_delta_pic_order_cnt[1]
 != ps_prev_poc->i4_delta_pic_order_cnt[1]));
 }
 }

 if((u1_nal_unit_type == IDR_SLICE_NAL)
 && (ps_prev_slice->u1_nal_unit_type == IDR_SLICE_NAL))
 {
        g = (u4_idr_pic_id != ps_prev_slice->u4_idr_pic_id);
 }

 if((u1_nal_unit_type == IDR_SLICE_NAL)
 && (ps_prev_slice->u1_nal_unit_type != IDR_SLICE_NAL))
 {
        h = 1;
 }
    i1_is_end_of_pic = a + b + c + d + e + f + g + h;
 return (i1_is_end_of_pic);
}
