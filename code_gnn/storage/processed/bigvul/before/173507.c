bool omx_vdec::allocate_color_convert_buf::update_buffer_req()
{
 bool status = true;
 unsigned int src_size = 0, destination_size = 0;
    OMX_COLOR_FORMATTYPE drv_color_format;
 if (!omx) {
        DEBUG_PRINT_ERROR("Invalid client in color convert");
 return false;
 }
 if (!enabled) {
        DEBUG_PRINT_HIGH("No color conversion required");
 return status;
 }
    pthread_mutex_lock(&omx->c_lock);
 if (omx->drv_ctx.output_format != VDEC_YUV_FORMAT_NV12 &&
 ColorFormat != OMX_COLOR_FormatYUV420Planar) {
        DEBUG_PRINT_ERROR("update_buffer_req: Unsupported color conversion");
        status = false;
 goto fail_update_buf_req;
 }
    c2d.close();
    status = c2d.open(omx->drv_ctx.video_resolution.frame_height,
            omx->drv_ctx.video_resolution.frame_width,
            NV12_128m,dest_format);
 if (status) {
        status = c2d.get_buffer_size(C2D_INPUT,src_size);
 if (status)
            status = c2d.get_buffer_size(C2D_OUTPUT,destination_size);
 }
 if (status) {
 if (!src_size || src_size > omx->drv_ctx.op_buf.buffer_size ||
 !destination_size) {
            DEBUG_PRINT_ERROR("ERROR: Size mismatch in C2D src_size %d"
 "driver size %u destination size %d",
                    src_size, (unsigned int)omx->drv_ctx.op_buf.buffer_size,
                    destination_size);
            status = false;
            c2d.close();
            buffer_size_req = 0;
 } else {
            buffer_size_req = destination_size;
 if (buffer_size_req < omx->drv_ctx.op_buf.buffer_size)
                buffer_size_req = omx->drv_ctx.op_buf.buffer_size;
 if (buffer_alignment_req < omx->drv_ctx.op_buf.alignment)
                buffer_alignment_req = omx->drv_ctx.op_buf.alignment;
 }
 }
fail_update_buf_req:
    pthread_mutex_unlock(&omx->c_lock);
 return status;
}
