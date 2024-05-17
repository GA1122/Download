WebGLRenderingContextBase::CreateWebGraphicsContext3DProvider(
    CanvasRenderingContextHost* host,
    const CanvasContextCreationAttributes& attributes,
    unsigned webgl_version,
    bool* using_gpu_compositing) {
  if (host->IsWebGLBlocked()) {
    host->HostDispatchEvent(WebGLContextEvent::Create(
        EventTypeNames::webglcontextcreationerror, false, true,
        "Web page caused context loss and was blocked"));
    return nullptr;
  }
  if ((webgl_version == 1 && !host->IsWebGL1Enabled()) ||
      (webgl_version == 2 && !host->IsWebGL2Enabled())) {
    host->HostDispatchEvent(WebGLContextEvent::Create(
        EventTypeNames::webglcontextcreationerror, false, true,
        "disabled by enterprise policy or commandline switch"));
    return nullptr;
  }

  return CreateContextProviderInternal(host, attributes, webgl_version,
                                       using_gpu_compositing);
}
