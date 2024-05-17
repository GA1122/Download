bool CommandBufferProxyImpl::Initialize() {
  bool result;
  if (!Send(new GpuCommandBufferMsg_Initialize(route_id_, &result))) {
    LOG(ERROR) << "Could not send GpuCommandBufferMsg_Initialize.";
    return false;
  }

  if (!result) {
    LOG(ERROR) << "Failed to initialize command buffer service.";
    return false;
  }

  int32 state_buffer = CreateTransferBuffer(sizeof *shared_state_, -1);

  if (state_buffer == -1) {
    LOG(ERROR) << "Failed to create shared state transfer buffer.";
    return false;
  }

  gpu::Buffer buffer = GetTransferBuffer(state_buffer);
  if (!buffer.ptr) {
    LOG(ERROR) << "Failed to get shared state transfer buffer";
    return false;
  }

  shared_state_ = reinterpret_cast<gpu::CommandBufferSharedState*>(buffer.ptr);
  shared_state_->Initialize();

  if (!Send(new GpuCommandBufferMsg_SetSharedStateBuffer(route_id_,
                                                         state_buffer))) {
    LOG(ERROR) << "Failed to initialize shared command buffer state.";
    return false;
  }

  return true;
}
