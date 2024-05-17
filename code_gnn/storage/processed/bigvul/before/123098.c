WebKit::WebGLId RenderWidgetHostViewAndroid::GetScaledContentTexture(
    const gfx::Size& size) {
  if (!CompositorImpl::IsInitialized() ||
      texture_id_in_layer_ == 0 ||
      texture_size_in_layer_.IsEmpty())
    return 0;

  GLHelper* helper = ImageTransportFactoryAndroid::GetInstance()->GetGLHelper();
  return helper->CopyAndScaleTexture(texture_id_in_layer_,
                                     texture_size_in_layer_,
                                     size,
                                     true);
}
