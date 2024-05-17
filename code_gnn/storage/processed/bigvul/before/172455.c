bool venc_dev::venc_set_encode_framerate(OMX_U32 encode_framerate, OMX_U32 config)
{
 struct v4l2_streamparm parm;
 int rc = 0;
 struct venc_framerate frame_rate_cfg;
    Q16ToFraction(encode_framerate,frame_rate_cfg.fps_numerator,frame_rate_cfg.fps_denominator);
    parm.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    parm.parm.output.timeperframe.numerator = frame_rate_cfg.fps_denominator;
    parm.parm.output.timeperframe.denominator = frame_rate_cfg.fps_numerator;

 if (frame_rate_cfg.fps_numerator > 0)
        rc = ioctl(m_nDriver_fd, VIDIOC_S_PARM, &parm);

 if (rc) {
        DEBUG_PRINT_ERROR("ERROR: Request for setting framerate failed");
 return false;
 }

    m_sVenc_cfg.fps_den = frame_rate_cfg.fps_denominator;
    m_sVenc_cfg.fps_num = frame_rate_cfg.fps_numerator;

 if (!config) {
        m_level_set = false;

 if (venc_set_profile_level(0, 0)) {
            DEBUG_PRINT_HIGH("Calling set level (Framerate) with %lu",profile_level.level);
 }
 }

 return true;
}
