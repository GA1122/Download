  gpu::CommandBufferProxyImpl* GetCommandBufferProxy() {
    ws::ContextProviderCommandBuffer* provider_command_buffer =
        static_cast<ws::ContextProviderCommandBuffer*>(context_provider_.get());
    gpu::CommandBufferProxyImpl* command_buffer_proxy =
        provider_command_buffer->GetCommandBufferProxy();
    DCHECK(command_buffer_proxy);
    return command_buffer_proxy;
  }
