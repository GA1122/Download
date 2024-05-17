void DataPipeProducerDispatcher::CompleteTransitAndClose() {
  node_controller_->SetPortObserver(control_port_, nullptr);

  base::AutoLock lock(lock_);
  DCHECK(in_transit_);
  transferred_ = true;
  in_transit_ = false;
  CloseNoLock();
}
