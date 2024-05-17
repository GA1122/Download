void TracingControllerImpl::OnDataComplete() {
  is_data_complete_ = true;
  if (is_metadata_available_)
    CompleteFlush();
}
