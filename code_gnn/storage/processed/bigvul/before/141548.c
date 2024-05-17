void TracingControllerImpl::CompleteFlush() {
  if (trace_data_endpoint_) {
    trace_data_endpoint_->ReceiveTraceFinalContents(
        std::move(filtered_metadata_));
  }
  filtered_metadata_.reset(nullptr);
  trace_data_endpoint_ = nullptr;
  trace_config_ = nullptr;
  drainer_ = nullptr;
}
