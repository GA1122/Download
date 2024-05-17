void GpuProcessHost::RequestHDRStatus(RequestHDRStatusCallback request_cb) {
  gpu_service_ptr_->RequestHDRStatus(std::move(request_cb));
}
