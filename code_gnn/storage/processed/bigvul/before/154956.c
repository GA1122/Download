WebGLRenderingContextBase::GetStaticBitmapImage(
    std::unique_ptr<viz::SingleReleaseCallback>* out_release_callback) {
  if (!GetDrawingBuffer())
    return nullptr;

  if (CreationAttributes().preserve_drawing_buffer)
    return GetImage();

  return GetDrawingBuffer()->TransferToStaticBitmapImage(out_release_callback);
}
