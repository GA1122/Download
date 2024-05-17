void WebGraphicsContext3DCommandBufferImpl::InitializeWithCommandBuffer(
    CommandBufferProxy* command_buffer,
    const WebGraphicsContext3D::Attributes& attributes,
    bool bind_generates_resources) {
  DCHECK(command_buffer);
  command_buffer_ = command_buffer;
  attributes_ = attributes;
  bind_generates_resources_ = bind_generates_resources;
}
