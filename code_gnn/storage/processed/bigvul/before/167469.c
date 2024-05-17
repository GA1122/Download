void DataPipeConsumerDispatcher::CancelTransit() {
  base::AutoLock lock(lock_);
  DCHECK(in_transit_);
  in_transit_ = false;
  UpdateSignalsStateNoLock();
}
