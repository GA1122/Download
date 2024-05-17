void venc_dev::venc_close()
{
 struct v4l2_encoder_cmd enc;
    DEBUG_PRINT_LOW("venc_close: fd = %u", (unsigned int)m_nDriver_fd);

 if ((int)m_nDriver_fd >= 0) {
        enc.cmd = V4L2_ENC_CMD_STOP;
        ioctl(m_nDriver_fd, VIDIOC_ENCODER_CMD, &enc);
        DEBUG_PRINT_HIGH("venc_close E");

 if (async_thread_created)
            pthread_join(m_tid,NULL);

        DEBUG_PRINT_HIGH("venc_close X");
        unsubscribe_to_events(m_nDriver_fd);
        close(m_nDriver_fd);
        m_nDriver_fd = -1;
 }

 if (m_debug.infile) {
        fclose(m_debug.infile);
        m_debug.infile = NULL;
 }

 if (m_debug.outfile) {
        fclose(m_debug.outfile);
        m_debug.outfile = NULL;
 }

 if (m_debug.extradatafile) {
        fclose(m_debug.extradatafile);
        m_debug.extradatafile = NULL;
 }
}
