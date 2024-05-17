 sk_sp<SkImage> ImageBitmap::getSkImageFromDecoder(
    std::unique_ptr<ImageDecoder> decoder) {
  if (!decoder->frameCount())
    return nullptr;
  ImageFrame* frame = decoder->frameBufferAtIndex(0);
  if (!frame || frame->getStatus() != ImageFrame::FrameComplete)
    return nullptr;
  DCHECK(!frame->bitmap().isNull() && !frame->bitmap().empty());
  return frame->finalizePixelsAndGetImage();
}
