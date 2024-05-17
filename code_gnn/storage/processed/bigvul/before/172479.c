bool venc_dev::venc_set_session_priority(OMX_U32 priority) {
 struct v4l2_control control;

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_PRIORITY;
 switch(priority) {
 case 0:
            control.value = V4L2_MPEG_VIDC_VIDEO_PRIORITY_REALTIME_ENABLE;
 break;
 case 1:
            control.value = V4L2_MPEG_VIDC_VIDEO_PRIORITY_REALTIME_DISABLE;
 break;
 default:
            priority = 1;
            control.value = V4L2_MPEG_VIDC_VIDEO_PRIORITY_REALTIME_DISABLE;
            DEBUG_PRINT_ERROR("Unsupported priority level %u", priority);
 break;
 }

 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
        DEBUG_PRINT_ERROR("Failed to set V4L2_MPEG_VIDC_VIDEO_PRIORITY_REALTIME_%s",
                priority == 0 ? "ENABLE" : "DISABLE");
 return false;
 }

    sess_priority.priority = priority;

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%x, val=%d",
            control.id, control.value);
 return true;
}
