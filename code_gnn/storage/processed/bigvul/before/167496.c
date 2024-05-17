DataPipeProducerDispatcher::DataPipeProducerDispatcher(
    NodeController* node_controller,
    const ports::PortRef& control_port,
    base::UnsafeSharedMemoryRegion shared_ring_buffer,
    const MojoCreateDataPipeOptions& options,
    uint64_t pipe_id)
    : options_(options),
      node_controller_(node_controller),
      control_port_(control_port),
      pipe_id_(pipe_id),
      watchers_(this),
      shared_ring_buffer_(std::move(shared_ring_buffer)),
      available_capacity_(options_.capacity_num_bytes) {}
