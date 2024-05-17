void VideoCaptureImpl::OnStateChanged(mojom::VideoCaptureState state) {
  DVLOG(1) << __func__ << " state: " << state;
  DCHECK(io_thread_checker_.CalledOnValidThread());

  switch (state) {
    case mojom::VideoCaptureState::STARTED:
      state_ = VIDEO_CAPTURE_STATE_STARTED;
      for (const auto& client : clients_)
        client.second.state_update_cb.Run(VIDEO_CAPTURE_STATE_STARTED);
      RequestRefreshFrame();
      break;
    case mojom::VideoCaptureState::STOPPED:
      state_ = VIDEO_CAPTURE_STATE_STOPPED;
      client_buffers_.clear();
      weak_factory_.InvalidateWeakPtrs();
      if (!clients_.empty() || !clients_pending_on_restart_.empty())
        RestartCapture();
      break;
    case mojom::VideoCaptureState::PAUSED:
      for (const auto& client : clients_)
        client.second.state_update_cb.Run(VIDEO_CAPTURE_STATE_PAUSED);
      break;
    case mojom::VideoCaptureState::RESUMED:
      for (const auto& client : clients_)
        client.second.state_update_cb.Run(VIDEO_CAPTURE_STATE_RESUMED);
      break;
    case mojom::VideoCaptureState::FAILED:
      for (const auto& client : clients_)
        client.second.state_update_cb.Run(VIDEO_CAPTURE_STATE_ERROR);
      clients_.clear();
      state_ = VIDEO_CAPTURE_STATE_ERROR;
      break;
    case mojom::VideoCaptureState::ENDED:
      for (const auto& client : clients_)
        client.second.state_update_cb.Run(VIDEO_CAPTURE_STATE_STOPPED);
      clients_.clear();
      state_ = VIDEO_CAPTURE_STATE_ENDED;
      break;
  }
}
