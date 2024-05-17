void GpuVideoDecodeAccelerator::NotifyError(
    media::VideoDecodeAccelerator::Error error) {
  if (init_done_msg_) {
    init_done_msg_->set_reply_error();
    if (!Send(init_done_msg_))
      DLOG(ERROR) << "Send(init_done_msg_) failed";
    init_done_msg_ = NULL;
  }
  if (!Send(new AcceleratedVideoDecoderHostMsg_ErrorNotification(
          host_route_id_, error))) {
    DLOG(ERROR) << "Send(AcceleratedVideoDecoderHostMsg_ErrorNotification) "
                << "failed";
  }
}
