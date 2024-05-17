void PepperPlatformVideoCapture::OnStateUpdate(VideoCaptureState state) {
  if (!handler_)
    return;
  switch (state) {
    case VIDEO_CAPTURE_STATE_STARTED:
      handler_->OnStarted();
      break;
    case VIDEO_CAPTURE_STATE_STOPPED:
      handler_->OnStopped();
      break;
    case VIDEO_CAPTURE_STATE_PAUSED:
      handler_->OnPaused();
      break;
    case VIDEO_CAPTURE_STATE_ERROR:
      handler_->OnError();
      break;
    default:
      NOTREACHED() << "Unexpected state: " << state << ".";
  }
}
