scoped_refptr<ui::Texture> RenderWidgetHostViewAura::CopyTexture() {
  if (!host_->is_accelerated_compositing_active())
    return scoped_refptr<ui::Texture>();

  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  GLHelper* gl_helper = factory->GetGLHelper();
  if (!gl_helper)
    return scoped_refptr<ui::Texture>();

  std::map<uint64, scoped_refptr<ui::Texture> >::iterator it =
      image_transport_clients_.find(current_surface_);
  if (it == image_transport_clients_.end())
    return scoped_refptr<ui::Texture>();

  ui::Texture* container = it->second;
  DCHECK(container);
  WebKit::WebGLId texture_id =
      gl_helper->CopyTexture(container->PrepareTexture(), container->size());
  if (!texture_id)
    return scoped_refptr<ui::Texture>();

  return scoped_refptr<ui::Texture>(
      factory->CreateOwnedTexture(
          container->size(), device_scale_factor_, texture_id));
}
