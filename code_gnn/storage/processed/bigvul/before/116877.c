TestWebKitPlatformSupport::createOffscreenGraphicsContext3D(
    const WebKit::WebGraphicsContext3D::Attributes& attributes) {
  switch (webkit_support::GetGraphicsContext3DImplementation()) {
    case webkit_support::IN_PROCESS:
      return webkit::gpu::WebGraphicsContext3DInProcessImpl::CreateForWebView(
          attributes, false);
    case webkit_support::IN_PROCESS_COMMAND_BUFFER: {
      scoped_ptr<webkit::gpu::WebGraphicsContext3DInProcessCommandBufferImpl>
          context(new
              webkit::gpu::WebGraphicsContext3DInProcessCommandBufferImpl());
      if (!context->Initialize(attributes, NULL))
        return NULL;
      return context.release();
    }
  }
  NOTREACHED();
  return NULL;
}
