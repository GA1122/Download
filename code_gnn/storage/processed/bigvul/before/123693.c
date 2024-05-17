void GpuVideoDecodeAccelerator::NotifyError(
    media::VideoDecodeAccelerator::Error error) {
  if (init_done_msg_) {
    GpuCommandBufferMsg_CreateVideoDecoder::WriteReplyParams(
        init_done_msg_, -1);
    if (!Send(init_done_msg_))
      DLOG(ERROR) << "Send(init_done_msg_) failed";
    init_done_msg_ = NULL;
    return;
  }
  if (!Send(new AcceleratedVideoDecoderHostMsg_ErrorNotification(
          host_route_id_, error))) {
    DLOG(ERROR) << "Send(AcceleratedVideoDecoderHostMsg_ErrorNotification) "
                << "failed";
  }
}
