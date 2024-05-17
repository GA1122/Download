void PepperPlatformVideoCapture::StartCapture(
    const media::VideoCaptureParams& params) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!stop_capture_cb_.is_null())
    return;
  VideoCaptureImplManager* manager =
      RenderThreadImpl::current()->video_capture_impl_manager();
  stop_capture_cb_ =
      manager->StartCapture(session_id_,
                            params,
                            media::BindToCurrentLoop(base::Bind(
                                &PepperPlatformVideoCapture::OnStateUpdate,
                                weak_factory_.GetWeakPtr())),
                            media::BindToCurrentLoop(base::Bind(
                                &PepperPlatformVideoCapture::OnFrameReady,
                                weak_factory_.GetWeakPtr())));
}
