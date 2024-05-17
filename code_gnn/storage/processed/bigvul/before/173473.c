int omx_vdec::log_output_buffers(OMX_BUFFERHEADERTYPE *buffer) {
 if (m_debug.out_buffer_log && !m_debug.outfile && buffer->nFilledLen) {
        sprintf(m_debug.outfile_name, "%s/output_%d_%d_%p.yuv",
                m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
        m_debug.outfile = fopen (m_debug.outfile_name, "ab");
 if (!m_debug.outfile) {
            DEBUG_PRINT_HIGH("Failed to open output file: %s for logging", m_debug.log_loc);
            m_debug.outfile_name[0] = '\0';
 return -1;
 }
 }
 if (m_debug.outfile && buffer && buffer->nFilledLen) {
 int buf_index = buffer - m_out_mem_ptr;
 int stride = drv_ctx.video_resolution.stride;
 int scanlines = drv_ctx.video_resolution.scan_lines;
 if (m_smoothstreaming_mode) {
            stride = drv_ctx.video_resolution.frame_width;
            scanlines = drv_ctx.video_resolution.frame_height;
            stride = (stride + DEFAULT_WIDTH_ALIGNMENT - 1) & (~(DEFAULT_WIDTH_ALIGNMENT - 1));
            scanlines = (scanlines + DEFAULT_HEIGHT_ALIGNMENT - 1) & (~(DEFAULT_HEIGHT_ALIGNMENT - 1));
 }
 char *temp = (char *)drv_ctx.ptr_outputbuffer[buf_index].bufferaddr;
 unsigned i;
        DEBUG_PRINT_HIGH("Logging width/height(%u/%u) stride/scanlines(%u/%u)",
            drv_ctx.video_resolution.frame_width,
            drv_ctx.video_resolution.frame_height, stride, scanlines);
 int bytes_written = 0;
 for (i = 0; i < drv_ctx.video_resolution.frame_height; i++) {
             bytes_written = fwrite(temp, drv_ctx.video_resolution.frame_width, 1, m_debug.outfile);
             temp += stride;
 }
        temp = (char *)drv_ctx.ptr_outputbuffer[buf_index].bufferaddr + stride * scanlines;
 int stride_c = stride;
 for(i = 0; i < drv_ctx.video_resolution.frame_height/2; i++) {
            bytes_written += fwrite(temp, drv_ctx.video_resolution.frame_width, 1, m_debug.outfile);
            temp += stride_c;
 }
 }
 return 0;
}
