bool DataPipeConsumerDispatcher::BeginTransit() {
  base::AutoLock lock(lock_);
  if (in_transit_)
    return false;
  in_transit_ = !in_two_phase_read_;
  return in_transit_;
}
