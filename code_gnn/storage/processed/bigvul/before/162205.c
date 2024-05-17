VideoCaptureImpl::VideoCaptureImpl(media::VideoCaptureSessionId session_id)
    : device_id_(session_id),
      session_id_(session_id),
      video_capture_host_for_testing_(nullptr),
      observer_binding_(this),
      state_(VIDEO_CAPTURE_STATE_STOPPED),
      weak_factory_(this) {
  io_thread_checker_.DetachFromThread();

  if (ChildThread::Get()) {   
    mojom::VideoCaptureHostPtr temp_video_capture_host;
    ChildThread::Get()->GetConnector()->BindInterface(
        mojom::kBrowserServiceName,
        mojo::MakeRequest(&temp_video_capture_host));
    video_capture_host_info_ = temp_video_capture_host.PassInterface();
  }
}
