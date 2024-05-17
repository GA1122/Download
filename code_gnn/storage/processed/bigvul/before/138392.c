std::unique_ptr<service_manager::Service> CreateVideoCaptureService() {
  return base::MakeUnique<video_capture::ServiceImpl>();
}
