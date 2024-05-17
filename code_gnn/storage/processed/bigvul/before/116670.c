WebGraphicsContext3D* RenderViewImpl::createGraphicsContext3D(
    const WebGraphicsContext3D::Attributes& attributes,
    bool direct) {
  if (!webview())
    return NULL;
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessWebGL)) {
    return webkit::gpu::WebGraphicsContext3DInProcessImpl::CreateForWebView(
        attributes, direct);
  } else {
    int surface = direct ? surface_id() : 0;

    GURL url;
    if (webview()->mainFrame())
      url = GURL(webview()->mainFrame()->document().url());

    base::WeakPtr<WebGraphicsContext3DSwapBuffersClient> swap_client;
    if (direct)
      swap_client = AsWeakPtr();

    scoped_ptr<WebGraphicsContext3DCommandBufferImpl> context(
        new WebGraphicsContext3DCommandBufferImpl(
            surface, url, RenderThreadImpl::current(), swap_client));

    if (!context->Initialize(attributes))
      return NULL;
    context_is_web_graphics_context_3d_command_buffer_impl_ = true;
    return context.release();
  }
}
