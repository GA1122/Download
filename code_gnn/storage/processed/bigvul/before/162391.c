MojoResult Core::CreateDataPipe(const MojoCreateDataPipeOptions* options,
                                MojoHandle* data_pipe_producer_handle,
                                MojoHandle* data_pipe_consumer_handle) {
  RequestContext request_context;
  if (options && options->struct_size != sizeof(MojoCreateDataPipeOptions))
    return MOJO_RESULT_INVALID_ARGUMENT;

  MojoCreateDataPipeOptions create_options;
  create_options.struct_size = sizeof(MojoCreateDataPipeOptions);
  create_options.flags = options ? options->flags : 0;
  create_options.element_num_bytes = options ? options->element_num_bytes : 1;
  create_options.capacity_num_bytes = options && options->capacity_num_bytes
                                          ? options->capacity_num_bytes
                                          : 64 * 1024;
  if (!create_options.element_num_bytes || !create_options.capacity_num_bytes ||
      create_options.capacity_num_bytes < create_options.element_num_bytes) {
    return MOJO_RESULT_INVALID_ARGUMENT;
  }

  scoped_refptr<PlatformSharedBuffer> ring_buffer =
      GetNodeController()->CreateSharedBuffer(
          create_options.capacity_num_bytes);
  if (!ring_buffer)
    return MOJO_RESULT_RESOURCE_EXHAUSTED;

  ports::PortRef port0, port1;
  GetNodeController()->node()->CreatePortPair(&port0, &port1);

  DCHECK(data_pipe_producer_handle);
  DCHECK(data_pipe_consumer_handle);

  uint64_t pipe_id = base::RandUint64();
  scoped_refptr<Dispatcher> producer = DataPipeProducerDispatcher::Create(
      GetNodeController(), port0, ring_buffer, create_options, pipe_id);
  if (!producer)
    return MOJO_RESULT_RESOURCE_EXHAUSTED;

  scoped_refptr<Dispatcher> consumer = DataPipeConsumerDispatcher::Create(
      GetNodeController(), port1, ring_buffer, create_options, pipe_id);
  if (!consumer) {
    producer->Close();
    return MOJO_RESULT_RESOURCE_EXHAUSTED;
  }

  *data_pipe_producer_handle = AddDispatcher(producer);
  *data_pipe_consumer_handle = AddDispatcher(consumer);
  if (*data_pipe_producer_handle == MOJO_HANDLE_INVALID ||
      *data_pipe_consumer_handle == MOJO_HANDLE_INVALID) {
    if (*data_pipe_producer_handle != MOJO_HANDLE_INVALID) {
      scoped_refptr<Dispatcher> unused;
      base::AutoLock lock(handles_->GetLock());
      handles_->GetAndRemoveDispatcher(*data_pipe_producer_handle, &unused);
    }
    producer->Close();
    consumer->Close();
    return MOJO_RESULT_RESOURCE_EXHAUSTED;
  }

  return MOJO_RESULT_OK;
}
