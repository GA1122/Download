bool RenderWidgetHostViewAndroid::PopulateBitmapWithContents(jobject jbitmap) {
  if (!CompositorImpl::IsInitialized() ||
      texture_id_in_layer_ == 0 ||
      texture_size_in_layer_.IsEmpty())
    return false;

  gfx::JavaBitmap bitmap(jbitmap);

  DCHECK(bitmap.format() == ANDROID_BITMAP_FORMAT_RGBA_8888);

  GLHelper* helper = ImageTransportFactoryAndroid::GetInstance()->GetGLHelper();

  WebKit::WebGLId texture = helper->CopyAndScaleTexture(texture_id_in_layer_,
                                                        texture_size_in_layer_,
                                                        bitmap.size(),
                                                        true);
  if (texture == 0)
    return false;

  helper->ReadbackTextureSync(texture,
                              bitmap.size(),
                              static_cast<unsigned char*> (bitmap.pixels()));

  WebKit::WebGraphicsContext3D* context =
      ImageTransportFactoryAndroid::GetInstance()->GetContext3D();
  context->deleteTexture(texture);

  return true;
}
