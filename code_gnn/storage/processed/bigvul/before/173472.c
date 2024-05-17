int omx_vdec::log_input_buffers(const char *buffer_addr, int buffer_len)
{
 if (m_debug.in_buffer_log && !m_debug.infile) {
 if(!strncmp(drv_ctx.kind,"OMX.qcom.video.decoder.mpeg4", OMX_MAX_STRINGNAME_SIZE)) {
           sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.m4v",
                   m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind,"OMX.qcom.video.decoder.mpeg2", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.mpg", m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this); }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.h263", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.263",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.avc", OMX_MAX_STRINGNAME_SIZE) ||
 !strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.mvc", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.264",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.hevc", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.265",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vc1", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.vc1",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.wmv", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.vc1",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else if(!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vp8", OMX_MAX_STRINGNAME_SIZE)) {
                sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.ivf",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
 else {
               sprintf(m_debug.infile_name, "%s/input_dec_%d_%d_%p.divx",
                        m_debug.log_loc, drv_ctx.video_resolution.frame_width, drv_ctx.video_resolution.frame_height, this);
 }
        m_debug.infile = fopen (m_debug.infile_name, "ab");
 if (!m_debug.infile) {
            DEBUG_PRINT_HIGH("Failed to open input file: %s for logging", m_debug.infile_name);
            m_debug.infile_name[0] = '\0';
 return -1;
 }
 if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vp8", OMX_MAX_STRINGNAME_SIZE)) {
 struct ivf_file_header {
                OMX_U8 signature[4];  
                OMX_U8 version         ;  
                OMX_U8 headersize      ;  
                OMX_U32 FourCC;
                OMX_U8 width;
                OMX_U8 height;
                OMX_U32 rate;
                OMX_U32 scale;
                OMX_U32 length;
                OMX_U8 unused[4];
 } file_header;

            memset((void *)&file_header,0,sizeof(file_header));
            file_header.signature[0] = 'D';
            file_header.signature[1] = 'K';
            file_header.signature[2] = 'I';
            file_header.signature[3] = 'F';
            file_header.version = 0;
            file_header.headersize = 32;
            file_header.FourCC = 0x30385056;
            fwrite((const char *)&file_header,
 sizeof(file_header),1,m_debug.infile);
 }
 }
 if (m_debug.infile && buffer_addr && buffer_len) {
 if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vp8", OMX_MAX_STRINGNAME_SIZE)) {
 struct vp8_ivf_frame_header {
                OMX_U32 framesize;
                OMX_U32 timestamp_lo;
                OMX_U32 timestamp_hi;
 } vp8_frame_header;
            vp8_frame_header.framesize = buffer_len;
  
            vp8_frame_header.timestamp_lo = 0;
            vp8_frame_header.timestamp_hi = 0;
            fwrite((const char *)&vp8_frame_header,
 sizeof(vp8_frame_header),1,m_debug.infile);
 }
        fwrite(buffer_addr, buffer_len, 1, m_debug.infile);
 }
 return 0;
}
