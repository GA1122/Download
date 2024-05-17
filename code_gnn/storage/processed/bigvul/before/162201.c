void VideoCaptureImpl::StartCaptureInternal() {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  state_ = VIDEO_CAPTURE_STATE_STARTING;

  mojom::VideoCaptureObserverPtr observer;
  observer_binding_.Bind(mojo::MakeRequest(&observer));
  GetVideoCaptureHost()->Start(device_id_, session_id_, params_,
                               std::move(observer));
}
