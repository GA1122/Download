WebGLRenderingContextBase::CreateWebGraphicsContext3DProvider(
    CanvasRenderingContextHost* host,
    const CanvasContextCreationAttributesCore& attributes,
    Platform::ContextType context_type,
    bool* using_gpu_compositing) {
  if (host->IsWebGLBlocked()) {
    host->SetContextCreationWasBlocked();
    host->HostDispatchEvent(WebGLContextEvent::Create(
        event_type_names::kWebglcontextcreationerror,
        "Web page caused context loss and was blocked"));
    return nullptr;
  }
  if ((context_type == Platform::kWebGL1ContextType &&
       !host->IsWebGL1Enabled()) ||
      (context_type == Platform::kWebGL2ContextType &&
       !host->IsWebGL2Enabled()) ||
      (context_type == Platform::kWebGL2ComputeContextType &&
       !host->IsWebGL2Enabled())) {
    host->HostDispatchEvent(WebGLContextEvent::Create(
        event_type_names::kWebglcontextcreationerror,
        "disabled by enterprise policy or commandline switch"));
    return nullptr;
  }

  return CreateContextProviderInternal(host, attributes, context_type,
                                       using_gpu_compositing);
}
