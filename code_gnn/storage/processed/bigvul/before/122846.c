DirectGLImageTransportFactory::DirectGLImageTransportFactory() {
  WebKit::WebGraphicsContext3D::Attributes attrs;
  attrs.shareResources = false;
  attrs.noAutomaticFlushes = true;
  context_.reset(
      webkit::gpu::WebGraphicsContext3DInProcessImpl::CreateForWindow(
          attrs,
          NULL,
          NULL));
}
