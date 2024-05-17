void WebPluginDelegatePepper::Synchronize3DContext(
    NPDeviceContext3D* context,
    gpu::CommandBuffer::State state) {
  context->getOffset = state.get_offset;
  context->putOffset = state.put_offset;
  context->token = state.token;
  context->error = static_cast<NPDeviceContext3DError>(state.error);
}
