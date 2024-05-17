OMX_U32 omx_vdec::count_MB_in_extradata(OMX_OTHER_EXTRADATATYPE *extra)
{
    OMX_U32 num_MB = 0, byte_count = 0, num_MB_in_frame = 0;
    OMX_U8 *data_ptr = extra->data, data = 0;
 while (byte_count < extra->nDataSize) {
        data = *data_ptr;
 while (data) {
            num_MB += (data&0x01);
            data >>= 1;
 }
        data_ptr++;
        byte_count++;
 }
    num_MB_in_frame = ((drv_ctx.video_resolution.frame_width + 15) *
 (drv_ctx.video_resolution.frame_height + 15)) >> 8;
 return ((num_MB_in_frame > 0)?(num_MB * 100 / num_MB_in_frame) : 0);
}
