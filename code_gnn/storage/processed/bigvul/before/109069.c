WebKit::WebCompositorOutputSurface* RenderViewImpl::createOutputSurface() {
  WebKit::WebGraphicsContext3D::Attributes attributes;
  attributes.antialias = false;
  attributes.shareResources = true;
  WebGraphicsContext3D* context = CreateGraphicsContext3D(attributes);
  if (!context)
    return NULL;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableSoftwareCompositingGLAdapter)) {
      return new CompositorOutputSurface(routing_id(), NULL,
          new CompositorSoftwareOutputDeviceGLAdapter(context));
  } else
      return new CompositorOutputSurface(routing_id(), context, NULL);
}
