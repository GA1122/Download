IV_API_CALL_STATUS_T impeg2d_get_frame_dimensions(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 impeg2d_ctl_get_frame_dimensions_ip_t *ps_ip;
 impeg2d_ctl_get_frame_dimensions_op_t *ps_op;
    WORD32 disp_wd, disp_ht, buffer_wd, buffer_ht, x_offset, y_offset;
 dec_state_t *ps_codec;
 dec_state_multi_core_t *ps_dec_state_multi_core;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_codec_obj->pv_codec_handle);
    ps_codec = ps_dec_state_multi_core->ps_dec_state[0];


    ps_ip = (impeg2d_ctl_get_frame_dimensions_ip_t *)pv_api_ip;
    ps_op = (impeg2d_ctl_get_frame_dimensions_op_t *)pv_api_op;
    UNUSED(ps_ip);
 if(ps_codec->u2_header_done)
 {
        disp_wd = ps_codec->u2_horizontal_size;
        disp_ht = ps_codec->u2_vertical_size;

 if(0 == ps_codec->u4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ps_codec->u2_frame_width;
            buffer_ht = ps_codec->u2_frame_height;
 }
 }
 else
 {

        disp_wd = ps_codec->u2_create_max_width;
        disp_ht = ps_codec->u2_create_max_height;

 if(0 == ps_codec->u4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ALIGN16(disp_wd);
            buffer_ht = ALIGN16(disp_ht);

 }
 }
 if(ps_codec->u2_frame_width > buffer_wd)
        buffer_wd = ps_codec->u2_frame_width;

    x_offset = 0;
    y_offset = 0;


    ps_op->u4_disp_wd[0] = disp_wd;
    ps_op->u4_disp_ht[0] = disp_ht;
    ps_op->u4_buffer_wd[0] = buffer_wd;
    ps_op->u4_buffer_ht[0] = buffer_ht;
    ps_op->u4_x_offset[0] = x_offset;
    ps_op->u4_y_offset[0] = y_offset;

    ps_op->u4_disp_wd[1] = ps_op->u4_disp_wd[2] = ((ps_op->u4_disp_wd[0] + 1)
 >> 1);
    ps_op->u4_disp_ht[1] = ps_op->u4_disp_ht[2] = ((ps_op->u4_disp_ht[0] + 1)
 >> 1);
    ps_op->u4_buffer_wd[1] = ps_op->u4_buffer_wd[2] = (ps_op->u4_buffer_wd[0]
 >> 1);
    ps_op->u4_buffer_ht[1] = ps_op->u4_buffer_ht[2] = (ps_op->u4_buffer_ht[0]
 >> 1);
    ps_op->u4_x_offset[1] = ps_op->u4_x_offset[2] = (ps_op->u4_x_offset[0]
 >> 1);
    ps_op->u4_y_offset[1] = ps_op->u4_y_offset[2] = (ps_op->u4_y_offset[0]
 >> 1);

 if((ps_codec->i4_chromaFormat == IV_YUV_420SP_UV)
 || (ps_codec->i4_chromaFormat == IV_YUV_420SP_VU))
 {
        ps_op->u4_disp_wd[2] = 0;
        ps_op->u4_disp_ht[2] = 0;
        ps_op->u4_buffer_wd[2] = 0;
        ps_op->u4_buffer_ht[2] = 0;
        ps_op->u4_x_offset[2] = 0;
        ps_op->u4_y_offset[2] = 0;

        ps_op->u4_disp_wd[1] <<= 1;
        ps_op->u4_buffer_wd[1] <<= 1;
        ps_op->u4_x_offset[1] <<= 1;
 }

 return IV_SUCCESS;

}
