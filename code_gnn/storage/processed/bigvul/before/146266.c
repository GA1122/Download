WebGLRenderingContextBase::CreateWebGraphicsContext3DProvider(
    CanvasRenderingContextHost* host,
    const CanvasContextCreationAttributes& attributes,
    unsigned web_gl_version) {
  if (!host->IsWebGLAllowed()) {
    host->HostDispatchEvent(WebGLContextEvent::Create(
        EventTypeNames::webglcontextcreationerror, false, true,
        "Web page was not allowed to create a WebGL context."));
    return nullptr;
  }

  return CreateContextProviderInternal(host, attributes, web_gl_version);
}
