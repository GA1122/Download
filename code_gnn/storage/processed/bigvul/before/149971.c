void LayerTreeHostImpl::MainThreadHasStoppedFlinging() {
  browser_controls_offset_manager_->MainThreadHasStoppedFlinging();
  if (input_handler_client_)
    input_handler_client_->MainThreadHasStoppedFlinging();
}
