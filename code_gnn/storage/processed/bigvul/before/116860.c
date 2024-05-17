WebGraphicsContext3D* RenderViewImpl::createGraphicsContext3D(
    const WebGraphicsContext3D::Attributes& attributes) {
  if (!webview())
    return NULL;
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessWebGL)) {
    return webkit::gpu::WebGraphicsContext3DInProcessImpl::CreateForWebView(
        attributes, true);
  } else {
    GURL url;
    if (webview()->mainFrame())
      url = GURL(webview()->mainFrame()->document().url());

    scoped_ptr<WebGraphicsContext3DCommandBufferImpl> context(
        new WebGraphicsContext3DCommandBufferImpl(
            surface_id(), url, RenderThreadImpl::current(), AsWeakPtr()));

    if (!context->Initialize(attributes))
      return NULL;
    context_is_web_graphics_context_3d_command_buffer_impl_ = true;
    return context.release();
  }
}
