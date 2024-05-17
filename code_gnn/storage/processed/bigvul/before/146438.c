PassRefPtr<Image> WebGLRenderingContextBase::VideoFrameToImage(
    HTMLVideoElement* video) {
  IntSize size(video->videoWidth(), video->videoHeight());
  ImageBuffer* buf = generated_image_cache_.GetImageBuffer(size);
  if (!buf) {
    SynthesizeGLError(GL_OUT_OF_MEMORY, "texImage2D", "out of memory");
    return nullptr;
  }
  IntRect dest_rect(0, 0, size.Width(), size.Height());
  video->PaintCurrentFrame(buf->Canvas(), dest_rect, nullptr);
  return buf->NewImageSnapshot();
}
