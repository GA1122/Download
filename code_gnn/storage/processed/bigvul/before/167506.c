void DataPipeProducerDispatcher::StartSerialize(uint32_t* num_bytes,
                                                uint32_t* num_ports,
                                                uint32_t* num_handles) {
  base::AutoLock lock(lock_);
  DCHECK(in_transit_);
  *num_bytes = sizeof(SerializedState);
  *num_ports = 1;
  *num_handles = 1;
}
