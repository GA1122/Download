void omx_vdec::set_frame_rate(OMX_S64 act_timestamp)
{
    OMX_U32 new_frame_interval = 0;
 if (VALID_TS(act_timestamp) && VALID_TS(prev_ts) && act_timestamp != prev_ts
 && llabs(act_timestamp - prev_ts) > 2000) {
        new_frame_interval = client_set_fps ? frm_int :
            llabs(act_timestamp - prev_ts);
 if (new_frame_interval != frm_int || frm_int == 0) {
            frm_int = new_frame_interval;
 if (frm_int) {
                drv_ctx.frame_rate.fps_numerator = 1e6;
                drv_ctx.frame_rate.fps_denominator = frm_int;
                DEBUG_PRINT_LOW("set_frame_rate: frm_int(%u) fps(%f)",
 (unsigned int)frm_int, drv_ctx.frame_rate.fps_numerator /
 (float)drv_ctx.frame_rate.fps_denominator);
  
 struct v4l2_outputparm oparm;
  
                oparm.timeperframe.numerator = drv_ctx.frame_rate.fps_denominator;
                oparm.timeperframe.denominator = drv_ctx.frame_rate.fps_numerator;

 struct v4l2_streamparm sparm;
                sparm.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
                sparm.parm.output = oparm;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_PARM, &sparm)) {
                    DEBUG_PRINT_ERROR("Unable to convey fps info to driver, \
                            performance might be affected");
 }

 }
 }
 }
    prev_ts = act_timestamp;
}
