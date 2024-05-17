void PepperPlatformVideoCapture::OnFrameReady(
    const scoped_refptr<media::VideoFrame>& frame,
    const media::VideoCaptureFormat& format,
    const base::TimeTicks& estimated_capture_time) {
  if (handler_ && !stop_capture_cb_.is_null())
    handler_->OnFrameReady(frame, format);
}
