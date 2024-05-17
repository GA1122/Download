void VideoCaptureImpl::StartCapture(
    int client_id,
    const media::VideoCaptureParams& params,
    const VideoCaptureStateUpdateCB& state_update_cb,
    const VideoCaptureDeliverFrameCB& deliver_frame_cb) {
  DVLOG(1) << __func__ << " |device_id_| = " << device_id_;
  DCHECK(io_thread_checker_.CalledOnValidThread());
  ClientInfo client_info;
  client_info.params = params;
  client_info.state_update_cb = state_update_cb;
  client_info.deliver_frame_cb = deliver_frame_cb;

  switch (state_) {
    case VIDEO_CAPTURE_STATE_STARTING:
    case VIDEO_CAPTURE_STATE_STARTED:
      clients_[client_id] = client_info;
      DCHECK_EQ(params_.resolution_change_policy,
                params.resolution_change_policy);
      return;
    case VIDEO_CAPTURE_STATE_STOPPING:
      clients_pending_on_restart_[client_id] = client_info;
      DVLOG(1) << __func__ << " Got new resolution while stopping: "
               << params.requested_format.frame_size.ToString();
      return;
    case VIDEO_CAPTURE_STATE_STOPPED:
    case VIDEO_CAPTURE_STATE_ENDED:
      clients_[client_id] = client_info;
      params_ = params;
      params_.requested_format.frame_rate =
          std::min(params_.requested_format.frame_rate,
                   static_cast<float>(media::limits::kMaxFramesPerSecond));

      DVLOG(1) << "StartCapture: starting with first resolution "
               << params_.requested_format.frame_size.ToString();
      StartCaptureInternal();
      return;
    case VIDEO_CAPTURE_STATE_ERROR:
      state_update_cb.Run(VIDEO_CAPTURE_STATE_ERROR);
      return;
    case VIDEO_CAPTURE_STATE_PAUSED:
    case VIDEO_CAPTURE_STATE_RESUMED:
      NOTREACHED();
      return;
  }
}
