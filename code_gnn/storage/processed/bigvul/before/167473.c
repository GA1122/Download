scoped_refptr<DataPipeConsumerDispatcher> DataPipeConsumerDispatcher::Create(
    NodeController* node_controller,
    const ports::PortRef& control_port,
    base::UnsafeSharedMemoryRegion shared_ring_buffer,
    const MojoCreateDataPipeOptions& options,
    uint64_t pipe_id) {
  scoped_refptr<DataPipeConsumerDispatcher> consumer =
      new DataPipeConsumerDispatcher(node_controller, control_port,
                                     std::move(shared_ring_buffer), options,
                                     pipe_id);
  base::AutoLock lock(consumer->lock_);
  if (!consumer->InitializeNoLock())
    return nullptr;
  return consumer;
}
