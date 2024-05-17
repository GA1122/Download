NPError WebPluginDelegatePepper::Device3DFlushContext(
    NPP id,
    NPDeviceContext3D* context,
    NPDeviceFlushContextCallbackPtr callback,
    void* user_data) {
  if (!context)
    return NPERR_GENERIC_ERROR;

#if defined(ENABLE_GPU)
  gpu::CommandBuffer::State state;

  if (context->waitForProgress) {
    if (callback) {
      command_buffer_->AsyncFlush(
          context->putOffset,
          method_factory3d_.NewRunnableMethod(
              &WebPluginDelegatePepper::Device3DUpdateState,
              id,
              context,
              callback,
              user_data));
    } else {
      state = command_buffer_->Flush(context->putOffset);
      Synchronize3DContext(context, state);
    }
  } else {
    if (callback) {
      command_buffer_->AsyncGetState(
          method_factory3d_.NewRunnableMethod(
              &WebPluginDelegatePepper::Device3DUpdateState,
              id,
              context,
              callback,
              user_data));
    } else {
      state = command_buffer_->GetState();
      Synchronize3DContext(context, state);
    }
  }
#endif   
  return NPERR_NO_ERROR;
}
