void* venc_dev::async_venc_message_thread (void *input)
{
 struct venc_msg venc_msg;
    omx_video* omx_venc_base = NULL;
    omx_venc *omx = reinterpret_cast<omx_venc*>(input);
    omx_venc_base = reinterpret_cast<omx_video*>(input);
    OMX_BUFFERHEADERTYPE* omxhdr = NULL;

    prctl(PR_SET_NAME, (unsigned long)"VideoEncCallBackThread", 0, 0, 0);
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 struct pollfd pfd;
 struct v4l2_buffer v4l2_buf;
 struct v4l2_event dqevent;
    pfd.events = POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM | POLLRDBAND | POLLPRI;
    pfd.fd = omx->handle->m_nDriver_fd;
 int error_code = 0,rc=0;

    memset(&v4l2_buf, 0, sizeof(v4l2_buf));

 while (1) {
        pthread_mutex_lock(&omx->handle->pause_resume_mlock);

 if (omx->handle->paused) {
            venc_msg.msgcode = VEN_MSG_PAUSE;
            venc_msg.statuscode = VEN_S_SUCCESS;

 if (omx->async_message_process(input, &venc_msg) < 0) {
                DEBUG_PRINT_ERROR("ERROR: Failed to process pause msg");
                pthread_mutex_unlock(&omx->handle->pause_resume_mlock);
 break;
 }

  
            pthread_cond_wait(&omx->handle->pause_resume_cond,
 &omx->handle->pause_resume_mlock);

            venc_msg.msgcode = VEN_MSG_RESUME;
            venc_msg.statuscode = VEN_S_SUCCESS;

 if (omx->async_message_process(input, &venc_msg) < 0) {
                DEBUG_PRINT_ERROR("ERROR: Failed to process resume msg");
                pthread_mutex_unlock(&omx->handle->pause_resume_mlock);
 break;
 }
 }

        pthread_mutex_unlock(&omx->handle->pause_resume_mlock);

        rc = poll(&pfd, 1, POLL_TIMEOUT);

 if (!rc) {
            DEBUG_PRINT_HIGH("Poll timedout, pipeline stalled due to client/firmware ETB: %d, EBD: %d, FTB: %d, FBD: %d",
                    omx->handle->etb, omx->handle->ebd, omx->handle->ftb, omx->handle->fbd);
 continue;
 } else if (rc < 0) {
            DEBUG_PRINT_ERROR("Error while polling: %d", rc);
 break;
 }

 if ((pfd.revents & POLLIN) || (pfd.revents & POLLRDNORM)) {
            v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
            v4l2_buf.memory = V4L2_MEMORY_USERPTR;
            v4l2_buf.length = omx->handle->num_planes;
            v4l2_buf.m.planes = plane;

 while (!ioctl(pfd.fd, VIDIOC_DQBUF, &v4l2_buf)) {
                venc_msg.msgcode=VEN_MSG_OUTPUT_BUFFER_DONE;
                venc_msg.statuscode=VEN_S_SUCCESS;
                omxhdr=omx_venc_base->m_out_mem_ptr+v4l2_buf.index;
                venc_msg.buf.len= v4l2_buf.m.planes->bytesused;
                venc_msg.buf.offset = v4l2_buf.m.planes->data_offset;
                venc_msg.buf.flags = 0;
                venc_msg.buf.ptrbuffer = (OMX_U8 *)omx_venc_base->m_pOutput_pmem[v4l2_buf.index].buffer;
                venc_msg.buf.clientdata=(void*)omxhdr;
                venc_msg.buf.timestamp = (uint64_t) v4l2_buf.timestamp.tv_sec * (uint64_t) 1000000 + (uint64_t) v4l2_buf.timestamp.tv_usec;

  
 if (v4l2_buf.flags & V4L2_QCOM_BUF_FLAG_IDRFRAME)
                    venc_msg.buf.flags |= QOMX_VIDEO_PictureTypeIDR;

 if (v4l2_buf.flags & V4L2_BUF_FLAG_KEYFRAME)
                    venc_msg.buf.flags |= OMX_BUFFERFLAG_SYNCFRAME;

 if (v4l2_buf.flags & V4L2_QCOM_BUF_FLAG_CODECCONFIG)
                    venc_msg.buf.flags |= OMX_BUFFERFLAG_CODECCONFIG;

 if (v4l2_buf.flags & V4L2_QCOM_BUF_FLAG_EOS)
                    venc_msg.buf.flags |= OMX_BUFFERFLAG_EOS;

 if (omx->handle->num_planes > 1 && v4l2_buf.m.planes->bytesused)
                    venc_msg.buf.flags |= OMX_BUFFERFLAG_EXTRADATA;

 if (omxhdr->nFilledLen)
                    venc_msg.buf.flags |= OMX_BUFFERFLAG_ENDOFFRAME;

                omx->handle->fbd++;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }
 }
 }

 if ((pfd.revents & POLLOUT) || (pfd.revents & POLLWRNORM)) {
            v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
            v4l2_buf.memory = V4L2_MEMORY_USERPTR;
            v4l2_buf.m.planes = plane;
            v4l2_buf.length = 1;

 while (!ioctl(pfd.fd, VIDIOC_DQBUF, &v4l2_buf)) {
                venc_msg.msgcode=VEN_MSG_INPUT_BUFFER_DONE;
                venc_msg.statuscode=VEN_S_SUCCESS;
 if (omx_venc_base->mUseProxyColorFormat && !omx_venc_base->mUsesColorConversion)
                    omxhdr = &omx_venc_base->meta_buffer_hdr[v4l2_buf.index];
 else
                    omxhdr = &omx_venc_base->m_inp_mem_ptr[v4l2_buf.index];

                venc_msg.buf.clientdata=(void*)omxhdr;
                omx->handle->ebd++;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }
 }
 }

 if (pfd.revents & POLLPRI) {
            rc = ioctl(pfd.fd, VIDIOC_DQEVENT, &dqevent);

 if (dqevent.type == V4L2_EVENT_MSM_VIDC_CLOSE_DONE) {
                DEBUG_PRINT_HIGH("CLOSE DONE");
 break;
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_FLUSH_DONE) {
                venc_msg.msgcode = VEN_MSG_FLUSH_INPUT_DONE;
                venc_msg.statuscode = VEN_S_SUCCESS;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }

                venc_msg.msgcode = VEN_MSG_FLUSH_OUPUT_DONE;
                venc_msg.statuscode = VEN_S_SUCCESS;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_HW_OVERLOAD) {
                DEBUG_PRINT_ERROR("HW Overload received");
                venc_msg.statuscode = VEN_S_EFAIL;
                venc_msg.msgcode = VEN_MSG_HW_OVERLOAD;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }
 } else if (dqevent.type == V4L2_EVENT_MSM_VIDC_SYS_ERROR) {
                DEBUG_PRINT_ERROR("ERROR: Encoder is in bad state");
                venc_msg.msgcode = VEN_MSG_INDICATION;
                venc_msg.statuscode=VEN_S_EFAIL;

 if (omx->async_message_process(input,&venc_msg) < 0) {
                    DEBUG_PRINT_ERROR("ERROR: Wrong ioctl message");
 break;
 }
 }
 }
 }

    DEBUG_PRINT_HIGH("omx_venc: Async Thread exit");
 return NULL;
}
