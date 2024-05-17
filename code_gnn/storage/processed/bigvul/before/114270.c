    WebGraphicsContext3DCommandBufferImpl::CreateViewContext (
      GpuChannelHostFactory* factory,
      int32 surface_id,
      const char* allowed_extensions,
      const WebGraphicsContext3D::Attributes& attributes,
      bool bind_generates_resources,
      const GURL& active_url,
      content::CauseForGpuLaunch cause) {
  WebGraphicsContext3DCommandBufferImpl* context =
      new WebGraphicsContext3DCommandBufferImpl(
          surface_id,
          active_url,
          factory,
          base::WeakPtr<WebGraphicsContext3DSwapBuffersClient>());
  if (!context->Initialize(attributes, bind_generates_resources, cause) ||
      !context->MaybeInitializeGL(allowed_extensions)) {
    delete context;
    return NULL;
  }
  return context;
}
