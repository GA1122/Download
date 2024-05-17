 bool omx_vdec::allocate_color_convert_buf::get_buffer_req
(unsigned int &buffer_size)
{
 bool status = true;
    pthread_mutex_lock(&omx->c_lock);
 if (!enabled)
        buffer_size = omx->drv_ctx.op_buf.buffer_size;
 else {
 if (!c2d.get_buffer_size(C2D_OUTPUT,buffer_size)) {
            DEBUG_PRINT_ERROR("Get buffer size failed");
            status = false;
 goto fail_get_buffer_size;
 }
 }
 if (buffer_size < omx->drv_ctx.op_buf.buffer_size)
        buffer_size = omx->drv_ctx.op_buf.buffer_size;
 if (buffer_alignment_req < omx->drv_ctx.op_buf.alignment)
        buffer_alignment_req = omx->drv_ctx.op_buf.alignment;
fail_get_buffer_size:
    pthread_mutex_unlock(&omx->c_lock);
 return status;
}
