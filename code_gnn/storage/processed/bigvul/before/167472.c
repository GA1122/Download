void DataPipeConsumerDispatcher::CompleteTransitAndClose() {
  node_controller_->SetPortObserver(control_port_, nullptr);

  base::AutoLock lock(lock_);
  DCHECK(in_transit_);
  in_transit_ = false;
  transferred_ = true;
  CloseNoLock();
}
