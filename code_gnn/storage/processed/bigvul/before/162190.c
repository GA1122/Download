mojom::VideoCaptureHost* VideoCaptureImpl::GetVideoCaptureHost() {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  if (video_capture_host_for_testing_)
    return video_capture_host_for_testing_;

  if (!video_capture_host_.get())
    video_capture_host_.Bind(std::move(video_capture_host_info_));
  return video_capture_host_.get();
};
