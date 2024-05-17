WORD32 ihevcd_get_frame_dimensions(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 ihevcd_cxa_ctl_get_frame_dimensions_ip_t *ps_ip;
 ihevcd_cxa_ctl_get_frame_dimensions_op_t *ps_op;
 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;
    WORD32 disp_wd, disp_ht, buffer_wd, buffer_ht, x_offset, y_offset;
    ps_ip = (ihevcd_cxa_ctl_get_frame_dimensions_ip_t *)pv_api_ip;
    ps_op = (ihevcd_cxa_ctl_get_frame_dimensions_op_t *)pv_api_op;
    UNUSED(ps_ip);
 if(ps_codec->i4_sps_done)
 {
        disp_wd = ps_codec->i4_disp_wd;
        disp_ht = ps_codec->i4_disp_ht;

 if(0 == ps_codec->i4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ps_codec->i4_strd;
            buffer_ht = ps_codec->i4_ht + PAD_HT;
 }
 }
 else
 {

        disp_wd = 0;
        disp_ht = 0;

 if(0 == ps_codec->i4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ALIGN16(disp_wd) + PAD_WD;
            buffer_ht = ALIGN16(disp_ht) + PAD_HT;

 }
 }
 if(ps_codec->i4_strd > buffer_wd)
        buffer_wd = ps_codec->i4_strd;

 if(0 == ps_codec->i4_share_disp_buf)
 {
        x_offset = 0;
        y_offset = 0;
 }
 else
 {
        y_offset = PAD_TOP;
        x_offset = PAD_LEFT;
 }

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

 if((ps_codec->e_chroma_fmt == IV_YUV_420SP_UV)
 || (ps_codec->e_chroma_fmt == IV_YUV_420SP_VU))
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
