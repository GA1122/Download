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
  ImageBuffer* buf = generated_image_cache_.GetImageBuffer(visible_size);
  if (!buf) {
    SynthesizeGLError(GL_OUT_OF_MEMORY, "texImage2D", "out of memory");
    return nullptr;
  }
  IntRect dest_rect(0, 0, visible_size.Width(), visible_size.Height());
  video->PaintCurrentFrame(buf->Canvas(), dest_rect, nullptr,
                           already_uploaded_id, out_metadata);
  return buf->NewImageSnapshot();
}
