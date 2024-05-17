void VideoCaptureImpl::RequestRefreshFrame() {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  GetVideoCaptureHost()->RequestRefreshFrame(device_id_);
}
