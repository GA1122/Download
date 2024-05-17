scoped_refptr<DataPipeProducerDispatcher> DataPipeProducerDispatcher::Create(
    NodeController* node_controller,
    const ports::PortRef& control_port,
    base::UnsafeSharedMemoryRegion shared_ring_buffer,
    const MojoCreateDataPipeOptions& options,
    uint64_t pipe_id) {
  scoped_refptr<DataPipeProducerDispatcher> producer =
      new DataPipeProducerDispatcher(node_controller, control_port,
                                     std::move(shared_ring_buffer), options,
                                     pipe_id);
  base::AutoLock lock(producer->lock_);
  if (!producer->InitializeNoLock())
    return nullptr;
  return producer;
}
