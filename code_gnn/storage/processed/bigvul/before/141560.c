void TracingControllerImpl::OnDataAvailable(const void* data,
                                            size_t num_bytes) {
  if (trace_data_endpoint_) {
    const std::string chunk(static_cast<const char*>(data), num_bytes);
    trace_data_endpoint_->ReceiveTraceChunk(
        std::make_unique<std::string>(chunk));
  }
}
