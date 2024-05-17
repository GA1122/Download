void* async_message_thread (void *input)
{
    OMX_BUFFERHEADERTYPE *buffer;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 struct pollfd pfd;
 struct v4l2_buffer v4l2_buf;
    memset((void *)&v4l2_buf,0,sizeof(v4l2_buf));
 struct v4l2_event dqevent;
    omx_vdec *omx = reinterpret_cast<omx_vdec*>(input);
    pfd.events = POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM | POLLRDBAND | POLLPRI;
    pfd.fd = omx->drv_ctx.video_driver_fd;
 int error_code = 0,rc=0,bytes_read = 0,bytes_written = 0;
    DEBUG_PRINT_HIGH("omx_vdec: Async thread start");
    prctl(PR_SET_NAME, (unsigned long)"VideoDecCallBackThread", 0, 0, 0);
 while (1) {
        rc = poll(&pfd, 1, POLL_TIMEOUT);
 if (!rc) {
            DEBUG_PRINT_ERROR("Poll timedout");
 break;
 } else if (rc < 0) {
            DEBUG_PRINT_ERROR("Error while polling: %d", rc);
 break;
 }
 if ((pfd.revents & POLLIN) || (pfd.revents & POLLRDNORM)) {
 struct vdec_msginfo vdec_msg;
            memset(&vdec_msg, 0, sizeof(vdec_msg));
            v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
            v4l2_buf.memory = V4L2_MEMORY_USERPTR;
            v4l2_buf.length = omx->drv_ctx.num_planes;
            v4l2_buf.m.planes = plane;
 while (!ioctl(pfd.fd, VIDIOC_DQBUF, &v4l2_buf)) {
                vdec_msg.msgcode=VDEC_MSG_RESP_OUTPUT_BUFFER_DONE;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                vdec_msg.msgdata.output_frame.client_data=(void*)&v4l2_buf;
                vdec_msg.msgdata.output_frame.len=plane[0].bytesused;
                vdec_msg.msgdata.output_frame.bufferaddr=(void*)plane[0].m.userptr;
                vdec_msg.msgdata.output_frame.time_stamp= ((uint64_t)v4l2_buf.timestamp.tv_sec * (uint64_t)1000000) +
 (uint64_t)v4l2_buf.timestamp.tv_usec;
 if (vdec_msg.msgdata.output_frame.len) {
                    vdec_msg.msgdata.output_frame.framesize.left = plane[0].reserved[2];
                    vdec_msg.msgdata.output_frame.framesize.top = plane[0].reserved[3];
                    vdec_msg.msgdata.output_frame.framesize.right = plane[0].reserved[4];
                    vdec_msg.msgdata.output_frame.framesize.bottom = plane[0].reserved[5];
                    vdec_msg.msgdata.output_frame.picsize.frame_width = plane[0].reserved[6];
                    vdec_msg.msgdata.output_frame.picsize.frame_height = plane[0].reserved[7];
 }
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 }
 }
 if ((pfd.revents & POLLOUT) || (pfd.revents & POLLWRNORM)) {
 struct vdec_msginfo vdec_msg;
            v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
            v4l2_buf.memory = V4L2_MEMORY_USERPTR;
            v4l2_buf.length = 1;
            v4l2_buf.m.planes = plane;
 while (!ioctl(pfd.fd, VIDIOC_DQBUF, &v4l2_buf)) {
                vdec_msg.msgcode=VDEC_MSG_RESP_INPUT_BUFFER_DONE;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                vdec_msg.msgdata.input_frame_clientdata=(void*)&v4l2_buf;
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 }
 }
 if (pfd.revents & POLLPRI) {
            rc = ioctl(pfd.fd, VIDIOC_DQEVENT, &dqevent);
 if (dqevent.type == V4L2_EVENT_MSM_VIDC_PORT_SETTINGS_CHANGED_INSUFFICIENT ) {
 struct vdec_msginfo vdec_msg;
                vdec_msg.msgcode=VDEC_MSG_EVT_CONFIG_CHANGED;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_HIGH("VIDC Port Reconfig recieved insufficient");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_FLUSH_DONE) {
 struct vdec_msginfo vdec_msg;
                vdec_msg.msgcode=VDEC_MSG_RESP_FLUSH_INPUT_DONE;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_HIGH("VIDC Input Flush Done Recieved");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
                vdec_msg.msgcode=VDEC_MSG_RESP_FLUSH_OUTPUT_DONE;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_HIGH("VIDC Output Flush Done Recieved");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_CLOSE_DONE) {
                DEBUG_PRINT_HIGH("VIDC Close Done Recieved and async_message_thread Exited");
 break;
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_HW_OVERLOAD) {
 struct vdec_msginfo vdec_msg;
                vdec_msg.msgcode=VDEC_MSG_EVT_HW_OVERLOAD;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_ERROR("HW Overload received");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_HW_UNSUPPORTED) {
 struct vdec_msginfo vdec_msg;
                vdec_msg.msgcode=VDEC_MSG_EVT_HW_UNSUPPORTED;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_ERROR("HW Unsupported received");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_SYS_ERROR) {
 struct vdec_msginfo vdec_msg;
                vdec_msg.msgcode=VDEC_MSG_EVT_HW_ERROR;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                DEBUG_PRINT_HIGH("SYS Error Recieved");
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exited");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_RELEASE_BUFFER_REFERENCE) {
 unsigned int *ptr = (unsigned int *)(void *)dqevent.u.data;

                DEBUG_PRINT_LOW("REFERENCE RELEASE EVENT RECVD fd = %d offset = %d", ptr[0], ptr[1]);
                omx->buf_ref_remove(ptr[0], ptr[1]);
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_RELEASE_UNQUEUED_BUFFER) {
 unsigned int *ptr = (unsigned int *)(void *)dqevent.u.data;
 struct vdec_msginfo vdec_msg;

                DEBUG_PRINT_LOW("Release unqueued buffer event recvd fd = %d offset = %d", ptr[0], ptr[1]);

                v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
                v4l2_buf.memory = V4L2_MEMORY_USERPTR;
                v4l2_buf.length = omx->drv_ctx.num_planes;
                v4l2_buf.m.planes = plane;
                v4l2_buf.index = ptr[5];
                v4l2_buf.flags = 0;

                vdec_msg.msgcode=VDEC_MSG_RESP_OUTPUT_BUFFER_DONE;
                vdec_msg.status_code=VDEC_S_SUCCESS;
                vdec_msg.msgdata.output_frame.client_data = (void*)&v4l2_buf;
                vdec_msg.msgdata.output_frame.len = 0;
                vdec_msg.msgdata.output_frame.bufferaddr = (void*)(intptr_t)ptr[2];
                vdec_msg.msgdata.output_frame.time_stamp = ((uint64_t)ptr[3] * (uint64_t)1000000) +
 (uint64_t)ptr[4];
 if (omx->async_message_process(input,&vdec_msg) < 0) {
                    DEBUG_PRINT_HIGH("async_message_thread Exitedn");
 break;
 }
 }
 else {
                DEBUG_PRINT_HIGH("VIDC Some Event recieved");
 continue;
 }
 }
 }
    DEBUG_PRINT_HIGH("omx_vdec: Async thread stop");
 return NULL;
}
