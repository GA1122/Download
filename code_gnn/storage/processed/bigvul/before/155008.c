scoped_refptr<Image> WebGLRenderingContextBase::VideoFrameToImage(
    HTMLVideoElement* video,
    int already_uploaded_id,
    WebMediaPlayer::VideoFrameUploadMetadata* out_metadata) {
  const IntSize& visible_size = video->videoVisibleSize();
  if (visible_size.IsEmpty()) {
    SynthesizeGLError(GL_INVALID_VALUE, "tex(Sub)Image2D",
                      "video visible size is empty");
    return nullptr;
  }
  CanvasResourceProvider* resource_provider =
      generated_image_cache_.GetCanvasResourceProvider(visible_size);
  if (!resource_provider) {
    SynthesizeGLError(GL_OUT_OF_MEMORY, "texImage2D", "out of memory");
    return nullptr;
  }
  IntRect dest_rect(0, 0, visible_size.Width(), visible_size.Height());
  video->PaintCurrentFrame(resource_provider->Canvas(), dest_rect, nullptr,
                           already_uploaded_id, out_metadata);
  return resource_provider->Snapshot();
}
