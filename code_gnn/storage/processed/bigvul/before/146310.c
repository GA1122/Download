IntRect WebGLRenderingContextBase::GetImageDataSize(ImageData* pixels) {
  DCHECK(pixels);
  return GetTextureSourceSize(pixels);
}
