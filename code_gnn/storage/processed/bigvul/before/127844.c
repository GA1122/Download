void PepperPlatformVideoCapture::StopCapture() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (stop_capture_cb_.is_null())
    return;
  stop_capture_cb_.Run();
  stop_capture_cb_.Reset();
}
