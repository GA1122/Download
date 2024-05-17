IntRect WebGLRenderingContextBase::SafeGetImageSize(Image* image) {
  if (!image)
    return IntRect();

  return GetTextureSourceSize(image);
}
